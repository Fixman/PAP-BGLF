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

class subArreglosDeL {
    const vm &M;
    vector<vm> productos;
    
public:
    subArreglosDeL(const vm &matrices) : M(matrices) {
        int N = (int)M.size();
        int exp = maxExp2Menor(N)+1;
        productos = vector<vm>(N);
        
        // productos[i][j] posee el producto del arreglo que empieza en 
        // i con longitud 2^j. 
        // Inicializamos todos los productos con el primer elemento.
        for(int i=0; i<N; i++) productos[i] = vm(exp, M[i]);
        
        // Llenamos la tabla de productorias. 
        // Inicialmente, productos[i][j] = productos[i][j-1], es decir, 
        // la potencia anterior. De ser posible, se agrega el subarreglo 
        // siguiente de tamaño 2^(j-1).
        for(int j=1; j<exp; j++) {
            for(int i=0; i<N; i++) {
                productos[i][j] = productos[i][j-1];
                int offset = (1 << (j-1));
                if(i+offset < N) productos[i][j] *= productos[i+offset][j-1];
            }
        }
    }
    
    vm calcularSubArreglos(int L) {
        // Calculamos las productorias de sub-arreglos de largo L. 
        // Usando producto[][] y el desarrollo binario de L, podemos 
        // construir los sub-arreglos de tamaño L en log(L), dado que 
        // un número L tendrá log(L) bits, y como mucho log(L) bits en 1. 
        int N = (int)M.size();
        vi bitsL = bitsEn1(L);
        vm subArreglos;
        for(int i=0; i<(N-L+1); i++) {
            int idx=i;
            Matriz res = id();
            for(auto b : bitsL) {
                res *= productos[idx][b];
                idx += (1<<b);
            }
            subArreglos.push_back(res);
        }
        return subArreglos;
    }
};

int main(){
    int N, L;
    while(cin >> N >> L) {
        vi matRes(9); 
        for(auto &d : matRes) cin >> d;
        Matriz matrizResultado(matRes);
        vm matrices(N);
        for(int i=0; i<N; i++) {
            vi mat(9);
            for(auto &d : mat) cin >> d;
            matrices[i] = Matriz(mat);
        }
        subArreglosDeL subArreglosL(matrices);
        vm arreglosDeL = subArreglosL.calcularSubArreglos(L);
        
        bool hayResultado = false;
        for(auto &m : arreglosDeL) hayResultado |= (m == matrizResultado);
        if(hayResultado) 
            cout << "SI\n";
        else 
            cout << "NO\n";
    }
    return 0;
}
