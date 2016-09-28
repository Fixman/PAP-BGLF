#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

struct Edge {
    int node, id;
    Edge() { node = -1, id = -1; }
    Edge(int n, int i) { node = n, id = i; }
};

typedef vector<int> vi;
typedef stack<int> si;
typedef vector<Edge> vEdge;

#define forsn(i,s,n) for(int i=(int)s; i<(int)n; i++)
#define forn(i,n) forsn(i,0,n)
#define pb push_back

vector<vEdge> graph;
vi low;
vi depth;
vector<bool> bridges; 
vi CCSizes;
vi CCNodes;

bool DEBUG = true;

void buildNewCC(int v, si &nodes){
    int newCC = 0;
    while(!nodes.empty() && nodes.top() != v){
        cerr << nodes.top() << endl;
        CCNodes[nodes.top()] = CCSizes.size();
        nodes.pop();
        newCC++;
    }
    CCSizes.pb(newCC);
}

void dfs(int v, int d, int p, si &nodes){
    depth[v] = d;
    low[v] = d;
    nodes.push(v);
    for(auto adj : graph[v]){
        int w = adj.node;
        if(depth[w] == -1 && w != p){
            dfs(w,d+1,v,nodes);
            low[v] = min(low[v], low[w]);
            if(low[w] >= depth[w]){
                // Puente
                bridges[adj.id] = true;
                if(DEBUG) cerr << "Puente:\n\tArista " << adj.id+1 << "\n\t(" << v+1 << "," << w+1 << ")" << endl;
                // Construimos la componente conexa sin el puente 
                buildNewCC(v, nodes);
                if(DEBUG) cerr << "Nueva CC: " << CCSizes[CCNodes[w]] << endl;
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
    CCSizes = vi(0);
    CCNodes = vi(N, -1);
    
    forn(i,N){ 
		if(depth[i] == -1){ 
			si nodes;
			dfs(i,0,i,nodes);
            buildNewCC(-1, nodes);
            if(DEBUG) cerr << "Nueva CC: " << CCSizes[CCNodes[i]] << endl;
		}
	}
    // Queries
    int QS; cin >> QS;
    forn(i,QS){
        char qi; cin >> qi;
        if(qi == 'A'){
            int u, v; cin >> u >> v; u--; v--;
            // Lanzamos un BFS desde u hasta v, reconstruimos el camino 
            // y contamos la cantidad de puentes en el camino. 
            vi pred(N, -1);
            vi predEdge(N, -1);
            queue<int> Q; Q.push(u);
            pred[u] = u;
            predEdge[u] = -1;
            while(!Q.empty()){
                int x = Q.front(); Q.pop();
                if(x == v) break;
                for(auto adj : graph[x]){
                    int y = adj.node;
                    if(pred[y] == -1){
                        pred[y] = x;
                        predEdge[y] = adj.id;
                        Q.push(y);
                    }
                }
            }
            int RTA = 0;
            int currNode = v;
            while(currNode != u){
                if(bridges[predEdge[currNode]]){
                    RTA++;
                }
                currNode = pred[currNode];
            }
            cout << RTA << endl;
        }else if(qi == 'B'){
            int e; cin >> e; e--; cout << bridges[e] << endl;
        }else if(qi == 'C'){
            int v; cin >> v; v--; cout << CCSizes[CCNodes[v]]-1 << endl;
        }
    }
    return 0;
}
