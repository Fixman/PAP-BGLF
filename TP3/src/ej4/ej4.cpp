#include <algorithm>
#include <iostream>
#include <vector>

#define clz __builtin_clz

class Pair
{
	const int a, b;
	friend Pair operator+(const Pair &, const Pair &);

	public:
	int sum() const
	{
		return a + b;
	}

	explicit Pair(int n)
	: a(n), b(b)
	{
	}

	Pair(int a, int b)
	: a(a), b(b)
	{
	}
};

Pair operator+(const Pair &f, const Pair &s)
{
	if (f.a > s.a)
		return Pair(f.a, std::max(f.b, s.a));
	
	return Pair(s.a, std::max(s.b, f.a));
}

template <typename T>
class SegmentTree
{
	const std::vector <T> tree;

	std::vector<T> built_tree(const std::vector<T> &values)
	{
		std::vector <T> tree(values.rbegin(), values.rend());

		for (int k = 0; k < tree.size(); k += 2)
			tree.push_back(tree[k] + tree[k + 1]);

		return std::vector <T> (tree.rbegin(), tree.rend());
	}

	public:
	SegmentTree(const std::vector <T>& values)
	: tree(built_tree(values))
	{
	}

	friend int main();
};

int main()
{
	std::vector <Pair> f{Pair(0), Pair(1), Pair(2), Pair(3), Pair(4), Pair(5)};
	SegmentTree<Pair> s(f);

	std::for_each(s.tree.begin(), s.tree.end(), [] (Pair a) { std::cout << a.sum() << std::endl; });

	return 0;
}
