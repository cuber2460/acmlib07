//Wypisywanie debugowe iterujące po przedziale
#ifndef LOCAL
#pragma GCC optimize("O3")
#endif
#include <bits/stdc++.h>
using namespace std;
#define sim template <class c
#define mor >muu & operator<<(
#define ris return *this
#define R22(r) sim> typename enable_if<1 r sizeof(dud<c>(0)), muu&>::type operator<<(c g) {
sim> struct rge {c b, e;};
sim> rge<c> range(c i, c j) {return rge<c>{i, j};}
sim> auto dud(c *r) -> decltype(cerr << *r);
sim> char dud(...);
sim, class F> struct zub {c b, e; F f;}; //dopisać to
sim, class F> zub<c, F> range_m(c i, c j, F f) {return zub<c, F>{i, j, f};} //i to
sim, class F> zub<typename c::const_iterator, F> range_m(const c& i, F f) {return range_m(begin(i), end(i), f);} //Ewentualnie to, jak chcemy przekazywać cały kontener
struct muu {
	#ifdef LOCAL
	stringstream a;
	~muu() {cerr << a.str() << endl;}
	R22(<) a << boolalpha << g; ris;}
	R22(==) ris << range(begin(g), end(g));}
	sim mor rge<c> u) {
		a << "[";
		for (c i = u.b; i != u.e; ++i)
			*this << ", " + 2 * (i == u.b) << *i;
		ris << "]";
	}
	sim, class F mor zub<c, F> u) { //i to
		a << "[";
		for (c i = u.b; i != u.e; ++i)
			u.f(*this << ", " + 2 * (i == u.b), *i);
		ris << "]";
	}
	sim, class b mor pair<b,c> u) {ris << "(" << u.first << ", " << u.second << ")";}
	#else
	sim mor const c&) {ris;}
	#endif
	muu & operator()(){ris;}
};
#define debug (muu() << __FUNCTION__ << "#" << __LINE__ << ": ")
#define imie(r) "[" #r ": " << (r) << "] "
#define imask(r) "[" #r ": " << bitset<8 * sizeof(r)>(r) << "] "
#define arr(a, i) "[" #a imie(i) ": " << a[i] << "] "
#define arr2(a, i, j) "[" #a imie(i) imie(j) ": " << a[i][j] << "] "

void main1() {
	//W trybie non-debug żadna z labmd się nie wykona
	vector <int> a = {0, 2, 4, 6, 8, 1, 3, 5, 7, 9};
	vector <pair <int, int> > vec = {{1, 3}, {3, 4}, {0, 6}, {8, 9}, {5, 10}, {6, 6}};
	debug << "vec : " << range_m(vec, [&](muu &deb, pair <int, int> x){deb << range(a.begin() + x.first, a.begin() + x.second);});
	debug << range_m(a.begin() + 3, a.begin() + 6, [&](muu &deb, int v){deb << imask(v);});
}

//Operator debug dla własnego structa
struct str {
	int a, b, c;
};
#ifdef LOCAL
muu & operator<<(muu &deb, str x) {
	return deb << "[" << x.a << " * x + " << x.b << " * y = " << x.c << "]";
}
#endif

//Operatory na parach: (a, b) + c = (a + c, b + c), (a, b) + (c, d) = (a + c, b + d), a + (b, c) = (a + b, a + c)
//(a, b) += (c, d) -> (a + c, b + d) (a, b) += c -> (a + c, b + c)
//Automatycznie przenosi się na operatory na większych krotkach, np (a, (b, c)) * (e, (f, g)) = (a * e, (b, c) * (f, g)) = (a * e, (b * f, c * g))
//((a, b), c) % (d, (e, f)) = ((a, b) % d, c % (e, f)) = ((a % d, b % d), (c % e, c % f))
//Typ zwracanej pary jest wybierany na podstawie typów element operator element. np.
// (int, long long) + (long long, unsigned int) = (long long, unsigned long long)
// (bitset, int) >> (int, long long) = (bitset, long long)
// (string, int) + (char *, double) = (string, double)
#define f first
#define s second
#define vsv sim, class d, class e
#define nop(o,c,r,l...) auto operator o(c a, r b)-> decltype(make_pair(l)) {return {l};}
//enable_if jest potrzebne tylko, jeśli chcemy mieć operator <<, w przeciwnym wypadku można po prostu: vsv> nop ...
#define pcg(o) \
/*para + para*/ vsv, class f> nop(o, pair <c u d>, pair <e u f>, a.f o b.f, a.s o b.s) \
/*liczba + para*/ vsv,class = typename enable_if<!is_same<c, muu>::value>::type> nop(o, c, pair<d u e>, a o b.f, a o b.s) \
/*para + liczba*/ vsv> nop(o, pair<c u d>, e, a.f o b, a.s o b)
#define clp(o) pcg(o) \
/*para += liczba*/ vsv> void operator o##= (pair <c,d> & a, e b) {a.f o##= b; a.s o##= b;} \
/*liczba += para*/ vsv, class f> void operator o##= (pair <c,d> & a, pair <e,f> b) {a.f o##= b.f; a.s o##= b.s;}
#define syd(o) sim, class d> auto operator o(pair<c, d> e) -> decltype(make_pair(o e.f, o e.s)) {return {o e.f, o e.s};}
#define u ,
//clp: razem z odpowiednim operatorem przypisania, pcg: bez niego, syd: operator jednoargumentowy
clp(+) clp(-) clp(*) clp(/) clp(%) clp(^) clp(|) clp(>>) clp(<<) clp(&) pcg(&&) pcg(||) syd(-) syd(+) syd(~) syd(!)
#undef u

//Struct liczący na liczbach wymiernych modulo
//w trybie debug: modulo i na long doublach, w trybie nondebug tylko modulo
//Domyślna wartość structa zet_p, to 0
//Wspiera operatory +, -, *, /, =, +=, -=, *=, /=, ==, - jednoargumentowy
//Działa też jeśli jednym argumentem jest zet_p, a drugim liczba ((unsigned) int / long long)
//Wypisywanie wyniku: printf("%d\n", x.get());
const int mod = 1e9 + 7;
long long inv(long long b) {//To jest oczywiście dla mod pierwszego
	assert(b);
	int e = mod - 2;
	long long r = 1;
	while (e) {
		if (e & 1)
			r = r * b % mod;
		b = b * b % mod;
		e >>= 1;
	}
	return r;
}
#ifdef LOCAL 
#define loc(r...) r
#else
#define loc(...)
#endif
struct zet_p {
	int v;
	loc(long double w;)
	sim = int> zet_p(c r = 0) : v(r % mod) loc(, w(r)) {}
	loc(sim> zet_p(c a, long double b) : v(a % mod), w(b) {})
	#define rer(o, f) zet_p operator o (zet_p y) {return zet_p(v f loc(, w o y.w));} \
		zet_p & operator o##=(zet_p y) {ris = *this o y;}
	rer(+, +y.v) rer(-, -y.v) rer(*, * 1ll * y.v) rer(/, * inv(y.v))
	int get() {return ((unsigned int)v + mod) % mod;} //Rzutowanie jest potrzebne tylko, jeśli 2*mod nie mieści się w incie
	zet_p operator-(){return zet_p(-v loc(, -w));}
	bool operator==(zet_p y) {return (v - y.v) % mod == 0;}
};
#define ccy(o) sim> zet_p operator o(c a, zet_p b) {return zet_p(a) o b;}
//Jeśli chcemy, żeby działały też operatory na parach (zet_p, coś), to dajemy
//#define ccy(o) sim> typename enable_if<is_integral<c>::value, zet_p>::type operator o(c a, zet_p b) {return zet_p(a) o b;}
ccy(+) ccy(-) ccy(*) ccy(/)
sim> bool operator == (c a, zet_p b) {return zet_p(a) == b;}
loc(muu & operator<<(muu & d, zet_p y) {return d << "<" << y.get() << "=" << y.w << ">";})
void main2() {
	int x;
	scanf("%d", &x);
	zet_p a = x;
	printf("%d\n", (2 * a * a - 1).get()); //dopilnuje, żeby get było z przedziału [0, mod)
}

//Mnożenie long longów modulo (jak nie ma __int128)
//zakłada, że |a|, |b| < m
//dla m <= 1e12: float
//dla m <= 1e17: double
//dla m > 1e17: long double
using ll = long long;
ll __attribute__ ((no_sanitize_undefined)) mul(ll a,ll b, ll m) {
	ll q = ((double)a * b / m);
	ll r = (a*b - q*m) % m;  //ma się przekręcać
	if (r < 0)
		r += m;
	return r;
}
ll mul2(ll a, ll b, ll c) {
	return a * (__int128) b % c;
}
//Sztuczki z maskami bitowymi
//Iterowanie się po zapalonych bitach: forbits w kolejności rosnącej, fordbits w kolejności malejącej
//Zagnieżdżanie pętli forbits/fordbits wywali warning: declaration of ‘quq’ shadows a previous local ... in expansion of macro ‘forbits’
//Nie należy się nim przejmować
#define forbits(i, m) for (int i = __builtin_ctz(m), quq = m; quq; quq ^= (1 << i), i = __builtin_ctz(quq)) //dla long longa używamy __builtin_ctzll
#define fordbits(i, m) for (int i = 31 - __builtin_clz(m), quq = m; quq; quq ^= (1 << i), i = 31 - __builtin_clz(quq)) //a tu __builtin_clzll i 63 zamiast 31
using unt = unsigned int;
inline unt rrh(unt m) {
	unt a = ~(m | m >> 1);
	unt b = a & -a;
	return (m | b) &~ (b - 1);
}
inline unt lyl(unt m) {
	unt x = m & -m;
	return (m ^ x) | ((x << 1) / 3);
}
inline unt ros(unt m) {
	unt a = m & -m;
	unt b = ~((a - 1) ^ m);
	unt c = b & -b;
	unt d = (c >> (1 + __builtin_ctz(a))) - 1;
	return c | (m & ~(c - 1)) | d;
}
inline unt rev(unt m) {//Odwraca kolejność bitów: 10110000 -> 00001101
	const static unt s1 = (~0u) / 3, s2 = (~0u) / 5, s4 = (~0u) / 17, s8 = (~0u) / 257, s16 = (~0u) / 65537;
	//Dla ull jeszcze s32 = (~0u), a w tych na górze zamieniamy 0u na 0ull
	#define her(i) m = ((m & s##i) << i) | ((m >> i) & s##i);
	her(1) her(2) her(4) her(8) her(16) //Dla ull jeszcze: her(32)
	return m;
}
int main() {
	main2();
	int n = 30, k = 15;
	unt mm = 1234567;
	for (unt i = mm; ; i = (i - 1) & mm) {
		//i - podzbiór mm
		if (!i) break;
	}
	for (unt i = 0; i < (1u << n); i = rrh(i)) { //na unsigned intach działa dla n <= 31
		//i - maska bez dwóch sąsiednich zapalonych bitów; kolejność rosnąca
	}
	for (unt i = (1u << (n + 1)) / 3; ; i = lyl(i)) { //na unsigned intach działa dla n <= 30
		//m - maska bez dwóch sąsiednich zapalonych bitów; kolejność malejąca
		if (!i) break;
	}
	
	//Iterowanie się po maskach w takiej kolejności, że każde dwie kolejne różnią się jednym bitem
	//go(0)
	for (unt quq = 1, m = 0; quq < (1u << n); ++quq) {
		int i = __builtin_ctz(quq);
		m ^= (1 << i);
		//go(m) - kolejna maska
	}

	//Iterowanie się po maskach mających dokładnie k zapalonych bitów. Działa dla k >= 1
	for (unt m = ((1u << k) - 1); m < (1u << n); m = ros(m)) {
		//m - maska z dokładnie k zapalonymi bitami. kolejność rosnąca
	}
}
//Szybsza unordered_mapa
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
sim, class m> using my_unordered_map = cc_hash_table<c, m, hash<c>>;
