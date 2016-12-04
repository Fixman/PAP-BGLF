#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>

// Un AccumulatingStack es similar a una pila el sentido de que se pushean
// elementos al tope de la pila y se popean de esta, pero tiene diferencias
// importantes.
// Cada elememento de un AccumulatingStack es un entero positivo, y cuando
// se popea solo se elimina de la pila cuando es igual a 0; en otro caso
// simplemente se decrementa.
// También se guarda la suma de todos los elementos.
class AccumulatingStack
{
	std::stack <int> accum;
	int sigma;

	public:

	// Chequear si la pila está vacía.
	// Complejidad: O(1).
	bool empty() const
	{
		return accum.empty();
	}

	// Popear un elemento, y devolver el tope.
	// Complejidad: O(1).
	int pop()
	{
		int a = accum.top();
		accum.pop();

		sigma -= 1;
		if (a - 1 > 0)
			accum.push(a - 1);

		return a;
	}

	// Pushear un elemento.
	// Complejidad: O(1).
	void push(int a)
	{
		accum.push(a);
		sigma += a;
	}

	// Devolver la suma de todos los elementos actualmente en la pila.
	// Complejidad: O(1).
	int sum() const
	{
		return sigma;
	}

	// Construir un AccumulatingStack con un vector V de valores.
	// Los valores simplemente se pushean de atrás para adelante (de
	// manera de que el tope de la pila sea el primer elemento del vector).
	// Complejidad: O(|V|)
	explicit AccumulatingStack(const std::vector <int> &values)
	: sigma(0)
	{
		for (auto k = values.rbegin(); k != values.rend(); k++)
			push(*k);
	}
};

// Agrupa un vector de elementos, y devuelve otro vector con la cantidad
// de veces que aparece cada elemento, ordenado en orden lexicográfico.
// Complejidad: O(|V| log |V|)
std::vector <int> group(std::vector <int> values)
{
	std::map <int, int> count;
	for (auto k : values)
		count[k] += 1;

	std::vector <int> grouped;
	for (auto k : count)
		grouped.push_back(k.second);

	return grouped;
}

// Devuelve la esperanza de la cantidad de permutaciones necesarias para
// terminar con todo el arreglo, dado como una pila acumulada.
// Notar que esto no asume que se va a permutar el arreglo al inicio; empieza
// desde el paso 3 del problema.
// Complejidad: O(N)
double shufflingOdds(AccumulatingStack &p)
{
	if (p.empty())
		return 0;

	// s / a - 1 = (s - a) / a
	double s = p.sum();
	double a = p.pop();
	return s / a - 1 + shufflingOdds(p);
}

// Dado un arreglo de valores, devuelve la esperanza de la cantidad de
// permutaciones que hay que hacer hasta que termine el algoritmo descripto
// en el TP.
// Complejidad: O(N log N + N) = O(N log N)
double expectedValue(const std::vector <int> &values)
{
	AccumulatingStack accum(group(values));
	return 1 + shufflingOdds(accum);
}

int main()
{
	int n;
	while (std::cin >> n)
	{
		std::vector <int> A(n);
		for (auto &k : A)
			std::cin >> k;

		std::printf("%.6f\n", expectedValue(A));
	}

	return 0;
}
