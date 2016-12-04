#include <iostream>
#include <map>
#include <set>
#include <cmath>
#include <cassert>

typedef long long tint;
typedef long double ldouble; 


#define forn(i,n) for(tint i=0;i<(tint)(n); i++)
#define debug(x) cout << #x << " = "  << x << endl


using namespace std;

struct Punto 
{
	tint x,y;
	Punto (tint xx, tint yy)
	{
		x = xx;
		y = yy;
	}
	Punto()
	{
		x = 0;
		y = 0;
	}
};

bool operator < (Punto p1, Punto p2)
{
	return make_pair(p1.x,p1.y) < make_pair(p2.x,p2.y);
}

bool operator != (Punto p1, Punto p2) 
{
	return (p1.x != p2.x) or (p1.y != p2.y);
}

struct Segmento
{
	Punto p,q;
	Segmento(Punto pp, Punto qq)
	{
		
		p = min(pp,qq);// importante tomar un representante
		q = max(pp,qq);// 		para los segmentos
	}
};

bool operator < (Segmento s1, Segmento s2)
{
	return make_pair(s1.p,s1.q) < make_pair(s2.p,s2.q);
}



const tint minCoord = -999999999999999999;

int main()
{
	#ifdef ACMTUYO
		assert(freopen("ej1.in", "r", stdin));
	#endif
	ios_base::sync_with_stdio(0);
	cin.tie(NULL);
	tint n;
	while (cin >> n)
	{
		map<Segmento,tint> contador;
		forn(i,n-2)
		{
			tint x1,y1,x2,y2,x3,y3;
			cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
			contador[Segmento(Punto(x1,y1),Punto(x2,y2))]++;
			contador[Segmento(Punto(x1,y1),Punto(x3,y3))]++;
			contador[Segmento(Punto(x2,y2),Punto(x3,y3))]++;
		}
		map<Punto,set<Punto> > poligono;
		for (auto x : contador)
			if (x.second == 1)
			{
				poligono[x.first.p].insert(x.first.q);
				poligono[x.first.q].insert(x.first.p);
			}
		Punto comienzo = (*poligono.begin()).first;
		cout << comienzo.x << " " << comienzo.y;
		Punto actual = Punto(minCoord,minCoord);
		Punto anterior = comienzo;
		for (auto p : poligono[comienzo])
			if (p.y > actual.y)
				actual = p;
		forn(i,n-1)
		{
			cout << " " << actual.x << " " << actual.y;
			for (auto p : poligono[actual]) // notar que es un for de largo 2
				if (p != anterior)
				{
					anterior = actual;
					actual = p;
					break;
				}
		}
		cout << "\n";
	}
	
	return 0;
}



