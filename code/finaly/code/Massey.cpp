const int mod = 1000 * 1000 * 1000 + 7;
void add_self(int & a, int b) { a += b; if(a >= mod) a -= mod; }
void sub_self(int & a, int b) { a -= b; if(a < 0) a += mod; }
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
	vector<int> start, coef; // 3 optional lines
	vector<vector<int>> powers;
	int memo_inv;
	// Start here and write the next ~25 lines until "STOP"
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
				add_self(d, mul(C[i], in[n-i]));
			if(d == 0) continue;
			vector<int> T = C;
			C.resize(max(B.size(), C.size()));
			for(int i = 0; i < (int) B.size(); ++i)
				sub_self(C[i], mul(d, B[i]));
			if(2 * L <= n) {
				L = n + 1 - L;
				B = T;
				d = my_inv(d);
				for(int & x : B) x = mul(x, d);
			}
		}
		cerr << "L = " << L << "\n";
		assert(2 * L <= N - 2); // NO RELATION FOUND :(
		// === STOP ===
		for(int i = 1; i < (int) C.size(); ++i)
			coef.push_back((mod - C[i]) % mod);
		assert((int) coef.size() == L);
		for(int i = 0; i < L; ++i)
			start.push_back(in[i]);
		while(!coef.empty() && !coef.back()) { coef.pop_back(); --L; }
		if(!coef.empty()) memo_inv = my_inv(coef.back());
		powers.push_back(coef);
	}
	vector<int> mul_cut(vector<int> a, vector<int> b) {
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
		return r;
	}
	int get(ll k) { // O(L^2 * log(k))
		if(k < (int) start.size()) return start[k];
		if(L == 0) return 0;
		k -= start.size();
		vector<int> vec = coef;
		for(int i = 0; (1LL << i) <= k; ++i) {
			if(i == (int) powers.size())
				powers.push_back(mul_cut(powers.back(), powers.back()));
			if(k & (1LL << i))
				vec = mul_cut(vec, powers[i]);
		}
		int total = 0;
		for(int i = 0; i < L; ++i)
			add_self(total, mul(vec[i], start[(int)start.size()-1-i]));
		return total;
	}
};
