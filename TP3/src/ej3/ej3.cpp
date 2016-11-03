#include <iostream>
#include <vector>

using namespace std;

typedef long long tint;

int C, A, Q;

void imprimirMatriz(vector<vector<tint> > &m) {
	for(const auto &v : m){
		for(const auto &e : v){
			cout << e << " ";
		}
		cout << endl;
	}
}

int main() {
	while(cin >> C >> A >> Q) {
		vector<vector<tint> > montos(C, vector<tint>(A));
		// Leemos los montos por cargo/antiguedad
		for(int i=0; i<C; i++)
		for(int j=0; j<A; j++)
			cin >> montos[i][j];
			
		vector<vector<tint> > sumasMontosParciales(C, vector<tint>(A));
		// Calcularemos las sumas parciales de los montos para todo (i,j) desde (0,0) hasta 
		// (C-1,A-1). 
		// En sumasMontosParciales[i][j] guardaremos la suma de todos los montos desde el 
		// (0,0) hasta el (i,j) inclusive. 
		
		// La suma parcial hasta en (0,0) es simplemente el monto (0,0). 
		sumasMontosParciales[0][0] = montos[0][0];
		
		// Para la primer fila/columna, simplemente debemos ver el nuevo elemento, 
		// y tomar la suma parcial hasta el elemento anterior, que es la suma de todos los elementos 
		// anteriores en esa fila/columna. 
		for(int i=1; i<C; i++) sumasMontosParciales[i][0] = montos[i][0] + sumasMontosParciales[i-1][0];
		for(int i=1; i<A; i++) sumasMontosParciales[0][i] = montos[0][i] + sumasMontosParciales[0][i-1];
		for(int i=1; i<C; i++){
			for(int j=1; j<A; j++){
				// En el caso general, si queremos la suma de todos los elementos 
				// desde el (0,0) hasta el (i,j) inclusive, primero que nada debemos tomar 
				// el elemento (i,j), que no lo tenemos en otro lado. 
				// En sumasMontosParciales[i-1][j] tenemos la suma de todos los elementos 
				// hasta la fila anterior, y hasta la misma columna, y 
				// en sumasMontosParciales[i][j-1] tenemos la suma de todos los elementos 
				// hasta la columna anterior, y hasta la misma fila. 
				// Si sumamos estas 3 cosas, tendremos en principio 
				// - La suma de la i-esima fila una vez, porque solo aparece con 
				//   sumasMontosParciales[i][j-1] sumado a montos[i][j]. 
				// - La suma de la j-esima columna una vez, porque solo aparece con 
				//   sumasMontosParciales[i-1][j] sumado a montos[i][j]. 
				// - Y tenemos todo el rectangulo desde (0,0) hasta (i-1,j-1) inclusive contado 
				//   2 veces, una vez sumasMontosParciales[i][j-1] y otra por sumasMontosParciales[i-1][j]. 
				// Pero justamente la seccion que tenemos contada 2 veces es exactamente 
				// sumasMontosParciales[i-1][j-1], entonces podemos restar este resultado que ya tenemos 
				// calculado y asi contaremos dicha seccion 1 sola vez. 
				// En total ahora tenemos cada elemento desde (0,0) hasta (i,j) 
				// contado exactamente una vez, que es lo que queriamos. 
				// La complejidad para calcular cada elemento de sumasMontosParciales 
				// es O(1) porque hacemos una cantidad constante de operaciones aritmeticas. 
				// En total llenar la tabla tiene complejidad O(A*C) por ende. 
				sumasMontosParciales[i][j] = montos[i][j] + sumasMontosParciales[i-1][j] + sumasMontosParciales[i][j-1] - sumasMontosParciales[i-1][j-1];
			}
		}
		
		tint c1, a1, c2, a2;
		for(int i=0; i<Q; i++){
			cin >> c1 >> a1 >> c2 >> a2; 
			c1--; a1--; c2--; a2--;
			
			// Para cada query sabemos que c1 <= c2 y a1 <= a2. 
			// Lo que nos pide cada query es la suma de los elementos desde 
			// (c1 + 1 , a1 + 1) hasta (c2 , a2) inclusive (no queremos contar los elementos 
			// de la fila c1 ni de la columna a1, por eso sumamos 1 a los indices). 
			// Lo que tenemos calculado en cada elemento de sumasMontosParciales son las 
			// sumas hasta un cierto indice, pero todas desde el (0,0). 
			// Pero podemos hacer algo similar al precomputo para obtener la suma que queremos. 
			// Si tomamos sumasMontosParciales[c2][a2] es la suma desde el (0,0) hasta el (c2,a2) inclusive, 
			// queremos quitarle todo lo que esta desde la fila c1 (inclusive) hacia arriba 
			// en las columnas desde la 0 hasta a2, 
			// y debemos quitarle todo lo que esta desde la columa a1 (inclusive) hacia la izquierda  
			// en las filas desde la 0 hasta c2. 
			// Estas 2 secciones se encuentran exactamente en 
			// sumasMontosParciales[c1][a2] y sumasMontosParciales[c2][a1] respectivamente, entonces 
			// se las restamos a sumasMontosParciales[c2][a2]. 
			// El problema como antes, es que asi estamos restando 2 veces la seccion que va desde 
			// el (0,0) hasta (c1,a1) inclusive, pero esto se encuentra en sumasMontosParciales[c1][a1] 
			// entonces podemos sumarlo al resultado para restarlo 1 vez en lugar de 2. 
			// Asi entonces tenemos solamente los 
			// elementos desde (c1 + 1 , a1 + 1) hasta (c2 , a2) inclusive contados exactamente una vez 
			// que es lo que queriamos. 
			// La complejidad de resolver cada query entonces es O(1) porque hacemos 
			// una cantidad constante de operaciones aritmeticas. 
			// Resolver las Q queries entonces tiene complejidad O(Q). 
			tint diferenciaCargoAntiguedad = sumasMontosParciales[c2][a2] - sumasMontosParciales[c2][a1] - sumasMontosParciales[c1][a2] + sumasMontosParciales[c1][a1];
			cout << diferenciaCargoAntiguedad << endl;
		}
		// La complejidad total del algoritmo es la complejidad del precomputo 
		// mas la complejidad de resolver cada query. 
		// Entonces la complejidad en total es O(A*C + Q) como buscabamos. 
	}
	return 0;
}
