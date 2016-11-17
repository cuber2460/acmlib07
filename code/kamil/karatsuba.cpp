#include <bits/stdc++.h>
using namespace std;
typedef unsigned int T;
vector<T> brute_mul(const vector<T> & a, const vector<T> & b) {
	if(a.empty() || b.empty()) return vector<T>{};
	vector<T> r(a.size() + b.size() - 1, 0);
	for(int i = 0; i < (int) a.size(); ++i)
		for(int j = 0; j < (int) b.size(); ++j)
			r[i+j] += a[i] * b[j];
	return r;
}
vector<T> karatsuba(vector<T> a, vector<T> b) {
	if(min(a.size(), b.size()) <= 150) return brute_mul(a, b);
	int half = (max(a.size(), b.size()) + 1) / 2;
	vector<T> a2(max(0, (int) a.size() - half)), b2(max(0, (int) b.size() - half));
	for(int i = half; i < (int) a.size(); ++i)
		a2[i - half] = a[i];
	a.resize(min(half, (int) a.size()));
	for(int i = half; i < (int) b.size(); ++i)
		b2[i - half] = b[i];
	b.resize(min(half, (int) b.size()));
	vector<T> z0 = karatsuba(a, b);
	assert(a.size() >= a2.size());
	assert(b.size() >= b2.size());
	for(int i = 0; i < (int) a2.size(); ++i)
		a[i] += a2[i];
	for(int i = 0; i < (int) b2.size(); ++i)
		b[i] += b2[i];
	vector<T> z1 = karatsuba(a, b);
	vector<T> z2 = karatsuba(a2, b2);
	vector<T> r(max(z2.size() + 2 * half, z1.size() + half), 0);
	for(int i = 0; i < (int) z2.size(); ++i) {
		r[i + half] -= z2[i];
		r[i + 2 * half] += z2[i];
	}
	for(int i = 0; i < (int) z1.size(); ++i)
		r[i + half] += z1[i];
	for(int i = 0; i < (int) z0.size(); ++i) {
		r[i] += z0[i];
		r[i + half] -= z0[i];
	}
	while(!r.empty() && r.back() == 0) r.pop_back();
	return r;
}

int main() {
	int n = 100 * 1000, m = 100 * 1000;
	vector<T> a, b;
	for(int i = 0; i < n; ++i)
		a.push_back(rand() % 100);
	for(int i = 0; i < m; ++i)
		b.push_back(rand() % 100);
	//brute_mul(a, b);
	karatsuba(a, b);
}
