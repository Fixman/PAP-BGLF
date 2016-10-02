#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int node, id;
    Edge() { node = -1, id = -1; }
    Edge(int n, int i) { node = n, id = i; }
};

typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<Edge> vEdge;

#define forsn(i,s,n) for(int i=(int)s; i<(int)n; i++)
#define forn(i,n) forsn(i,0,n)
#define pb push_back

vector<vEdge> graph;
vi depth;
vi low;
vb visit;
vector<bool> bridges; 
vi CCSizes;

// Hace el DFS calculando los puentes. 
// Ademas al encontrar un puente, genera la nueva componente conexa 
// delimitada por el puente. 
void dfsBridges(int v, int d, int p){
    depth[v] = low[v] = d;
    for(auto adj : graph[v]){
        int w = adj.node;
        if(w != p){
            if(depth[w] == -1){
                dfsBridges(w,d+1,v);
                low[v] = min(low[v], low[w]);
                // Puente
                if(low[w] >= depth[w]) bridges[adj.id] = true;
            }else{
                low[v] = min(low[v], depth[w]);
            }
        }
    }
}

void dfsComponents(int v, vi &nodes){
    visit[v] = true;
    nodes.pb(v);
    for(auto adj : graph[v]){
        int w = adj.node;
        int edge = adj.id;
        if(!bridges[edge] && !visit[w]) dfsComponents(w, nodes);
    }
}

int main(){
    int N, M; cin >> N >> M;
    graph = vector<vEdge>(N, vEdge());
    forn(i,M){
        int u, v; cin >> u >> v; u--; v--;
        graph[u].pb(Edge(v,i));
        graph[v].pb(Edge(u,i));
    }
    
    // Preproceso
    depth = vi(N, -1);
    low = vi(N, -1);
	visit = vb(N, false);
    bridges = vb(M, false);
    CCSizes = vi(N, 0);
    
    // DFS para calcular los puentes
    dfsBridges(0,0,0);
    // DFS para calcular las componentes conexas sin puentes
    forn(i,N) if(!visit[i]){
        vi nodes;
        dfsComponents(i,nodes);
        for(auto v : nodes) CCSizes[v] = nodes.size();
    }
    
    // Queries
    int QS; cin >> QS;
    forn(i,QS){
        char qi; cin >> qi;
        if(qi == 'A'){
            int u, v; cin >> u >> v; u--; v--;
            // Lanzamos un BFS desde 'u' hasta 'v', reconstruimos el camino 
            // y contamos la cantidad de puentes en el camino. 
            vi predBFS(N, -1);
            vi predEdge(N, -1);
            queue<int> Q; Q.push(u);
            predBFS[u] = u;
            predEdge[u] = -1;
            while(!Q.empty()){
                int x = Q.front(); Q.pop();
                if(x == v) break;
                for(auto adj : graph[x]){
                    int y = adj.node;
                    if(predBFS[y] == -1){
                        predBFS[y] = x;
                        predEdge[y] = adj.id;
                        Q.push(y);
                    }
                }
            }
            int RTA = 0;
            int currNode = v;
            while(currNode != u){
                if(bridges[predEdge[currNode]]) RTA++;
                currNode = predBFS[currNode];
            }
            cout << RTA << endl;
        }else if(qi == 'B'){
            // Devolvemos 1 cuando la calle 'e' es un puente, y 
            // 0 en el caso contrario. 
            int e; cin >> e; e--; cout << bridges[e] << endl;
        }else if(qi == 'C'){
            // Devolvemos el tamanio de la componente conexa libre 
            // de puentes que contiene a 'v' (menos 1, para no contar 'v').
            int v; cin >> v; v--; cout << CCSizes[v]-1 << endl;
        }
    }
    return 0;
}
