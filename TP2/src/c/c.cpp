#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int node, id;
    Edge() { node = -1, id = -1; }
    Edge(int n, int i) { node = n, id = i; }
};

typedef vector<int> vi;
typedef vector<Edge> vEdge;

#define forsn(i,s,n) for(int i=(int)s; i<(int)n; i++)
#define forn(i,n) forsn(i,0,n)
#define pb push_back

vector<vEdge> graph;
vi low;
vi depth;
vector<bool> bridges; 

void dfs(int v, int d, int p, vi nodes){
    depth[v] = d;
    low[v] = d;
    for(auto adj : graph[v]){
        int w = adj.node;
        if(depth[w] == -1 && w != p){
            dfs(w,d+1,v,nodes);
            low[v] = min(low[v], low[w]);
            if(low[w] >= depth[w]){
                // Puente
                bridges[adj.id] = true;
                cerr << "Puente:\n\tArista " << adj.id+1 << "\n\t(" << v+1 << "," << w+1 << ")" << endl;
            }
        }else if(w != p){
            low[v] = min(low[v], depth[w]);
        }
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
    bridges = vector<bool>(M, false);
    vi nodes;
    forn(i,N) if(depth[i] == -1) dfs(i,0,i,nodes);
    return 0;
}
