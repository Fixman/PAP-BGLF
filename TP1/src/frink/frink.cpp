#include <iostream>

#define forn(i,n) for(int i=0;i<(int)(n); i++)

using namespace std;

int main()
{
	int tmp,n;
	int maxGlobal=0;
	int maxEnd=0;
	cin >> n;
	forn(i,n){
		cin >> tmp;
		//voy llevando la suma parcial truncada
		maxEnd = max(maxEnd+tmp,0);
		//actualizo el maximo global 
		maxGlobal = max(maxEnd,maxGlobal);
	}
	cout << maxGlobal << endl;
	return 0;
}



