#include <bits/stdc++.h>
#include "matriz.h"

using namespace std;

typedef vector<int> vi;
typedef vector<Matriz> vm;

// Devuelve el maximo K tal que 2^K <= N
int maxExp2Menor(int N) {
    return (sizeof(N)*8 - __builtin_clz(N) - 1);
}

// Devuelve los indices de los bits en 1 de L
vi bitsEn1(int L) {
    vi res;
    int idx=0;
    while(L>0){
        if(L%2) res.push_back(idx);
        L/=2;
        idx++;
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
        // Al principio inicializamos todos los productos[i][k] 
        // para todo k con el elemento i, que en definitiva es productos[i][0].
        // productos[][] tiene tamanio O(N*log(N)), dado que se crean N arreglos 
        // cada uno de tamanio exp que es O(log(N)), porque 2^exp <= N
        for(int i=0; i<N; i++) productos[i] = vm(exp, M[i]);
        
        // Llenamos la tabla con las productorias. 
        // Inicialmente, productos[i][j] = productos[i][j-1], es decir, 
        // la potencia anterior que comienza en i. De ser posible, 
        // se multiplica por el subarreglo siguiente de longitud 2^(j-1), 
        // que estara en productos[i+2^(j-1)][j-1], 
        // en caso de que haya mas elementos adelante todavia. 
        // Calculamos las productorias en orden del exponente, es decir, 
        // primero todas las de largo 2^1, luego las de largo 2^2, y asi. 
        // De este modo, para calcular una productoria de largo 2^j, simplemente 
        // usamos 2 productorias de largo 2^(j-1) que ya fueron calculadas, 
        // y por tanto cada productoria es O(1). Finalmente, calcular 
        // todas las productorias es O(N*log(N)). 
        for(int j=1; j<exp; j++) {
            for(int i=0; i<N; i++) {
                productos[i][j] = productos[i][j-1];
                int offset = (1 << (j-1));
                if(i+offset < N) productos[i][j] *= productos[i+offset][j-1];
            }
        }
    }
    
    vm calcularSubArreglos(int L) {
        // Calculamos las productorias de los todos sub-arreglos de 
        // largo exactamente L en el arreglo N (que son N-L+1 exactamente). 
        // Para esto, usamos productos[][] y el desarrollo binario de L. 
        // Supongamos que queremos la productoria P del sub-arreglo que 
        // comienza en i (con 0 <= i < N-L, es decir que el sub-arreglo 
        // no se va fuera del arreglo original). 
        
        // Llevamos un indice idx que comienza en i. 
        // Iteramos sobre los bits de L. Sea j un bit de L; 
        // si j esta en 1, quiere decir que lo necesitamos para formar L, 
        // es decir que necesitamos sumar 2^j para formar L. Entonces, 
        // cuando j es 1, agregamos la productoria de longitud 2^j 
        // que comienza en idx a nuestro resultado, que no es otra cosa 
        // que productos[idx][j]. Luego de esto, incrementamos idx en 2^j, 
        // dado que esos elementos ya los incluimos en P. 
        
        // Al terminar de iterar sobre los bits de L, tendremos P. 
        // Como L <= N, L tiene log(N) bits, por tanto armar P cuesta log(N). 
        // Como hacemos esto para cada sub-arreglo, y hay O(N) de estos, 
        // podemos calcular todos en O(N*log(N)). 
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
