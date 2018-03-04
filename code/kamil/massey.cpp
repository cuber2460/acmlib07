//~ https://en.wikipedia.org/wiki/Berlekamp%E2%80%93Massey_algorithm

#pragma GCC optimize ("Ofast")
#include <bits/stdc++.h>
using namespace std;
#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge {c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "

typedef long long ll;

const int mod = 1000 * 1000 * 1000 + 7;

int mul(int a, int b) { return (ll) a * b % mod; }
int my_pow(int a, int b) {
	int r = 1;
	while(b) {
		if(b % 2) r = mul(r, a);
		a = mul(a, a);
		b /= 2;
	}
	return r;
}
int my_inv(int a) { return my_pow(a, mod - 2); }

struct Massey {
	int L; // L == coef.size() <= start.size()
	Massey(vector<int> in) { // O(N^2)
		L = 0;
		const int N = in.size();
		vector<int> C{1}, B{1};
		for(int n = 0; n < N; ++n) {
			assert(0 <= in[n] && in[n] < mod); // invalid input
			B.insert(B.begin(), 0);
			int d = 0;
			for(int i = 0; i <= L; ++i)
				d = (d + (ll) C[i] * in[n-i]) % mod;
			if(d == 0) continue;
			vector<int> T = C;
			C.resize(max(B.size(), C.size()));
			for(int i = 0; i < (int) B.size(); ++i) {
				C[i] -= mul(d, B[i]);
				if(C[i] < 0) C[i] += mod;
			}
			if(2 * L <= n) {
				L = n + 1 - L;
				B = T;
				d = my_inv(d);
				for(int & x : B) x = mul(x, d);
			}
		}
		assert(2 * L <= N - 3); // NO RELATION FOUND :(
		cerr << "L = " << L << "\n";
	}
};

int add(int a, int b) { return (a + b) % mod; }

int main() {
	{
		// f[n] = 2 * f[n-1] + f[n-2] ---> coef: [2, 1]
		vector<int> in{0, 1, 1, 2, 3, 5};
		for(int i = in.size(); i < 30; ++i)
			in.push_back(add(in[i-1], add(in[i-2], in[i-6])));
		Massey massey(in);
		//~ for(int i = 0; i < 20; ++i) printf("%d ", massey.get(i));
		//~ puts(""); // 93 0 ... 169 408 985 2378 5741 13860 33461
	}
	{
		// f[n] = 2 * f[n-1] + f[n-2] ---> coef: [2, 1]
		vector<int> in{93, 0, 1, 2, 5, 12, 29, 70, 169};
		Massey massey(in);
		//~ for(int i = 0; i < 15; ++i) printf("%d ", massey.get(i));
		//~ puts(""); // 93 0 ... 169 408 985 2378 5741 13860 33461
	}
	{
		// f[n] = 2 * f[n-1] + f[n-2] ---> coef: [2, 1]
		vector<int> in{15, 17, 2, 20, 50, 10, 0, 1, 40};
		for(int i = in.size(); i < 30; ++i)
			in.push_back(add(mul(3, in[i-1]), add(mul(5, in[i-1]), add(mul(2, in[i-5]), mul(10, in[i-7])))));
		Massey massey(in);
		//~ for(int i = 0; i < 20; ++i) printf("%d ", massey.get(i));
		//~ puts(""); // 93 0 ... 169 408 985 2378 5741 13860 33461
	}
}
