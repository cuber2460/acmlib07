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

int add(int a, int b) { a += b; if(a >= mod) a -= mod; return a; }
void add_self(int & a, int b) { a = add(a, b); }
int sub(int a, int b) { a -= b; if(a < 0) a += mod; return a; }
void sub_self(int & a, int b) { a = sub(a, b); }
int mul(int a, int b) { return (ll) a * b % mod; }
void mul_self(int & a, int b) { a = mul(a, b); }

int my_pow(int a, int b) {
	int r = 1;
	while(b) {
		if(b % 2) mul_self(r, a);
		mul_self(a, a);
		b /= 2;
	}
	return r;
}
int my_inv(int a) { return my_pow(a, mod - 2); }
#define div div_compile
int div(int a, int b) { return mul(a, my_inv(b)); }

struct Polynomial {
	vector<int> w;
	int& operator [] (int i) { return w[i]; }
	int size() { return w.size(); }
	Polynomial operator +(Polynomial he) {
		vector<int> s(max(size(), he.size()));
		for(int i = 0; i < size(); ++i)
			s[i] = w[i];
		for(int i = 0; i < he.size(); ++i)
			add_self(s[i], he[i]);
		return Polynomial{s};
	}
	Polynomial operator -(Polynomial he) {
		vector<int> s(max(size(), he.size()));
		for(int i = 0; i < size(); ++i)
			s[i] = w[i];
		for(int i = 0; i < he.size(); ++i)
			sub_self(s[i], he[i]);
		return Polynomial{s};
	}
	void operator *= (int stala) {
		for(int & x : w) mul_self(x, stala);
	}
	void shift(int m) {
		for(int rep = 0; rep < m; ++rep) w.push_back(0);
		for(int i = size() - 1; i >= m; --i)
			w[i] = w[i-m];
		for(int i = 0; i < m; ++i)
			w[i] = 0;
	}
};

struct Massey {
	vector<int> start, coef;
	vector<vector<int>> powers;
	int L; // L == coef.size() <= start.size()
	int memo_inv;
	
	Massey(vector<int> in) { // O(input*L)
		L = 0;
		for(int x : in) assert(0 <= x && x < mod);
		const int N = in.size();
		Polynomial C{vector<int>{1}};
		Polynomial B = C;
		int m = 1;
		int b = 1;
		
		for(int n = 0; n < N; ++n) {
			int d = in[n];
			for(int i = 1; i <= L; ++i)
				add_self(d, mul(C[i], in[n-i]));
			if(d == 0) {
				++m;
				continue;
			}
			
			if(n >= N - 3) { // N-10 to be sure, N to guess
				cerr << "NO RELATION FOUND (maybe try a longer sequence?)\n";
				assert(false);
			}
			
			auto T = C;
			assert(b > 0);
				
			auto other = B;
			other.shift(m);
			other *= div(d, b);
			C = C - other;
			
			if(2 * L <= n) {
				L = n + 1 - L;
				B = T;
				b = d;
				m = 1;
			}
			else ++m;
		}
		for(int i = 1; i < C.size(); ++i) {
			coef.push_back(sub(0, C[i]));
			// the following line makes coefficients easier to read
			//~ if(coef.back() > mod / 2) coef.back() -= mod;
		}
		assert((int) coef.size() == L);
		// maybe: if(L == 0) { coef.push_back(0); ++L; } ???
		for(int i = 0; i < L; ++i)
			start.push_back(in[i]);
		while(!coef.empty() && !coef.back()) { coef.pop_back(); --L; }
		if(!coef.empty()) memo_inv = my_inv(coef.back());
		powers.push_back(coef);
		debug() << imie(L); // changed to cerr if you use -O2
		debug() << imie(coef);
	}
	
	vector<int> multiply_and_cut(vector<int> a, vector<int> b) {
		assert((int) a.size() == L && (int) b.size() == L);
		assert(L > 0);
		assert(coef.back() > 0);
		
		vector<int> r(2 * L - 1);
		
		for(int i = 0; i < L; ++i)
			for(int j = 0; j < L; ++j)
				add_self(r[i+j], mul(a[i], b[j]));
		
		while((int) r.size() > L) {
			int value = mul(r.back(), memo_inv); // div(r.back(), coef.back());
			const int X = r.size();
			add_self(r[X-L-1], value);
			for(int i = 0; i < L; ++i)
				sub_self(r[X-L+i], mul(value, coef[i]));
			assert(r.back() == 0);
			r.pop_back();
		}
		assert((int) r.size() == L);
		return r;
	}
	
	int get(ll k) { // O(L^2 * log(k))
		if(k < (int) start.size()) return start[k];
		if(L == 0) return 0;
		k -= start.size();
		vector<int> vec = coef;
		for(int i = 0; (1LL << i) <= k; ++i) {
			if(i == (int) powers.size())
				powers.push_back(multiply_and_cut(powers.back(), powers.back()));
			if(k & (1LL << i))
				vec = multiply_and_cut(vec, powers[i]);
		}
		//~ debug() << imie(vec);
		int total = 0;
		for(int i = 0; i < L; ++i)
			add_self(total, mul(vec[i], start[(int)start.size()-1-i]));
		return total;
	}
};

int main() {
	// f[n] = 2 * f[n-1] + f[n-2] ---> coef: [2, 1]
	vector<int> in{93, 0, 1, 2, 5, 12, 29, 70, 169};
	Massey massey(in);
	for(int i = 0; i < 15; ++i) printf("%d ", massey.get(i));
	puts(""); // 93 0 ... 169 408 985 2378 5741 13860 33461
}
