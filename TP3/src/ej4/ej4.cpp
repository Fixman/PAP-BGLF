#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

// Count Leading Zeroes. Devuelve la posición del primer 1 en la
// representación binaria de un número.
#define clz __builtin_clzl

// Devuelve la mayor potencia de 2 que sea menor o igual a n.
constexpr long nearest_power(long n)
{
	assert(n > 0);
	return 1ull << 8 * sizeof(long) - clz(n) - 1;
}

// Representa un par de números con operaciones respectivas a un monoide con
// identidad [0, 0] (constructor nulario), y una operación asociativa binaria
// a • b que devuelve el mayor y segundo mayor elemento de a y b (operador +).
class Pair
{
	// Precondición: a ≥ b
	const long a, b;

	// Crear un Pair con dos elementos. Esto es necesario cuando se combinan
	// dos Pair.
	Pair(long a, long b)
	: a(a), b(b)
	{
		assert (a >= b);
	}

	public:
	// Devolver un Pair con los dos máximos entre los elementos
	// de f y los elementos de s.
	// Al igual que la suma y el máximo entre múltiples elementos,
	// esta operación es asociativa.
	friend Pair operator+(const Pair &f, const Pair &s)
	{
		// Si f tiene el mayor elemento, entonces este es el mayor de la
		// combinación de los dos, y el segundo mayor es el mayor entre
		// el segundo mayor de f, y el mayor de s.
		if (f.a > s.a)
			return Pair(f.a, std::max(f.b, s.a));

		// Sino, pasa lo contrario.
		return Pair(s.a, std::max(s.b, f.a));
	}

	// Devolver la suma de a y b. Esto representa la diversión
	// compuesta en un rango.
	long sum() const
	{
		return a + b;
	}

	// Identidad: ∀ x ∈ Pair. x + Pair() = x
	// Esto es cierto en el contexto del problema porque todas las diversiones
	// son no-negativas.
	explicit Pair()
	: Pair(0)
	{
	}

	// Crear un Pair con un solo elemento. Esto representa los días individuales.
	explicit Pair(long n)
	: Pair(n, 0)
	{
	}
};

// Segment tree para un monoide T.
// Este árbol es constante, y el árbol no se puede modificar una vez que
// es construido con una lista de T. Permite buscar, para un par [f, t), el
// elemento resultante de aplicar x_f • ... • x_(t - 1).
template <typename T>
class SegmentTree
{
	// El árbol binario se representa con un vector, donde la raíz es el
	// elemento 1, y por cada elemento no-hoja x, sus hijos son (2 * x) y (2 *
	// x + 1). Cada hoja contiene un valor de la entrada o una identidad, y
	// todos los otros elementos contienen el resultado de acumular (fold) con
	// • a todos sus hijos.
	// Por simplicidad, el árbol siempre debe estar completo y por ende tener
	// 2^n - 1 elementos totales para algún entero n.
	const std::vector <T> tree;

	// Dado una lista de valores, devuelve una lista representando un
	// segment tree donde las hojas son los elementos.
	// Como el árbol debe tener 2^m hojas para algún m, el resto de las hojas
	// se rellena con la identidad de T.
	// Complejidad temporal: O(D).
	static std::vector<T> build_tree(const std::vector<T> &values)
	{
		assert(!values.empty());

		// El árbol se arma de atras para adelante: primero los elementos
		// identidad, luego los elementos de entrada en orden reverso, y luego
		// la combinación de cada par de elementos del árbol.
		// De esta manera nos podemos aprovechar de que agregar un elemento al
		// final de un std::vector es mucho más rápido que agregarlo al inicio sin
		// tener que crearlo con un montón de elementos vacíos, lo que sería
		// propenso a errores.
		std::vector <T> tree(values.size() - nearest_power(values.size()));
		std::copy(values.rbegin(), values.rend(), std::back_inserter(tree));

		for (int k = 0; k < tree.size() - 1; k += 2)
			tree.push_back(tree[k] + tree[k + 1]);

		// El último elemento representa el elemento 0, que no se usa ya que
		// el número 0 no tiene lindas propiedades con la duplicación.
		tree.push_back(T());

		// Devolver el reverso del árbol que se acaba de crear.
		return std::vector <T>(tree.rbegin(), tree.rend());
	}

	// Acumular, recursivamente, el subconjunto más grande posible de elementos
	// en el rango [f, t) que se encuentren bajo el nodo n.
	// Complejidad temporal: O(h) = O(log D).
	T accum_node(int n, int f, int t)
	{
		assert(!tree.empty());

		// Aprovechando las propiedades de los árboles binarios, se puede calcular
		// la altura a la que se encuentra este nodo y su rango solamente con
		// su número, sin guardar nada en memoria y en tiempo O(1).
		unsigned int h = 1ull << clz(n) - clz(tree.size()) - 1;
		int l = (n - nearest_power(n)) * h;
		int r = (n - nearest_power(n) + 1) * h;

		// Si el rango buscado es disjunto al representado por este nodo,
		// devolver la identidad.
		if (f >= r || t <= l)
			return T();

		// Si el rango buscado es un superconjunto del representado por este
		// nodo, devolver su valor.
		if (f <= l && t >= r)
			return tree[n];

		// Sino, hay partes del rango buscado que están dentro de este rango,
		// pero hay partes de este rango que no están en el rango buscado.
		// En ese caso, aplicar la misma operación a los dos hijos de este nodo.
		// Esta linea puede hacer parecer que la complejidad total es O(2^h) = O(d),
		// sin embargo está probado en el informe que solo se llega a esta parte en
		// casos muy especificos.
		return accum_node(2 * n, f, t) + accum_node(2 * n + 1, f, t);
	}

	public:
	// Acumular los valores de los elementos correspondientes al rango [f, t)
	// en todo el árbol.
	// Complejidad temporal: O(h) = O(log D).
	T accum(int f, int t)
	{
		return accum_node(1, f, t);
	}

	// Crear un Segment Tree.
	// Complejidad temporal: O(D).
	SegmentTree(const std::vector <T>& values)
	: tree(build_tree(values))
	{
	}
};

int main()
{
	int d, r;

	// Tomar múltiples (posiblemente 1) casos de prueba.
	// Complejidad temporal: O(D + R log D).
	while (std::cin >> d >> r)
	{
		std::vector <long> E(d);
		for (auto &k : E)
			std::cin >> k;

		// Crear el Segment Tree con los datos de entrada.
		// Complejidad temporal: O(D).
		SegmentTree<Pair> st(std::vector<Pair>(E.begin(), E.end()));

		// Por cada rango del departamento, imprimir la suma de los mayores dos
		// elementos de él.
		// Complejidad temporal: O(R log D).
		for (int i = 0; i < r; i++)
		{
			int p, u;
			std::cin >> p >> u;
			std::cout << st.accum(p, u).sum() << std::endl;
		}
	}

	return 0;
}
