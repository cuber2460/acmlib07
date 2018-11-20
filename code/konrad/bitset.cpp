#define private public
#include <bitset>
#undef private
#include <bits/stdc++.h>
#define tab _M_w
using namespace std;
#ifdef CF //u nas system 32 bitowy, a na sprawdzaczce 64, lub odwrotnie
using ull = typename remove_reference<decltype(bitset<1024>().tab[0])>::type;
int popc(unsigned long long x) {return __builtin_popcountl(x);}
int popc(unsigned int x) {return __builtin_popcount(x);}
//I wszędzie zamiast __builtin_popcountl używamy popc
#else
using ull = unsigned long long;
#endif
const int SIZE = 8 * sizeof(ull);
const int LOG = __builtin_ctz(SIZE);
//Iteruje sie po przedziale [l, r)
//Ustawia i na index w tablicy tab, mask na zbior bitow przecinajacych sie z przedzialem [l, r)
//Wykonuje dla kazdego takiego ustawienia fragment code
#define run(l, r, code...) { \
	assert(l < r);\
	int L = l >> LOG;\
	int R = r >> LOG;\
	int lmod = l & (SIZE - 1);\
	int rmod = r & (SIZE - 1);\
	if (L == R) {\
		int i = L;\
		ull mask = ~(~(ull)0 << (rmod - lmod)) << lmod;\
		{code}\
	}\
	else {\
		ull mask = (~(ull)0 << lmod); \
		int i = L;\
		{code}\
		mask = ~(ull)0;\
		for (int i = L + 1; i < R; ++i) {code}\
		if (rmod) {\
			i = R;\
			mask = ~(~(ull)0 << rmod);\
			{code}\
		}\
	}\
}
//N musi byc >= 128 i podzielne przez 64
//Wszystkie funkcje operuja na przedzialach [l, r)
template <size_t N>
void flip(int l, int r, bitset<N> & b) {
	run(l, r, b.tab[i] ^= mask;)
}
template <size_t N>
void set_1(int l, int r, bitset<N> & b) {
	run(l, r, b.tab[i] |= mask;)
}
template <size_t N>
void set_0(int l, int r, bitset<N> &b) {
	run(l, r, b.tab[i] &= ~mask;)
}
template <size_t N>
int count(int l, int r, const bitset<N> &b) {
	int res = 0;
	run(l, r, res += __builtin_popcountl(b.tab[i] & mask);) //builtin_popcountl z l na koncu
	return res;
}
int kth(ull mask, int num) {
	for (int i = 0; i < SIZE; ++i)
		if (mask >> i & 1) {
			num--;
			if (num == 0)
				return i;
		}
	assert(false);
}
//Zwraca k-ty zapalony bit (liczac od jedynki, od najmniejszych indeksow)
template <size_t N>
int kth_one(int k, const bitset<N> &b) {
	for (int i = 0; i < (int)(N >> LOG); ++i) {
		int c = __builtin_popcountl(b.tab[i]);
		if (c < k) {
			k -= c;
		}
		else {
			return (i << LOG) | kth(b.tab[i], k);
		}
	}
	return -1; //bitset ma mniej niz k jedynek
}
