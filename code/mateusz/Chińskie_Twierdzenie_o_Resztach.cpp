/* Chińskie twierdzenie o resztach O(n*(polylog+szybkość_faktoryzacji)).      */
/* Jeśli kongruencje są dowolne, to używamy crt().                            */
/* Jeśli wiemy, że modula są względnie pierwsze, to można użyć crt_coprime(). */
/* W crt() można podać inną funkcję do faktoryzacji, np. za pomocą sita.      */
/* Zarówno wynik, jak i argumenty są postaci x = first (mod second).          */

using ll = long long;
using pll = pair<ll, ll>;

// Ustawia takie @x i @y, że: ax + by = nwd(a, b).
void eukl(ll &x, ll &y, ll a, ll b) {
  if (!a) { x = 0; y = 1; return; }
  eukl(y, x, b % a, a);
  x -= y * (b / a);
}

// Zwraca taką liczbę, że: (odwr(a, b) * a) % b == 1.
ll odwr(ll a, ll b) {
  a %= b;
  ll x, y;
  eukl(x, y, a, b);
  return ((x % b) + b) % b;
}

// Zwraca ciąg liczb pierwszych (z powtórzeniami), których iloczyn jest równy v.
vector<ll> sqrt_factor(ll v) {
  vector<ll> ret;
  for (ll i = 2; i * i <= v; i++) {
    if (v % i == 0) {
      v /= i;
      ret.push_back(i);
      i--;
    }
  }
  if (v > 1) ret.push_back(v);
  return ret;
}

pll crt_coprime(const vector<pll>& wek) {  // x == first (mod second)
  ll ret = 0;
  ll mod = 1;
  for (int i = 0; i < (int) wek.size(); i++)
    mod *= wek[i].second;
  for (int i = 0; i < (int) wek.size(); i++) {
    ret += wek[i].first * odwr(mod / wek[i].second, wek[i].second)
                        * (mod / wek[i].second);
    ret %= mod;
  }
  return make_pair(ret, mod);
}

pll crt(const vector<pll>& wek) {  // x == first (mod second)
  vector<pair<ll, pll>> pom1;
  vector<pll> pom2;
  for (int i = 0; i < (int) wek.size(); i++) {
    const vector<ll>& roz =
        sqrt_factor(wek[i].second);  // Można zmienić na coś szybszego.
    ll il = 1;
    for (int j = 0; j < (int) roz.size(); j++) {
      il *= roz[j];
      if (j + 1 == (int) roz.size() || roz[j] != roz[j + 1]) {
        pom1.push_back(make_pair(roz[j], make_pair(il, wek[i].first % il)));
        il = 1;
      }
    }
  }
  sort(pom1.begin(), pom1.end());
  for (int i = 0; i < pom1.size(); i++) {
    if (i + 1 == (int) pom1.size() || pom1[i].first != pom1[i + 1].first) {
      pom2.push_back(pom1[i].second);
      swap(pom2.back().first, pom2.back().second);
    } else if ((pom1[i + 1].second.second % pom1[i].second.first) !=
                   pom1[i].second.second) {
      return make_pair(-1, -1);
    }
  }
  return crt_coprime(pom2);
}
