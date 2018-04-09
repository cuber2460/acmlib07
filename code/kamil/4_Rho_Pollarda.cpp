//~ vector<ll> witness = {2, 7, 61}; // < 4759123141 = 4e9
vector<ll> witness = {2, 325, 9375, 28178, 450775, 9780504, 1795265022}; // < 2^64

ll mul(ll a, ll b, ll mod) {
	return (__int128) a * b % mod;
}
ll my_pow(ll a, ll b, ll mod) {
	ll res = 1;
	while(b) {
		if(b % 2) res = mul(res, a, mod);
		a = mul(a, a, mod);
		b /= 2;
	}
	return res;
}
bool test(ll n) {
	if(n == 2) return true;
	if(n < 2 || n % 2 == 0) return false;
	ll d = n - 1, s = 0;
	while(d % 2 == 0) {
		d /= 2;
		++s;
	}
	for(auto i : witness) if(i % n) {
		ll x = my_pow(i, d, n);
		if(x == 1) continue;
		bool zlozona = true;
		for(int j = 0; j < s; ++j) {
			if(x == n - 1) {
				zlozona = false;
				break;
			}
			x = mul(x, x, n);
		}
		if(zlozona) return false;
	}
	return true;
}
ll f(ll x, ll mod, ll c) {
	ll y = mul(x, x, mod) + c;
	if(y > mod) y -= mod;
	return y;
}
void rho(ll n, vector<ll> & w) {
	if(n <= 1) return;
	if(test(n)) {
		w.push_back(n);
		return;
	}
	for(ll c = 1; true; ++c) {
		ll x = 2, y = 2, d = 1;
		while(d == 1) {
			x = f(x, n, c);
			y = f( f(y,n,c), n, c);
			d = __gcd(abs(x - y), n);
		}
		if(d < n) {
			rho(d, w);
			rho(n / d, w);
			return;
		}
	}
}
vector<ll> rozklad(ll n) {
	vector<ll> w;
	for(int i = 2; i <= 100; ++i) while(n % i == 0) {
		n /= i;
		w.push_back(i);
	}
	rho(n, w);
	sort(w.begin(), w.end());
	return w;
}
