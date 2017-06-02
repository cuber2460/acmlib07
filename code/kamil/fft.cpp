#include <bits/stdc++.h>
using namespace std;
#define REP(i,n) for(int i = 0; i < int(n); ++i)
typedef double ld; // or 'long double' for better precision
struct C {
	ld real, imag;
	C operator *(const C & he) const {
		return C{real * he.real - imag * he.imag,
				real * he.imag + imag * he.real};
	}
};
void dft(vector<C> & a, bool rev) {
	const int n = a.size();
	for(int i = 1, j = 0; i < n; ++i) {
		for(int bit = n / 2; !((j ^= bit) & bit); bit /= 2);
		if(i < j) swap(a[i], a[j]);
	}
	for(int len = 2; len <= n; len *= 2) {
		ld kat = 4 * acos(0) / len * (rev ? -1 : 1);
		C primitive{cos(kat), sin(kat)};
		for(int i = 0; i < n; i += len) {
			C e{1, 0};
			for(int j = 0; j < len / 2; ++j) {
				if(j && j % 16 == 0) e = C{cos(j*kat), sin(j*kat)};
				const C x = a[i+j], y = a[i+j+len/2] * e;
				a[i+j+len/2] = C{x.real - y.real, x.imag - y.imag};
				a[i+j] = C{x.real + y.real, x.imag + y.imag};
				e = e * primitive;
			}
		}
	}
	if(rev) for(int i = 0; i < n; ++i) a[i].real /= n;
}
template<typename T> vector<T> multiply(const vector<T> & a,
																			const vector<T> & b) {
	if(a.empty() || b.empty()) return {};
	int n = a.size() + b.size();
	while(n&(n-1)) ++n;
	vector<C> one(n), two(n);
	REP(i, a.size()) one[i].real = a[i];
	REP(i, b.size()) two[i].real = b[i];
	dft(one, false);
	dft(two, false);
	REP(i, n) one[i] = one[i] * two[i];
	dft(one, true);
	vector<T> ans(int(a.size() + b.size()) - 1);
	//~ ld err = 0; REP(i,ans.size())err=max(err,
	// error = MAX of: abs(one[i].real - llround(one[i].real))
	REP(i, ans.size()) ans[i] = is_integral<T>::value ?
			llround(one[i].real) : one[i].real;
	return ans;
}
typedef long long ll; // slow_mul takes non-negative numbers
vector<ll> slow_mul(const vector<ll>&a, const vector<ll>&b){
	const ll M = 30; // should be close to sqrt(max_value)
	if(a.empty() || b.empty()) return {};
	int n = a.size() + b.size();
	while(n&(n-1)) ++n;
	vector<vector<C>> four(4, vector<C>(n));
	REP(i, a.size()) {
		four[0][i].real = a[i] % M;
		four[1][i].real = a[i] / M;
	}
	REP(i, b.size()) {
		four[2][i].real = b[i] % M;
		four[3][i].real = b[i] / M;
	}
	for(auto & vec : four) dft(vec, false);
	vector<ll> ans(int(a.size() + b.size()) - 1);
	for(int one : {0, 1}) for(int two : {2, 3}) {
		if(one == 1 && two == 2) continue;
		vector<C> they(n);
		REP(i, n) they[i] = four[one][i] * four[two][i];
		if(one == 0 && two == 3) REP(i, n) {
			C he = four[1][i] * four[2][i];
			they[i].real += he.real; they[i].imag += he.imag;
		}
		dft(they, true);
		REP(i, ans.size()) {
			ll tmp = llround(they[i].real);
			REP(rep, one + two - 2) tmp *= M;
			ans[i] += tmp;
		}
	}
	return ans;
}

int main() {
	const int n = 500 * 1000, M = 100;
	vector<ll> a, b;
	for(int i = 0; i < n; ++i) {
		a.push_back(rand() % M);
		b.push_back(rand() % M);
	}
	auto ans = slow_mul(a, b);
	for(int i = 0; i < min(10, (int) ans.size()); ++i)
		cout << ans[i] << " ";
	cout << "\n";
}
