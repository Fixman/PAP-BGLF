#include <bits/stdc++.h>
#include "matriz.h"

using namespace std;

typedef vector<int> vi;

// Devuelve el máximo K tal que 2^K <= N
int maxExp2Menor(int N) {
    return (sizeof(N)*8 - __builtin_clz(N) - 1);
}

// Devuelve la máxima potencia de 2 menor o igual a N
int maxPot2Menor(int N) {
    return 1<<maxExp2Menor(N);
}

void printMat(vector<vi> &m) {
    for(auto &v : m) {
        for(auto &d : v) {
            cout << d << " ";
        }cout << endl;
    }
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
        vi nums(N);
        for(auto &d : nums) {
            cin >> d;
        }
        int exp = maxExp2Menor(N)+1;
        vector<vi> prods(N);
        // Inicializamos todos los productos con el primer elemento
        for(int i=0; i<N; i++) {
            prods[i] = vi(exp, nums[i]);
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
        printMat(prods);
        // Calculamos las productorias de sub-arreglos de largo L 
        vi bitsL = bitsEn1(L);
        for(int i=0; i<(N-L+1); i++) {
            int idx=i;
            int res=1;
            for(auto b : bitsL) {
                res *= prods[idx][b];
                idx += (1<<b);
            }
            cout << res << endl;
        }
    }
    return 0;
}
