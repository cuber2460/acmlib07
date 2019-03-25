using T = int;         // Typ, w którym mieści się modulo.
using T2 = long long;  // Typ, w którym mieści się kwadrat modulo.
// Dla pierwszego p > 2, potrafi szukać pierwiastków modulo p.
class Pierwiastek {  // Jeśli kod ma być używany dla stałego p, to warto p
 public:             // przekazać jako argument template'a, a nie konstruktora.
  Pierwiastek(T p) : s(0), p(p), m(p - 1) {
    assert(p > 2);
    while (m % 2 == 0) { m /= 2; s++; }
    c = 2;
    while (Pot(c, p / 2) == 1)
      c = rand() % (p - 1) + 1;  // Uwaga, rand() musi zwracać wartości rzędu p.
  }
  T Pot(T a, T pot) const {
		T r = 1;
		while (pot) {
			if (pot & 1) r = r * (T2) a % p;
			a = a * (T2) a % p;
			pot >>= 1;
		}
		return r;
	}  // Zwraca a**pot % p.
  T Licz(T a) const {  // Znajduje pierwiastek z a modulo p.
    if (a == 0) return 0;               // Sprawdza, czy a jest resztą
    if (Pot(a, p / 2) != 1) return -1;  // kwadratową. Jeśli nie, zwraca -1.
    T z = Pot(c, m);
    T v = Pot(a, m / 2);
    T u = (T2) v * v % p;
    v = (T2) v * a % p;
    u = (T2) u * a % p;
    for (int i = s - 1; i >= 1; i--) {
      if (Pot(u, T(1) << (i - 1)) != 1) {
        u = (T2) u * z % p;
        u = (T2) u * z % p;
        v = (T2) v * z % p;
      }
      z = (T2) z * z % p;
    }
    return v;  // Pierwiastkami liczby a są: {v, (p - v) % p}.
  }
 private:
  int s;
  T p, m, c;
};
template<class num_t>
inline num_t isqrt(num_t k) {
	num_t r = sqrt(k) + 1;
	while (r * r > k) r--;
	return r;
}
pii cornacchia(int p, int d) {//dla p pierwszego, znajduje a, b: p = a^2 + db^2
	int x0 = Pierwiastek(p).Licz(p - d);
	if (x0 == -1) return {-1, -1};
	x0 = max(x0, p - x0);
	int a = p, b = x0;
	int l = isqrt(p);
	while (b > l) {
		int r = a % b;
		a = b;
		b = r;
	}
	int c = p - b * b;
	if (c % d) return {-1, -1};
	c /= d;
	int cc = isqrt(c);
	if (cc * cc != c) return {-1, -1};
	return {b, cc};
}
