#ifndef __MATRIZ__
#define __MATRIZ__ 

#include <bits/stdc++.h>

using namespace std;

class Matriz{
	class Zp {
		int _n;
		int _p;
	public:
		Zp() {}
		Zp(int n) {
			_p = 10007;
			_n = ((n%_p)+_p)%_p;
		}
		Zp(const Zp &t) {
			_p = 10007;
			_n = ((t._n%_p)+_p)%_p;
		}
		int value() {
			return _n;
		}
		Zp operator+(Zp t) {
			return Zp(_n+t._n);
		}
		Zp operator*(Zp t) {
			return Zp(_n*t._n);
		}
		Zp operator=(Zp t) {
			_n = t._n;
			return (*this);
		}
		bool operator!=(Zp t) {
			return _n != t._n;
		}
		bool operator==(Zp t) {
			return _n == t._n;
		}
	};
	Zp m[3][3];
	friend istream& operator>>(istream& i, Matriz &mat);
	friend ostream& operator<<(ostream& o, Matriz mat);
	int val(int i, int j) {
		return m[i][j].value();
	}
public:
	Matriz() {
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				m[i][j] = 0;
			}
		}
	}
	Matriz(vector<int> vec) {
		assert(vec.size() == 9);
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				m[i][j] = vec[3*i+j];
			}
		}
	}
	Matriz(const Matriz &o) {
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				m[i][j] = o.m[i][j];
			}
		}
	}
	Matriz operator*(Matriz o) {
		Matriz r;
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				for(int t=0;t<3;t++) {
					r.m[i][j] = r.m[i][j] + (m[i][t] * o.m[t][j]);
				}
			}
		}
		return r;
	}
	void operator*=(Matriz o) {
		(*this) = (*this) * o;
	}
	Matriz operator=(Matriz o) {
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				m[i][j] = o.m[i][j];
			}
		}
		return (*this);
	}
	bool operator==(Matriz o) {
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				if(m[i][j] != o.m[i][j]) {
					return false;
				}
			}
		}
		return true;
	}
	bool operator!=(Matriz o) {
		return !((*this) == o);
	}
};

Matriz id() {
	vector<int> vec(9,0);
	for(int i=0;i<9;i+=4) {
		vec[i] = 1;
	}
	return Matriz(vec);
}

istream& operator>>(istream& i, Matriz &mat) {
	vector<int> vec(9);
	for(int j=0;j<9;j++) {
		i >> vec[j];
	}
	mat = Matriz(vec);
	return i;
}

ostream& operator<<(ostream& o, Matriz mat) {
	for(int i=0;i<3;i++) {
		o << mat.val(i,0) <<" "<< mat.val(i,1) << " "<< mat.val(i,2) << endl;
	}
	return o;
}

#endif
