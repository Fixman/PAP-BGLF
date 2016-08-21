#include <bits/stdc++.h>
#include "matriz.h"

using namespace std;

typedef vector<int> vi;
typedef vector<Matriz> vm;

// Devuelve el máximo K tal que 2^K <= N
int maxExp2Menor(int N) {
    return (sizeof(N)*8 - __builtin_clz(N) - 1);
}

// Devuelve los índices de los bits en 1 de L
vi bitsEn1(int L) {
    vi res;
    for(int i=0; i<(int)sizeof(L)*8; i++) {
        if(L&(1<<i)) {
            res.push_back(i);
        }
    }
    return res;
}

int main(){
    int N, L;
    while(cin >> N >> L) {
        vi matRes(9); 
        for(auto &d : matRes) {
            cin >> d;
        }
        Matriz matrizResultado(matRes);
        vm matrices(N);
        for(int i=0; i<N; i++) {
            vi mat(9);
            for(auto &d : mat) {
                cin >> d;
            }
            matrices[i] = Matriz(mat);
        }
        int exp = maxExp2Menor(N)+1;
        vector<vm> prods(N);
        // Inicializamos todos los productos con el primer elemento
        for(int i=0; i<N; i++) {
            prods[i] = vm(exp, matrices[i]);
        }
        // Llenamos la tabla de productorias
        for(int j=1; j<exp; j++) {
            for(int i=0; i<N; i++) {
                int offset = (1 << (j-1));
                prods[i][j] = prods[i][j-1];
                if(i+offset < N) {
                    prods[i][j] *= prods[i+offset][j-1];
                }
            }
        }
        // Calculamos las productorias de sub-arreglos de largo L 
        bool hayResultado = false;
        vi bitsL = bitsEn1(L);
        for(int i=0; i<(N-L+1); i++) {
            int idx=i;
            Matriz res = id();
            for(auto b : bitsL) {
                res *= prods[idx][b];
                idx += (1<<b);
            }
            hayResultado |= (res == matrizResultado);
        }
        if(hayResultado) 
            cout << "SI\n";
        else 
            cout << "NO\n";
    }
    return 0;
}
