#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <experimental/optional>

using namespace std;
using namespace std::experimental;

typedef int Node;
typedef vector <vector <Node>> Graph;

// Simula una red de flujo en un grafo sin pesos.
// Esta red es igual a la del grafo original excepto por agrega dos nodos como
// fuente y sumidero, y todos los otros nodos se separan entre "left" y
// "right", con una arista del primero al segundo.
// Para todas las aristas entrantes a cualquier nodo n del grafo original hay
// una arista entrante a left(n); para todas las aristas salientes hay una
// arista saliente de right(n).
// También se pueden agregar aristas de la fuente y al sumidero.
struct FlowNetwork
{
	Graph G;
	const Node S, T;

	// Dar el corte mínimo del grafo usando el algoritmo de Edmonds-Karp.
	int MinCut()
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

	string print(Node n)
	{
		if (n == S)
			return "S";

		if (n == T)
			return "T";

		return to_string(n / 2 + 1) + " " + (n % 2 == 0 ? "left" : "right");
	}

	Node left(int n)
	{
		if (n == S || n == T)
			return n;

		return 2 * n;
	}

	Node right(int n)
	{
		if (n == S || n == T)
			return n;

		return 2 * n + 1;
	}

	void addEdge(int f, int t)
	{
		G[right(f)].push_back(left(t));
		G[right(t)].push_back(left(f));
	}

	FlowNetwork(int n)
	: S(2 * n), T(2 * n + 1)
	{
		G = Graph(2 * n + 2);
		for (int i = 0; i < n; i++)
			G[left(i)].push_back(right(i));
	}
};

int main()
{
	int n, m;
	while (cin >> n >> m)
	{
		FlowNetwork fn(n);
		for (int i = 0; i < n; i++)
		{
			char c; cin >> c;
			if (c == 'E')
				fn.addEdge(i, fn.T);
			if (c == 'A')
				fn.addEdge(fn.S, i);
		}

		for (int i = 0; i < m; i++)
		{
			int a, b; cin >> a >> b; a -= 1; b -= 1;
			fn.addEdge(a, b);
		}

		cout << fn.MinCut() << endl;
	}

	return 0;
}
