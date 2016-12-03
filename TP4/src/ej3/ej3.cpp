#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

#define forsn(i,s,n) for(int i=(int)s; i<(int)n; i++)
#define forn(i,n) forsn(i,0,n)

typedef long long tint;

struct Pto {
    tint x, y, id;
    Pto() { x = y = id = 0; }
    Pto(tint xx, tint yy) : x(xx) , y(yy) { id = 0; }
    Pto operator+(Pto o) {
        return Pto(x+o.x, y+o.y);
    }
    Pto operator-(Pto o){
        return Pto(x-o.x, y-o.y);
    }
    tint pcruz(Pto o){
        return x*o.y - y*o.x;
    }
    tint normaCuadrado(){
        return x*x + y*y;
    }
};

bool operator<(const Pto&p, const Pto&q) {
    return tie(p.y, p.x) < tie(q.y, q.x);
}

tint signo(const Pto &p1, const Pto &p2, const Pto &p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool puntoEnTriangulo(const Pto &p, const Pto &t1, const Pto &t2, const Pto &t3) {
    bool b1 = signo(p, t1, t2) < 0;
    bool b2 = signo(p, t2, t3) < 0;
    bool b3 = signo(p, t3, t1) < 0;
    return ((b1 == b2) && (b2 == b3));
}

bool menorAnguloPivot(Pto p1, Pto p2, Pto pivot){
    Pto u = p1 - pivot;
    Pto v = p2 - pivot;
    tint area = u.pcruz(v);
    if(area != 0) return area > 0;
    return p1.normaCuadrado() < p2.normaCuadrado();
}

bool comparadorMenorAnguloPivot(int x, int y, int pivot, vector<Pto> &pts){
	return menorAnguloPivot(pts[x], pts[y], pts[pivot]);
}

bool esConvexo(Pto p1, Pto p2, Pto p3){
	Pto u = p3 - p2;
	Pto v = p1 - p2;
	return u.pcruz(v) > 0;
}

const tint MAXN = 45;
tint puntajeTriagunlos[MAXN][MAXN][MAXN];
bool trianguloValido[MAXN][MAXN][MAXN];
bool triangulosAdyacentes[MAXN][MAXN][MAXN];
tint dpTriangulos[MAXN][MAXN];

int main() {
    int H, E; 
    while(cin >> H >> E) {
        vector<Pto> puntosBuenos(H);
        vector<Pto> puntosMalos(E);
        forn(i,H) cin >> puntosBuenos[i].x >> puntosBuenos[i].y;
        forn(i,E) cin >> puntosMalos[i].x >> puntosMalos[i].y;
        
        // Ordenamos porque luego es util
        sort(puntosBuenos.begin(), puntosBuenos.end());
        forn(i,H) puntosBuenos[i].id = i;
        
        memset(puntajeTriagunlos, 0, sizeof(puntajeTriagunlos));
        memset(trianguloValido, true, sizeof(trianguloValido));
        
        // Preprocesamos el puntaje de todos los triangulos, que con O(N^3). 
        // Complejidad del preproceso es O(N^4).
        forn(i,H){
        forn(j,H){
        forn(k,H){
            if(i == j || i == k || j == k) continue;
            // Verificamos que el triangulo no contenga puntos enemigos.
            forn(z,E)
                if(puntoEnTriangulo(puntosMalos[z], puntosBuenos[i], puntosBuenos[j], puntosBuenos[k]))
                    trianguloValido[i][j][k] = false;
            if(!trianguloValido[i][j][k]) continue;
            // Calculamos el puntaje del triangulos validos iterando 
            // sobre los otros puntos buenos.
            forn(z,H){
                if(z == i || z == j || z == k) continue;
                if(puntoEnTriangulo(puntosBuenos[z], puntosBuenos[i], puntosBuenos[j], puntosBuenos[k]))
                    puntajeTriagunlos[i][j][k]++;
            }
        }}}
        
        // Calculo de la solucion: 
        tint MAXP = min(2, H);
        // Lo ultimos 2 puntos no podran ser pivot, porque no podran formar triangulos.
        forn(pivot,H-2){
            // Tomamos los puntos siguientes al pivot (que son los que estan mas abajo y derecha)
            // Los ordenamos por angulo relativo a pivot
            vector<tint> sigs;
            forsn(i,pivot+1,H) sigs.push_back(i);
            // Ordenamos los indices de los puntos en funcion del angulo que forman con el pivot
            sort(sigs.begin(), sigs.end(), bind(comparadorMenorAnguloPivot, placeholders::_1, placeholders::_2, pivot, puntosBuenos));
            
            // Ahora vemos que triangulos son adyacentes entre si, en funcion 
            // de que sean ambos validos, compartan el punto intermedio y formen un 
            // poligono convexo. 
            // Para todo par de puntos, que formen un triangulo valido, 
            // tomamos un tercer punto que forme un triangulo valido con el segundo punto, 
            // y si los tres puntos forman un angulo convexo, entonces los 
            // triangulos son adyacentes. Complejidad O(N^3).
            tint ss = sigs.size();
            memset(triangulosAdyacentes, false, sizeof(triangulosAdyacentes));
            forn(i,ss){
                int idxPi = sigs[i];
                
                forsn(j,i+1,ss){
                    int idxPj = sigs[j];
                    if(!trianguloValido[pivot][idxPi][idxPj]) continue;
                    
                    forsn(k,j+1,ss){
                        int idxPk = sigs[k];
                        if(!trianguloValido[pivot][idxPj][idxPk]) continue;
                        
                        Pto pi = puntosBuenos[idxPi];
                        Pto pj = puntosBuenos[idxPj];
                        Pto pk = puntosBuenos[idxPk];
                        if(esConvexo(pi,pj,pk)) triangulosAdyacentes[i][j][k] = true;
                    }
                }
            }
            // Dinamica: 
            // Para cada triangulo formado por (pivot,i,j) calculamos el mejor 
            // puntaje acumulado que puede tener si ese es el ultimo triangulo. 
            // Para eso, tomamos todo otro triangulo (pivot,k,i) que vaya antes y 
            // que sea adyacente con (pivot,i,j), y la solucion de (pivot,i,j) 
            // sera el maximo entre su puntaje (+3), o su puntaje (+1) mas la solucion 
            // de (pivot,k,i). 
            // Cuando iteramos k, lo hacemos sobre los puntos anteriores a i (o sea, 
            // estan antes segun el angulo), de modo que sus soluciones ya fueron calculadas. 
            // Complejidad O(N^3).
            memset(dpTriangulos, 0, sizeof(dpTriangulos));
            forn(i,ss){
                int idxPi = sigs[i];
                
                forsn(j,i+1,ss){
                    int idxPj = sigs[j];
                    if(!trianguloValido[pivot][idxPi][idxPj]) continue;
                    
                    tint puntajePivotIJ = puntajeTriagunlos[pivot][idxPi][idxPj];
                    dpTriangulos[i][j] = 3 + puntajePivotIJ;
                    
                    forn(k,i)
                        if(triangulosAdyacentes[k][i][j])
                            dpTriangulos[i][j] = max(dpTriangulos[i][j], dpTriangulos[k][i] + puntajePivotIJ + 1);
                    
                    MAXP = max(MAXP, dpTriangulos[i][j]);
                }
            }
        }
        // Cada iteracion de pivot lleva O(N^3), por tanto todo este proceso es O(N^4). 
        
        // Sumado al preproceso que era O(N^4), en total el algoritmo tiene 
        // complejidad O(N^4). 
        cout << MAXP << endl;
    }
    return 0;
}
