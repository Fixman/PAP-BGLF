#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <experimental/optional>

using namespace std;
using namespace std::experimental;

typedef int Node;
typedef vector <vector <Node>> Graph;

// Dar el corte mínimo del grafo usando el algoritmo de Edmonds-Karp.
int MinCut(	Graph &G,Node S,Node T)
{
	// Flujo que hay entre cada par de nodos <x, y>.
	// Cada elemento puede ser igual a 0 (no pasa flujo), 1 (pasa flujo
	// de x a y), o -1 (pasa flujo de y a x).
	vector <vector <int>> flow(G.size(), vector <int> (G.size(), 0));

	// Buscar caminos de augmento que mejoren el flujo máximo usando BFS.
	// La cantidad de caminos de augmento está acotada en N * M en el
	// caso general (Cormen, teorema 26.8). De hecho, como hay una nueva
	// arista dirigida con peso 1 por cada uno de los N nodos originales
	// el flujo máximo F está acotado por N, y esto también acota la
	// cantidad de caminos en N. Por lo tanto, hay O(N) caminos.
	int maxFlow = 0;
	while (true)
	{
		// El predecesor de cada nodo en el camino de augmento.
		vector <optional<Node>> pred(G.size());

		// Hacer DFS desde la fuente hasta el sumidero.
		// Esto visita una vez cada nodo, y por cada nodo recorre
		// todas sus aristas, por lo que tarda O(M) iteraciones.
		queue <Node> q;
		q.push(S);
		while (!q.empty())
		{
			Node c = q.front(); q.pop();
			for (auto &e : G[c])
			{
				// El siguiente nodo puede ser parte del camino de augmento
				// si no es parte del camino actual ni de alguno anterior.
				if (!pred[e] && e != S && flow[c][e] < 1)
				{
					pred[e] = c;
					q.push(e);
				}
			}
		}

		// Si no se encontró ningún camino de augmento hasta el sumidero,
		// el algoritmo ya tiene el corte óptimo.
		if (!pred[T])
			break;

		// Recorrer el camino de augmento 
		int df = 1 - flow[T][*pred[T]];
		for (Node e = *pred[T]; pred[e]; e = *pred[e])
			df = min(df, 1 - flow[e][*pred[e]]);

		for (Node e = T; pred[e]; e = *pred[e])
		{
			flow[*pred[e]][e] += df;
			flow[e][*pred[e]] -= df;
		}

		maxFlow += df;
	}

	return maxFlow;
}

int main(){
	int a,d;cin>>a>>d;
	vector<vector<int> > p(a,vector<int>(d));
	for(int i=0;i<a;i++)
		for(int j=0;j<d;j++)
			cin>>p[i][j];
	
	Graph G(2*a+2);
	Node S=0;Node T=2*a+1;
	
	//Conectamos la fuente y el sumidero
	for (int i = 0; i < a; i++)
	{
		G[S].push_back(i+1);
		G[i+1].push_back(S);
		G[i+1+a].push_back(T);
		G[T].push_back(i+1+a);
	}
	
	
	//Conectamos la primera columna con la segunda
	//Este procesamiento tiene complejidad O(a*a*d)
	for(int i=0;i<a;i++)
		for(int j=0;j<a;j++){
			if(j==i)continue;
			bool loAgrego = true;
			for(int t=0;t<d-1;t++){
				if(p[i][t]>p[j][t] && p[i][t+1]<p[j][t+1]){
					//si se cruzan en el medio
					loAgrego = false;
					break;
				}
				else if(p[i][t]<p[j][t] && p[i][t+1]>p[j][t+1]){
					//si se cruzan en el medio
					loAgrego = false;
					break;
				}else if(p[i][t]==p[j][t] || p[i][t+1]==p[j][t+1]){
					//si se cruzan en las puntas
					loAgrego = false;
					break;
				}else if(p[i][t]>p[j][t]){
					//solo agrego si la acción i-esima esta por debajo de la acción j-esima
					loAgrego = false;
					break;
				}
			}
			if(loAgrego){
				//Agrego solo si estoy por debajo de j
				G[i+1].push_back(j+1+a);
				G[j+1+a].push_back(i+1);
			}
		}
	//Calculamos el flujo maximo, como la cantidad de vertices
	//es 2a y la cantidad de aristas como máximo es a*a entonces
	//la complejidad es O(a*a*a)
	int k = MinCut(G,S,T);
	cout <<a-k<<endl;
	return 0;
}
