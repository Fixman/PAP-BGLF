#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef long long tint; 

#define forn(i,n) for(tint i = 0; i < tint (n); i++)
#define forsn(i,s,n) for(tint i = tint(s); i < tint (n); i++)

// Cuenta la cantidad de unos (amigas) en una mascara 
// Complejidad : O(log(mask)), o sea O(n), pues mask <= 2^n

tint cantidadAmigas (tint mask) 
{
	tint ans = 0;
	while (mask > 0)
	{
		ans += (mask%2);
		mask /= 2;
	}
	return ans;
}

// Dada una mascara, calcula la diversion del
// grupo de amigas que representa 

// Complejidad : O(log(mask) + r^2), 
// donde r es la cantidad de unos en
// mask, en particular nos queda O(n^2)

tint diversion (vector<vector<tint> > &d, tint mask)
{
	tint n = tint(d.size()) - 1; // indexamos de 0
	vector<tint> amigas;
	// Calcula los indices que tienen un uno en la mascara
	while (mask > 0) 
	{
		if (mask % 2)
			amigas.push_back(n);
		n--;
		mask /= 2;
	}
	tint r = amigas.size(),f = 0;
	// Aca usamos que la matriz es simetrica
	forn(i,r)
	forsn(j,i+1,r)
		f += d[amigas[i]][amigas[j]]; 
	return f;
} 





// Devuelve la respuesta al problema sobre el subconjunto
// que representa la mascara "mask"

// Complejidad (dado que se corre con mask = 2^n - 1) : O(n^2*2^n + 3^n) 
// (ver informe para detalles) 


tint best(vector<tint> &dp, vector<vector<tint> > &d, tint mask) 
{
	tint n = d.size();
	if (dp[mask] == -1)
	{
		if (cantidadAmigas(mask) == 1) // Caso base
			dp[mask]  = 0;
		else
		{
			// solucion inicial: no separamos en ningun grupo
			dp[mask] = diversion(d,mask); 
			// probamos la solucion en todas las particiones en 
			// 2 subconjuntos propios, y nos quedamos con la mejor.
			for(tint i = mask&(mask-1); i != 0; i = mask&(i-1)) 
				// Para evitar llamar dos veces el mismo caso, 
				// notar que el segundo numero es el "complemento de i" 
				//(en el subconjunto que representa mask)
				if (i > (mask & (i ^ ((1 << n)-1)))) 
					dp[mask] = max(dp[mask],best(dp,d,i) + best(dp,d,mask & (i ^ ((1 << n)-1))));
		}
	}
	return dp[mask];
	
}


int main()
{
	assert(freopen("lisa.in", "r", stdin));
	tint n;
	while(cin >> n)
	{
		vector<vector<tint> > d (n,vector<tint> (n));
		forn(i,n)
		forn(j,n)
			cin >> d[i][j];
		// En dp[i] guardamos la solucion al problema usando solo las 
		// amigas que tienen '1' en la representacion binaria de i 
		vector<tint> dp (1 << n,-1);
		cout << best(dp,d, (1 << n) - 1) << "\n"; // Devolvemos la solucion 		
	}
	return 0;
}


