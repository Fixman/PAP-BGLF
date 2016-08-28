#include <iostream>

#define forn(i,n) for(int i=0;i<(int)(n); i++)

using namespace std;
/*
 * Idea:
 * Miro el grafico de las sumas acumuladas, reseteando cuando me paso
 * de 0. Y me voy quedando con los maximos.
 * 
 * Por que funciona: induccion en n
 * 	si n=1 entonces devuelve max(a_1,0)
 *  supongo que vale para n-1, veamos que vale para n
 * hasta el n-1 tengo el maxGlobal y el maxEnd entonces
 * si maxEnd+a_n es mayor que maxGlobal tengo una
 * sucesion que suma mas y termina en a_n entonces 
 * le asigno este valor a maxGlobal, 
 * sino maxGloba sigue siendo la que suma mas.
 * 
 * 
 * complejidad, es lineal en n 
*/


int main()
{
	int tmp,n;
	int maxGlobal=0;//me voy guardando el maximo hasta ahora
	int maxEnd=0;//me voy guardando el maximo que termina en el i en el que estoy parado
	cin >> n;
	forn(i,n){
		cin >> tmp;
		maxEnd = max(maxEnd+tmp,0);//si me paso a negativo reseteo
		maxGlobal = max(maxEnd,maxGlobal);
	}
	cout << maxGlobal << endl;
	return 0;
}



