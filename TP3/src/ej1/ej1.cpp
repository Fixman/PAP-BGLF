#include <iostream>
#include <vector>

using namespace std;

vector<int> calcularBorde(string &p){
	int m = p.size();
	vector<int> borde(m);
	
	//~ inicializamos en -1 porque los arreglos los indexo desde 0
	borde[0]=-1;
	
	//~ contador de cuantos caracteres coincido -1
	int k=-1;
	
	for(int q=1;q<m;q++){
		//~ si el proximo caracter no coincide retrocedo
		while(k>=0 && p[k+1]!=p[q])
			k=borde[k];
		//~ si coincide aumento el contador
		if(p[k+1]==p[q])
			k++;
		borde[q]=k;
	};
	return borde;
}

bool kmpMatcher(string t, string p){
	int n = t.size();
	int m = p.size();
	vector<int> borde = calcularBorde(p);
	
	//~ cantidad de caracteres que matcheamos -1
	//~ porque el indice de los arreglos va desde 0
	int q = -1;	
	
	//~ recorremos el texo de izquierda a derecha
	for(int i=0;i<n;i++){
		
		
		//~ si los caracteres no coinciden empiezo a 
		//~ comparar desde una posicion anterior, dada por la
		//~ tabla de bordes
		while(q>=0 && p[q+1]!=t[i])	
			q=borde[q];
		
		
		//~ si los caracteres coinciden avanzamos uno mas
		if(p[q+1]==t[i])
			q++;
		
		
		//~ nos preguntamos si ya llegamos al final del 
		//~ patron, es decir si matcheamos todo el patron
		//~ notar el -1, pues indexamos desde 0
		if(q==m-1)
			return true;
	}
	return false;
}

int main(){
	string t,p;cin >> t >> p;
	if(kmpMatcher(t,p))cout<<"S"<<endl;
	else cout << "N"<<endl;
	return 0;
}
