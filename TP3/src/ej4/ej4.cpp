#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#define clz __builtin_clz

constexpr int nearest_power(int n)
{
	assert(n > 0);
	return 1 << 8 * sizeof(int) - clz(n) - 1;
}

class Pair
{
	const int a, b;
	friend Pair operator+(const Pair &f, const Pair &s)
	{
		if (f.a > s.a)
			return Pair(f.a, std::max(f.b, s.a));

		return Pair(s.a, std::max(s.b, f.a));
	}

	public:
	int sum() const
	{
		return a + b;
	}

	explicit Pair()
	: Pair(0)
	{
	}

	explicit Pair(int n)
	: Pair(n, 0)
	{
	}

	Pair(int a, int b)
	: a(a), b(b)
	{
		assert (a >= b);
	}
};

template <typename T>
class SegmentTree
{
	const std::vector <T> tree;

	static std::vector<T> build_tree(const std::vector<T> &values)
	{
		assert(!values.empty());

		std::vector <T> tree(values.size() - nearest_power(values.size()));
		std::copy(values.rbegin(), values.rend(), std::back_inserter(tree));

		for (int k = 0; k < tree.size() - 1; k += 2)
			tree.push_back(tree[k] + tree[k + 1]);

		tree.push_back(T());
		return std::vector <T>(tree.rbegin(), tree.rend());
	}

	T accum_node(int n, int f, int t)
	{
		assert(!tree.empty());

		int h = 1 << clz(n) - clz(tree.size()) - 1;
		int l = (n - nearest_power(n)) * h;
		int r = (n - nearest_power(n) + 1) * h;

		if (f >= r || t <= l)
			return T();

		if (f <= l && t >= r)
			return tree[n];

		return accum_node(2 * n, f, t) + accum_node(2 * n + 1, f, t);
	}

	public:
	T accum(int f, int t)
	{
		return accum_node(1, f, t);
	}

	SegmentTree(const std::vector <T>& values)
	: tree(build_tree(values))
	{
	}
};

int main()
{
	int d, r;
	while (std::cin >> d >> r)
	{
		std::vector <int> E(d);
		for (auto &k : E)
			std::cin >> k;

		SegmentTree<Pair> st(std::vector<Pair>(E.begin(), E.end()));
		for (int i = 0; i < r; i++)
		{
			int p, u;
			std::cin >> p >> u;
			std::cout << st.accum(p, u).sum() << std::endl;
		}
	}

	return 0;
}
