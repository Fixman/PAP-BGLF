#include <cassert>
#include <iostream>
#include <set>
#include <vector>

static_assert(
	sizeof(long) >= 8,
	"long tiene que ser de por lo menos 64 bit para que el codigo no haga overflow"
);

// Backtracker(D).backtrack() construye una lista de costos posibles para
// comprar rosquillas de costo D_0, ..., D_m.
// Llama a backtrackFrom 2 veces por cada llamada desde D_0 hasta D_m, asi
// que la complejidad del algoritmo es O(2^m).
struct Backtracker
{
	const std::vector <long> &D;
	std::set <long> R;

	// Hacer backtracking en D_t, ..., D_m, mientras que se asume que ya se
	// compraron c rosquillas.
	// Cuando se llega al final, agregar el resultado al conjunto R.
	void backtrackFrom(int t, long c)
	{
		if (t == D.size())
			R.insert(c);
		else
		{
			backtrackFrom(t + 1, c);
			backtrackFrom(t + 1, c + D[t]);
		}
	}

	// Hacer backtracking para encontrar
	std::set <long> &backtrack()
	{
		if (R.empty())
			backtrackFrom(0, 0);

		return R;
	}

	Backtracker(const std::vector <long> &D)
	: D(D)
	{
	}
};

// Devuelve la mayor cantidad de rosquillas que se pueden comprar si vienen
// en paquetes de D_0, ..., D_n, y se tienen p pesos.
// La complejidad total es O(2^(n / 2) + 2^(n / 2) * n) = O(2^(n / 2) * n).
long comprarRosquillas(std::vector <long> &D, long p)
{
	// Particionar D en dos partes con igual tamano.
	std::vector <long> A(D.begin(), D.begin() + D.size() / 2);
	std::vector <long> B(D.begin() + D.size() / 2, D.end());

	// Buscar la lista de costos posibles para comprar rosquillas tanto en la
	// mitad izquierda como en la mitad derecha.
	// Cada uno de esos tiene complejidad temporal O(2^(n / 2)).
	std::set <long> valuesLeft = Backtracker(A).backtrack();
	std::set <long> valuesRight = Backtracker(B).backtrack();

	// Se deberian poder gastar 0 pesos en la mitad izquierda, ya que siempre
	// se puede no comprar ninguna rosquilla. Si esto no fuese cierto,
	// *--valuesLeft.upper_bound(x) podria fallar con x < min(valuesLeft).
	assert(valuesLeft.find(0) != valuesLeft.end());

	// Por cada elemento de la particiin de la derecha, buscar el elemento de
	// la particion de la izquierda tal que comprando esas partes de las dos
	// mitades cueste menos que p, pero que igual se compren la mayor cantidad
	// de donas. Entre todos los elementos, elegir el que maximice la suma.
	// Como std::set::upper_bound es un arbol balanceado, la complejidad
	// de esto es O(log 2^(n / 2)) = O(n) por cada elemento de valuesRight,
	// asi que la complejidad total es O(2^(n / 2) * n).
	long best = 0;
	for (long r : valuesRight)
	{
		if (r <= p)
			best = std::max(best, r + *--valuesLeft.upper_bound(p - r));
	}

	return best;
}

int main()
{
	long p;
	int n;
	while (std::cin >> p >> n)
	{
		std::vector <long> D(n);
		for (long &k : D)
			std::cin >> k;

		std::cout << comprarRosquillas(D, p) << std::endl;
	}

	return 0;
}
