//Hashowanie stringów za pomocą macierzy.
//Umożliwia stwierdzenie, czy dane słowa są równoważne cyklicznie, oraz czy są równe/równoważne
//Przy założeniu, że można gdziekolwiek dopisać/usunąć np. aaa, bb

//mod - 1 = 2^7 * 3^2 * 5 * 7 * 11 * 13 * 17 * 19
//Jeśli używamy, gen_with_order, to mod - 1 musi być podzielne przez rząd, który chcemy uzyskać
//W przeciwnym przypadku można po prostu 1e9 + 7
const int mod = 1862340481;
using ll = long long;
using matrix = array <array <ll, 3>, 3>;
const matrix ID = {1, 0, 0, 0, 1, 0, 0, 0, 1};
ll powe(ll x, int e) {
	ll ret = 1;
	while (e) {
		if (e & 1)
			ret = ret * x % mod;
		x = x * x % mod;
		e >>= 1;
	}
	return ret;
}
ll inv(ll x) {
	return powe(x, mod - 2);
}
int find_gen() {
	vector <int> factors;
	int x = mod - 1;
	for (int p = 2; p * p <= x; ++p) {
		if (x % p == 0) {
			factors.push_back((mod - 1) / p);
			while (x % p == 0)
				x /= p;
		}
	}
	if (x > 1)
		factors.push_back((mod - 1) / x);
	while (true) {
		int g = rand() % mod;
		if (g == 0)	continue;
		bool good = true;
		for (int x : factors)
			if (powe(g, x) == 1) {
				good = false;
				break;
			}
		if (good) return g;
	}
}
//Liczy wielomian charakterystyczny macierzy m.
//Jeśli słowa A, B są równoważne cyklicznie, to wielomiany charakterystyczne są równe
//Jeśli nie są, to prawdopodobieństwo, że są równe jest rzędu 1/(mod^2)
array <ll, 3> poly(const matrix &m) {
	array <ll, 3> ret = {};
	int c = 1;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) if (i != j) {
			int k = 3 ^ i ^ j;
			int s = (c++ & 2) - 1;
			for (int ii = 0; ii <= (i == 0); ++ii)
				for (int jj = 0; jj <= (j == 1); ++jj)
					for (int kk = 0; kk <= (k == 2) && ii + jj + kk < 3; ++kk) {
						ret[ii + jj + kk] += s * (ii ? 1 : m[0][i]) * (jj ? 1 : m[1][j]) % mod * (kk ? 1 : m[2][k]);
						ret[ii + jj + kk] %= mod;
					}
		}
	for (int i = 0; i < 3; ++i)
		if (ret[i] < 0)
			ret[i] += mod;
	return ret;
}
matrix operator* (const matrix &a, const matrix &b) {
	matrix c = {};
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; ++k) {
				c[i][j] += a[i][k] * b[k][j];
				c[i][j] %= mod; //mod^2 * 3 > zakres long longa
			}
	return c;
}
//Odwraca macierz
matrix operator! (const matrix &a) {
	matrix ret = {};
	ll det = 0;
	for (int i = 0; i < 3; ++i)
		det += (a[1][(i + 1) % 3] * a[2][(i + 2) % 3] - a[1][(i + 2) % 3] * a[2][(i + 1) % 3]) % mod * a[0][i];
	det %= mod;
	assert(det != 0); //Macierz jest osobliwa
	det = inv(det);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			//ret[j][i], nie ret[i][j]!!!!
			ret[j][i] = (a[(i + 1) % 3][(j + 1) % 3] * a[(i + 2) % 3][(j + 2) % 3] - a[(i + 2) % 3][(j + 1) % 3] * a[(i + 1) % 3][(j + 2) % 3]) % mod * det % mod;
			if (ret[j][i] < 0)
				ret[j][i] += mod;
		}
	return ret;
}
//Generuje macierz A taką, że A^order == I, a dla 0 < x < order A^x != I
matrix gen_with_order(int order) {
	assert((mod - 1) % order == 0);
	static int g = 61; //Jeśli używamy innego modulo, to g = find_mod();
	matrix ret = ID;
	ll G = powe(g, (mod - 1) / order);
	vector <ll> gens;
	if (order == 1)
		return ID;
	if (order == 2)
		gens = {1, -1, rand() % 2 * 2 - 1};
	else if (order == 3 || order == 4)
		gens = {G, powe(G, 2), powe(G, 3)};
	else if (order == 6)
		gens = {G, powe(G, 5), powe(G, rand() % 6)};
	else {
		vector <int> exps;
		while (exps.size() < 3u) {
			int a = rand() % order; //rand musi być duży
			bool good = __gcd(a, order) == 1;
			for (int i = 0; good && i < (int)exps.size(); ++i)
				if (a == exps[i])
					good = false;
			if (good)
				exps.push_back(a);
		}
		for (int x : exps)
			gens.push_back(powe(G, x));
	}
	for (int i = 0; i < 3; ++i)
		ret[i][i] = gens[i];
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (i != j) {
				int v = rand() % mod; //rand musi być duży
				for (int l = 0; l < 3; ++l)
					ret[j][l] = (ret[j][l] + v * ret[i][l]) % mod;
				for (int l = 0; l < 3; ++l)
					ret[l][i] = (ret[l][i] - v * ret[l][j]) % mod;
			}
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (ret[i][j] < 0)
				ret[i][j] += mod;
	return ret;
}
//Prawdopodobieństwo otrzymania osobliwej macierzy jest rzędu 1 / mod^2
matrix gen_random() {
	matrix ret;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			ret[i][j] = rand() % mod; //rand musi być duży
	return ret;
}
