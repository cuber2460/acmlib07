#include <bits/stdc++.h>
using namespace std;
#define REP(i,n) for(int i = 0; i < int(n); ++i)
/*Precision error max_ans/1e15 (2.5e18) for (long) doubles.
So doubles are barely ok for sizes 2^20 and RANDOM positive
ints up to 45k, or for sizes 2^18 and ints close to 45k.
Those values assume DBL_MANT_DIG=53 and LDBL_MANT_DIG=64.
For input in [0, M], you can decrease everything by -M/2.
For many small vectors, use brute-force and step = 4.*/
typedef double ld; // 'long double' is 2.2 times slower
struct C {
	ld real, imag;
	C operator * (const C & he) const {
		return C{real * he.real - imag * he.imag,
				real * he.imag + imag * he.real};
	}
};
void dft(vector<C> & a, bool rev) {
	const int n = a.size();
	for(int i = 1, k = 0; i < n; ++i) {
		for(int bit = n / 2; !((k ^= bit) & bit); bit /= 2);
		if(i < k) swap(a[i], a[k]);
	}
	for(int len = 2; len <= n; len *= 2) {
		const ld ang = 4 * acosl(0) / len * (rev ? -1 : 1);
		const int step = 64; // step = 4 if many small vectors
		vector<C> baby(step); // step = 1 to check samples
		REP(i, step) baby[i] = C{cos(ang*i), sin(ang*i)};
		vector<C> giant;
		for(int i = 0; i < len / 2; i += step)
			giant.push_back(C{cos(ang*i), sin(ang*i)});
		for(int i = 0; i < n; i += len) {
			REP(k, len / 2) {
				const C x = a[i+k], y = a[i+k+len/2] * (k >= step ?
					giant[k/step] * baby[k%step] : baby[k%step]);
				a[i+k] = C{x.real + y.real, x.imag + y.imag};
				a[i+k+len/2] = C{x.real - y.real, x.imag - y.imag};
			}
		}
	}
	if(rev) REP(i, n) a[i].real /= n;
}
template<typename T>vector<T> multiply(const vector<T> & a, const vector<T> & b) {
	if(a.empty() || b.empty()) return {};
	int n = a.size() + b.size();
	vector<T> ans(n - 1);/*if(min(a.size(), b.size()) < 200){
	REP(i, a.size()) REP(j, b.size()) ans[i+j] += a[i]*b[j];
	return ans;} // (use brute-force for many small vectors)*/
	while(n&(n-1)) ++n;
	vector<C> one(n), two(n);
	REP(i, a.size()) one[i].real = a[i];
	REP(i, b.size()) two[i].real = b[i];
	dft(one, false);
	dft(two, false);
	REP(i, n) one[i] = one[i] * two[i];
	dft(one, true);
	REP(i, ans.size()) ans[i] = is_integral<T>::value ?
			llround(one[i].real) : one[i].real;
	return ans;		// for integers: error = abs(ans[i]-real)
} // slow_mul() is 7/3 times slower (3 vs 7 runs of DFT)
template<typename T>vector<T> slow_mul(const vector<T> & a, const vector<T> & b) {
	if(a.empty() || b.empty()) return {}; int n = a.size()
	+ b.size(); vector<T> ans(n - 1); while(n&(n-1)) ++n;
	// if(min(sizes) < 200) run brute force
	const T M = 150; // sqrt(max_absvalue)
	vector<C> t[2][2];  //two 10^6 vectors means that n = 2^21
	REP(x,2)REP(y,2) {  // so we get 2^24 numbers of 'ld' type
		t[x][y].resize(n);// (watch out for MLE)
		auto & in = x ? b : a;
		REP(i,in.size()) t[x][y][i].real= y ? in[i]/M : in[i]%M;
		dft(t[x][y], false);
	}
	REP(s, 3) {
		vector<C> prod(n);
		REP(x, 2) REP(y, 2) if(x + y == s) REP(i, n) {
			C he = t[0][x][i] * t[1][y][i];
			prod[i].real += he.real; prod[i].imag += he.imag;
		}
		dft(prod, true);
		REP(i, ans.size()) {
			T tmp = llround(prod[i].real); // error=abs(tmp-real)
			REP(_, s) tmp *= M;
			ans[i] += tmp;
		}
	}
	return ans;
}

typedef long long ll;
int main() {
	const int n = 500 * 1000;
	const int M = 25 * 1000;
	vector<ll> a, b;
	for(int i = 0; i < n; ++i) {
		a.push_back(rand() % M);
		b.push_back(rand() % M);
	}
	auto ans = multiply(a, b);
	for(int i = 0; i < min(10, (int) ans.size()); ++i)
		cout << ans[i] << " ";
	cout << "\n";
}
