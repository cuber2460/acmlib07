#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef pair<ll, ull> int128;

int128 operator +(int128 a, int128 b) {
	return int128{a.first + b.first + (a.second + b.second < a.second),
			a.second + b.second};
}
int128 operator -(int128 a, int128 b) {
	return int128{a.first - b.first - (a.second < b.second),
			a.second - b.second};
}

int128 mul(ll one, ll two) { // tested, works for any arguments
	const int K = 32;
	ll a = one >> K; // [-2^31, 2^31)
	ull b = one - (a << K); // [0, 2^32)
	ll c = two >> K;
	ull d = two - (c << K);
	int128 ans{a * c, b * d};
	for(int rep = 0; rep < 2; ++rep) {
		ll x = rep ? a * d : b * c;
		ans.first += x >> K;
		x -= (x >> K) << K;
		assert(0 <= x && x < (1LL << K));
		ans = ans + int128{0, (ull) x << K}; // works even without casting
	}
	return ans;
}
int128 mul(ll one, ull two) { // tested, works for any arguments
	int128 ans = mul(one, ll(two / 2));
	ans = ans + ans;
	if(two % 2) ans = ans + int128{-(one < 0), (ull) one};
	return ans;
}
int128 mul(ull one, ll two) { return mul(two, one); }
int128 mul(ull one, ull two) { // tested, works if one * two < 2^127
	int128 ans = mul(one, ll(two / 2));
	ans = ans + ans;
	if(two % 2) ans = ans + int128{0, one};
	return ans;
}
int128 mul(int128 one, ll two) { // not tested
	int128 ans = mul(one.second, two);
	ans.first += one.first * two;
	return ans;
}
int128 mul(ll one, int128 two) { return mul(two, one); }
int128 mul(int128 one, int128 two) { // not tested
	if(!one.first) return mul(one.second, two);
	if(!two.first) return mul(one, two.second);
	if(two.first == -1) swap(one, two);
	if(one.first == -1) return int128{0,0} - mul(-one.second, two);
	assert(false); // both numbers outside of [-2^64, 2^64)
}

ll my_randLL() {
	int type = rand() % 4;
	if(type == 0) return numeric_limits<long long>::max() - rand() % 20;
	if(type == 1) return numeric_limits<long long>::min() + rand() % 20;
	if(type == 2) return -20 + rand() % 40;
	assert(type == 3);
	ll x = 0;
	for(int i = 0; i < 63; ++i)
		x = 2 * x + rand() % 2;
	if(rand() % 2) x *= -1;
	return x;
}

ull my_randULL() {
	int type = rand() % 3;
	if(type == 0) return numeric_limits<unsigned long long>::max() - rand() % 30;
	if(type == 1) return rand() % 30;
	assert(type == 2);
	ull x = 0;
	for(int i = 0; i < 64; ++i)
		x = 2 * x + rand() % 2;
	return x;
}

template<typename X, typename Y> void write(X a, Y b) {
	cout << "assert " << a << " * " << b << " == ";
	int128 p = mul(a, b);
	cout << p.first << " * 2**64 + " << p.second << "\n";
}

int main() {
	srand(5023);
	string ff = "py_random_file_asdf";
	assert(freopen(ff.c_str(), "w", stdout));
	for(int i = 0; i < 5000; ++i) {
		write(my_randLL(), my_randLL());
		write(my_randLL(), my_randULL());
		ull a = my_randULL();
		ull b = my_randULL();
		if((long double) a * (long double) b < powl(2, 127)) {
			//cerr << "yes, I'm testing unsigned\n";
			write(a, b);
		}
		//else cerr << "skipping unsigned, too big product\n";
	}
	fflush(stdout);
	system(("python " + ff).c_str());
}
