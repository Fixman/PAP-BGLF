#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>

class AccumulatingStack
{
	std::stack <int> accum;
	int sigma;

	public:
	bool empty()
	{
		return accum.empty();
	}

	int pop()
	{
		int a = accum.top();
		accum.pop();

		sigma -= 1;
		if (a - 1 > 0)
			accum.push(a - 1);

		return a;
	}

	int sum()
	{
		return sigma;
	}

	explicit AccumulatingStack(const std::vector <int> &values)
	: sigma(0)
	{
		for (auto k = values.rbegin(); k != values.rend(); k++)
		{
			accum.push(*k);
			sigma += *k;
		}
	}
};

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

double shufflingOdds(AccumulatingStack &p)
{
	if (p.empty())
		return 0;

	double s = p.sum();
	double a = p.pop();
	return s / a - 1 + shufflingOdds(p);
}

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
