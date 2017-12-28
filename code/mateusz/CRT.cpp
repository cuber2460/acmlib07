/* Chińskie twierdzenie o resztach O(n*log(zakres)).                          */
/* Zarówno wynik, jak i argumenty są postaci x = first (mod second).          */
/* Jeśli kongruencja jest niespełnialna to zwraca (-1, -1).                   */

using ll = long long;
using pll = pair<ll, ll>;

void eukl(ll &x, ll &y, ll a, ll b) {
  if (!a) { x = 0; y = 1; return; }
  eukl(y, x, b % a, a);
  x -= y * (b / a);
}

ll mno(ll a, ll b, ll mod) {//a moze byc ujemne
  return (__int128(a)*b)%mod;
}

pll crt2(ll p, ll a, ll q, ll b) {
	if (a==-1)
		return {-1, -1};
	ll x, y;
	eukl(x, y, a, b);
	ll nwd=x*a+y*b;
	if ((p%nwd)!=(q%nwd))
		return {-1, -1};
	a/=nwd;
	b/=nwd;
	ll nww=a*b;
	ll ret=mno(x*a, q/nwd, nww)+mno(y*b, p/nwd, nww);
	if ((ret%=nww)<0)
		ret+=nww;
	return {ret*nwd+(p%nwd), nww*nwd};
}

pll crt(vector <pll> wek) {
	pll ret={0, 1};
	for (auto i : wek)
		ret=crt2(ret.first, ret.second, i.first, i.second);
	return ret;
}
