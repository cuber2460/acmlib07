#include <bits/stdc++.h>
using namespace std;
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
		ld kat = 4 * acosl(0) / len * (rev ? -1 : 1);
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
template<typename T> vector<T> multiply(const vector<T> & a, const vector<T> & b) {
	int n = a.size() + b.size();
	while (n&(n-1)) ++n;
	vector<C> one(n), two(n);
	for(int i = 0; i < (int) a.size(); ++i) one[i].real = a[i];
	for(int i = 0; i < (int) b.size(); ++i) two[i].real = b[i];
	dft(one, false);
	dft(two, false);
	for(int i = 0; i < n; ++i) one[i] = one[i] * two[i];
	dft(one, true);
	vector<T> ans(int(a.size() + b.size()) - 1);
	for(int i = 0; i < (int) ans.size(); ++i)
		ans[i] = one[i].real + 0.5;
	return ans;
}

int main() {
	const int n = 500 * 1000, M = 20;
	vector<int> a, b;
	for(int i = 0; i < n; ++i) {
		a.push_back(rand() % M);
		b.push_back(rand() % M);
	}
	auto ans = multiply(a, b);
	for(int i = 0; i < min(10, (int) ans.size()); ++i)
		cout << ans[i] << " ";
	cout << "\n";
}
