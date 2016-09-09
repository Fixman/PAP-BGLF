#include<bits/stdc++.h>

using namespace std;

bool primo(long long p) {
    for(long long i=2;i<=p/i;i++) {
        if(p%i==0) {
            return false;
        }
    }
    return true;
}

long long pot(long long base, int exp, long long mod) {
    long long res = 1;
    for(int i=0;i<exp;i++) {
        res = (res*base)%mod;
    }
    return res;
}

long long mult(long long a, long long b, long long seed) {
    if(b==0) {
        return 0;
    }
    long long res = mult((a+a)%seed,b/2,seed);
    if(b%2==1) {
        res = (res+a)%seed;
    }
    return res;
}

long long hashLeo(string s, long long seed) {
    assert(seed>(1LL<<32)&&seed<(1LL<<56));
    vector<long long> primos;
    int n = s.size();
    for(int i=2;primos.size()<=n;i++) {
        bool esPrimo = true;
        for(int j=0;j<primos.size();j++) {
            if(i%primos[j]==0) {
                esPrimo = false;
                break;
            }
        }
        if(esPrimo) {
            primos.push_back(i);
        }
    }
    while(!primo(seed))
        seed++;
    long long groupNumber = 1;
    for(int i=0;i<n;i++) {
        groupNumber = mult(groupNumber,pot(primos[i],(int)s[i],seed),seed);
    }
    long long i = primos[n];
    long long exp=(int)s[groupNumber%n];
    while(2LL*groupNumber < 10000000000000000LL) {
        groupNumber = 2LL*groupNumber+pot(i,exp,seed)%2;
        exp += (int) s[groupNumber%n];
    }
    return groupNumber;
}

string integrantes(int grupo) {
    string apellidos[17] =
        {"Contrufo,De Carli,Rodriguez,Tavolaro Ortiz,Vargas Telles(1)",
        "Gasperi Jabalera,Kantor,Maurizio,Russo,Tagliavini Ponce(2)",
        "Chamo,Donatucci,Noriega,Zimenspitz(3)",
        "Gomez,Guillamon,Rozenberg,Tanenzapf(4)",
        "Giordano,Hurovich,Palladino,Roulet(5)",
        "Ciraco,Heredia,Martinelli,Ventura(6)",
        "Barrios,Caravaria,Ginsberg,Truffat(7)",
        "Assenza,Candioti,Lang,Zylber(8)",
        "Chibana,Fosco,Minces Muller,More(9)",
        "Nale,Pinelli,Pontalti,Raffo(10)",
        "Bulavka,Fixman,Gutierrez,Levy(11)",
        "Bekier,Bokser,Borgna,Del Gobbo(12)",
        "Gambaccini,Penas,Pesaresi,Rissola(13)",
        "Bohe,Cadaval,Lew,Noli Villar(14)",
        "Bianchi,Jedwabny,Mena,Pondal(15)",
        "Cherny,Conde,Dorr,Salvador,Umfurer(16)"
        "Gonzalez,Luce,Maldonado,Pirich(17)"
    };
    assert(grupo >= 1 && grupo <= 17);
    return apellidos[grupo-1];
}

int main() {
    long long grupo, seed = 0;
    cout << "Ingrese su numero de grupo" << endl;
    cin >> grupo;
    while (grupo < 1 || grupo > 17) {
        cout << "Dale! Media pila, como van a ser el grupo " << grupo << " si los numeros de grupo van de 1 a 17?" << endl;
        cin >> grupo;
    }
    cout << "Ahora elijan una semilla" << endl;
    while(seed <= (1LL<<32) || seed >= (1LL<<56)) {
        cin >> seed;
        if(seed <= (1LL<<32)) {
            cout<< "lo siento, " << seed << "es muy chico, tiene que ser mayor a " << (1LL<<32) << endl;
        } else if(seed >= (1LL<<56)) {
            cout << "lo siento, " << seed << "es muy grande, tiene que ser menor a " << (1LL<<56) << endl;
        }
    }
    cout <<"Su numero de grupo para entregar es " << hashLeo(integrantes(grupo),seed) << " y no se olviden que lo generaron con la semilla " << seed <<", van a necesitar guardar este numero"<<  endl;
    return 0;
}
