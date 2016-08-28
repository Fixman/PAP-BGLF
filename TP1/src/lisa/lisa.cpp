#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef long long tint; // uso long long porque me gusta, no hace falta :-P

#define forn(i,n) for(tint i = 0; i < tint (n); i++)
#define forsn(i,s,n) for(tint i = tint(s); i < tint (n); i++)

//Complejidad : O(log(mask)), o sea O(n), pues mask <= 2^n
tint cantidadAmigas (tint mask) // Cuenta la cantidad de unos (amigas) en una mascara 
{
	tint ans = 0;
	while (mask > 0)
	{
		ans += (mask%2);
		mask /= 2;
	}
	return ans;
}

// Complejidad : O(log(mask) + r^2), donde r es la cantidad de unos en mask, en particular nos queda O(n^2)
tint felicidad (vector<vector<tint> > &d, tint mask) // Dada una mascara, calcula la felicidad del grupo de amigas que representa 
{
	tint n = d.size() - 1;
	vector<tint> amigas;
	while (mask > 0) // Calcula los indices que tienen un uno en la mascara
	{
		if (mask % 2)
			amigas.push_back(n);
		n--;
		mask /= 2;
	}
	tint r = amigas.size(),f = 0;
	forn(i,r)
	forsn(j,i+1,r)
		f += d[amigas[i]][amigas[j]]; // Aca usamos que la matriz es simetrica
	return f;
} 

// Complejidad (dado que se corre con mask = 2^n - 1) : O(n^2*2^n) (creo, revisar esto bien) 
tint best(vector<tint> &dp, vector<vector<tint> > &d, tint mask) // Devuelve la respuesta al problema sobre el subconjunto que representa la mascara "mask"
{
	tint n = d.size();
	if (dp[mask] == -1)
	{
		if (cantidadAmigas(mask) == 1) // Caso base
			dp[mask]  = 0;
		else
		{
			dp[mask] = felicidad(d,mask); // solucion inicial: no separamos en ningun grupo
			for(tint i = mask&(mask-1); i != 0; i = mask&(i-1)) // probamos la solucion en todas las particiones en 2 subconjuntos propios, y me quedo con la mejor.
				if (i > (mask & (i ^ ((1 << n)-1)))) // Para evitar llamar dos veces el mismo caso, notar que el segundo numero es el "complemento de i" (en el subconjunto que representa mask)
					dp[mask] = max(dp[mask],best(dp,d,i) + best(dp,d,mask & (i ^ ((1 << n)-1))));
		}
	}
	return dp[mask];
	
}


int main()
{
	assert(freopen("entradaLisa.in", "r", stdin));
	tint n;
	while(cin >> n)
	{
		vector<vector<tint> > d (n,vector<tint> (n));
		forn(i,n)
		forn(j,n)
			cin >> d[i][j];
		vector<tint> dp (1 << n,-1); // En dp[i] guardo la solucion al problema usando solo las amigas que tienen '1' en la representacion binaria de i 
		cout << best(dp,d, (1 << n) - 1) << "\n"; // Devolvemos la solucion 		
	}
	return 0;
}


