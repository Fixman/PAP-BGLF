#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
 
#define forn(i,n) for(int i=0;i<(int)(n); i++)
 
typedef long long tint;
 
using namespace std;

tint LETRAS_ALFABETO = 26; // alfabeto ingles de 'a' - 'z'

tint MAX_LETRAS = 52; // Notar que el enunciado dice que pueden 
					  // ser mayusculas y minusculas.
					  // Aunque en los ejemplos son todas minusculas.
/****************
* Nodo del Trie *
****************/

struct Nodo 
{
	tint cantidadPrefijos; 	// Indica la cantidad de prefijos que  
							// utilizan al nodo
	vector<Nodo> hijos;	// Vector con los hijos del nodo.
	Nodo() 		// Inicializacion de un nodo
	{
		cantidadPrefijos = 0;
		hijos = {};	// Un nodo que no fue creado, no tiene hijos
	}
};

Nodo crearNodo() // Creacion de un nodo
{
	Nodo nodo = Nodo(); // Inicializamos un nodo
	forn(i,MAX_LETRAS)  // Inicializamos todos sus hijos
		nodo.hijos.push_back(Nodo());
	return nodo;		
}

/*******************
* Funcion Auxiliar *
*******************/

// 'a' - 'z' -> 0-25
// 'A' - 'Z' -> 26-51

tint char2Number (char x)
{
	if (x < 'a') 	// es mayuscula
		return LETRAS_ALFABETO + x - 'A';
	else 			// es minuscula
		return x - 'a';
}

/*********************
* Funciones del Trie *
*********************/

/* Como precondicion de "agregarPalabra" se tiene que:
  
 1) 0 <= indice <= palabra.size()

*/ 

void agregarPalabra (Nodo &trie, string &palabra, tint indice)
{
	tint n = palabra.size();
	trie.cantidadPrefijos++;
	if (indice < n)
	{
		tint letra = char2Number(palabra[indice]);
		if (trie.hijos[letra].hijos.empty()) 
			trie.hijos[letra] = crearNodo();
		agregarPalabra(trie.hijos[letra],palabra,indice+1);
	}
}

/* Como precondicion de "prefijosPalabra" se tiene que:
  
 * 1) La palabra tiene que haber sido agregada antes al Trie
 * 2) 0 <= indice <= palabra.size()

*/ 

tint prefijosPalabra (Nodo &trie, string &palabra, tint indice)
{
	tint n = palabra.size();
	if (indice < n)
	{
		tint letra = char2Number(palabra[indice]); 
		prefijosPalabra(trie.hijos[letra],palabra,indice+1);
	}
	else
		return trie.cantidadPrefijos;
}

 
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	#ifdef ACMTUYO
		assert(freopen("ej2.in", "r", stdin));
	#endif
	tint n;
	while (cin >> n)
	{
		Nodo trie = crearNodo(); 	// Creamos la raiz del trie
		vector<string> mail (n); 	// Aqui guardaremos todos los prefijos
									// que recuerdan de los mails
								 
		forn(i,n)
		{
			string d; 	// mail
			tint p;   	// largo del prefijo que recuerdan
			cin >> d >> p; 	// lectura de la entrada
			mail[i] = d.substr(0,p); 	// Nos quedamos con el prefijo
			agregarPalabra(trie,mail[i],0);	// Agregamos el prefijo en
											// 	en el Trie
											
		}
		
		tint maxi = -1; 
		forn(i,n)
			maxi = max(maxi,prefijosPalabra(trie,mail[i],0)); 
			// Tomamos el maximo de palabras que utilizan cada prefijo
				
		cout << maxi << "\n";
	}
	return 0;
}
