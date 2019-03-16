vector<ll> witness = {2, 325, 9375, 28178, 450775, 9780504, 1795265022}; // < 2^64
//vector<ll> witness = {2, 7, 61}; // < 4759123141
ll mnoz(ll a, ll b, ll mod) {return (__int128(a)*b)%mod;}
ll pot(ll b, ll e, ll mod) {/*...*/}
bool test(ll n) {
  if (n == 2)
    return true;
  if (n < 2 || n%2 == 0)
    return false;
  ll d = n-1;
  ll s = 0;
  while (d%2 == 0) {
    d /= 2;
    ++s;
  }
  for (auto i: witness) if (i%n) {
    ll x = pot(i,d,n);
    if (x != 1) {
      bool zlozona = true;
      for (int j = 0; j < s; ++j) {
        if (x == n-1) {
          zlozona = false;
          break;
        }
        x = mnoz(x, x, n);
      }
      if (zlozona)
        return false;
    }
  }
  return true;
}
ll f(ll x, ll mod, ll c) {return mnoz(x,x,mod) + c;}
void rho(ll n, vector<ll> &v) {
  if (n <= 1) return;
  if (test(n)) {
    v.push_back(n);
    return;
  }
	for (ll c = 1;;++c) {
    ll x = 2, y = 2, d = 1;
    while (d == 1) {
      x = f(x,n,c);
      y = f(f(y,n,c),n,c);
      d = __gcd(abs(x-y),n);
    }
    if (d < n) {
      rho(d, v);
      rho(n/d,v);
      return;
    }
  }
}
void rozklad(ll n, vector<ll> &v) {
  int BLOK = 100;
  for (int i = 2; i < BLOK; ++i) while (n%i == 0) {
    n /= i;
    v.push_back(i);
  }
  rho(n,v);
  sort(v.begin(),v.end());
}
