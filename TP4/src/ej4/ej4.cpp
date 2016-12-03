#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>

#define MOD 1000000007
#define debug(x) cout<<#x<<": "<<x<<endl

using namespace std;

typedef long long int tint;

tint mcd(tint a, tint b){ 
	return (a==0)?b:mcd(b %a, a);
}

tint potLog(tint b, tint e, tint m) {
	if (!e) return 1LL;
	tint r=potLog(b, e>>1, m);
	r=(r*r) %m;
	return (e&1)?(r*b) %m:r;
}


int main(){
	int n;cin>>n;
	vector<int> p(n+1);
	
	//guardamos los elementos todavia no asignados a un ciclo
	set<int> libres;
	for(int i=1;i<n+1;i++){cin >> p[i];libres.insert(i);}
	
	//Identificamos los ciclos y guardamos la cantidad de veces que aparece
	//cada longitud
	map<int,int> apariciones; 
	vector<int> longitud;
	int t=0; //cuantos ciclos en total hay
	int k;
	
	while(libres.size()>0){
		set<int> ciclo;
		k=*libres.begin();
		ciclo.insert(k);
		libres.erase(k);
		
		while(ciclo.find(p[k])==ciclo.end()){
			ciclo.insert(p[k]);
			k=p[k];
			libres.erase(k);
		}
		apariciones[ciclo.size()]++;
		longitud.push_back(ciclo.size());
		t++;
	}
	
	//Calculo de grafos invariantes por P
	tint count=1;	
	
	//Agrego las opciones dadas por los ciclos individualmente
	for(int i=0;i<(int)longitud.size();i++)
			count*=potLog(2,apariciones[longitud[i]]*(longitud[i]-1)/2,MOD);
	
	//Agrego las opciones dadas por las relaciones entre ciclos distintos
	for(int i=0;i<(int)longitud.size();i++)
		for(int j=i+1;j<(int)longitud.size();j++)
				count*=potLog(2,apariciones[longitud[i]]*
					apariciones[longitud[j]]*mcd(longitud[i],longitud[j]),MOD);
	
	cout << count << endl;
}
