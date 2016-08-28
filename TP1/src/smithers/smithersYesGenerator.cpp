#include <bits/stdc++.h>
#include "matriz.h"

using namespace std;

typedef vector<int> vi;
typedef vector<Matriz> vm;

// Genera T casos de prueba para el problema smithers. 
// Cada uno de tamaño N, siempre con respuesta SI. 

const int P = 10007;

int main(){
    srand(time(NULL));
    int T, N; cin >> T >> N;
    // Para cada caso 
    for(int i=0; i<T; i++){
        // Generamos N matrices aleatorias
        vm mats(N);
        for(int j=0; j<N; j++){
            vi vals(9);
            for(int k=0; k<9; k++){
                vals[k] = rand() % P;
            }
            mats[j] = Matriz(vals);
        }
        // Generamos el resultado como el producto de un rango aleatorio
        int s = rand() % N;
        int e = rand() % N;
        int start = min(s,e);
        int end   = max(s,e);
        Matriz res = id();
        for(int j=start; j<=end; j++){
            res*=mats[j];
        }
        int L = end - start + 1;
        cout << N << " " << L << endl;
        cout << res;
        for(int j=0; j<N; j++) cout << mats[j];
    }
    return 0;
}
