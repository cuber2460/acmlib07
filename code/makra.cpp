#ifndef LOCAL
#pragma GCC optimize ("O3")
#endif
#include <bits/stdc++.h>
using namespace std;
#define sim template < class c
#define ris return * this
#define mor > muu & operator << (
#define R22(r) sim > typename \
  enable_if<1 r sizeof dud<c>(0), muu&>::type operator<<(c g) {
sim > struct rge { c b, e; };
sim > rge<c> range(c h, c n) { return {h, n}; }
sim > auto dud(c* r) -> decltype(cerr << *r);
sim > char dud(...);
struct muu {
#ifdef LOCAL
stringstream a;
~muu() { cerr << a.str() << endl; }
R22(<) a << boolalpha << g; ris; }
R22(==) ris << range(begin(g), end(g)); }
sim, class b mor pair < b, c > r) { ris << "(" << r.first << ", " << r.second << ")"; }
sim mor rge<c> u) {
  a << "[";
  for (c i = u.b; i != u.e; ++i)
    *this << ", " + 2 * (i == u.b) << *i;
  ris << "]";
}
#else
sim mor const c&) { ris; }
#endif
};
#define imie(r...) "[" #r ": " << (r) << "] "
#define imask(r...) "[" #r ": " << bitset<8 * sizeof(r)>(r) << "] "
#define arr(a, i) "[" #a imie(i) ": " << a[i] << "] "
#define arr2(a, i, j) "[" #a imie(i) imie(j) ": " << a[i][j] << "] "
#define debug muu() << __FUNCTION__ << "#" << __LINE__ << ": "

int main() {
  int a = 81281;
  set<int> b{3, 5, 7};
  debug << imie(a) imie(b) imie(vector<int>{1, 2, 3}) << 10 << imask(0xbaca);
  // -std=c++11 -g -DLOCAL -Wshadow -Wall -fsanitize=undefined -fsanitize=address -D_GLIBCXX_DEBUG
  return 0;
}
