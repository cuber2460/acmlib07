// sprawdzanie czasu
while(clock()<=69*CLOCKS_PER_SEC)

// tablica o indeksach [-nax, nax]
int tab_[nax * 2 + 1];
#define tab (tab_ + nax)

// tablica o indeksach [-nax, nax][-nax, nax][-nax, nax]
int tab_[nax * 2 + 1][nax * 2 + 1][nax * 2 + 1];
struct {
  struct A {
    int x;
    int* operator[](int y) { return tab_[x + nax][y + nax] + nax; }
  };
  A operator[](int x) { return {x}; }
} tab;

// wygodne wypisywanie structa
debug & operator << (debug & dd, P p) {
	dd << "(" << p.x << ", " << p.y << ") "; // lub: p.print()
	return dd;
}

// iterowanie sie po kilku obiektach

for(int i : {1,2,4}) { ... }

for(vector<int>* it : {&a,&b}) { // chcemy posortowac wektory a oraz b
	vector<int> & x = *it;
	sort(x.begin(), x.end());
}



/* --- Wzbogacony ordered_set z c++11.
		Trzeba kompilować z -std=c++11.   ---   */

#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

template <typename T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

ordered_set<int> s;

int main() {
  s.insert(1);
  s.insert(2);
  cout << s.order_of_key(1) << endl;    // Out: 0.
  cout << *s.find_by_order(1) << endl;  // Out: 2.
}

// Własna funkcja haszująca.
struct MyHash {
  std::size_t operator()(const MojTyp& x) { /* ... */ }
};

bool operator==(const MojTyp& a, const MojType& b) { /* ... */ }

std::unordered_set<MojTyp, MyHash> secik;

// ,,Find first'' oraz ,,Find third'' w bitsecie:
vector<int> ListBits(bitset<1005> bs) {
  vector<int> result;
  for (int pos = bs._Find_first(); pos != bs.size(); pos = bs._Find_next(pos)) {
    result.push_back(pos);
  }
  return result;
}

// szybsze unordered_map oraz unordered_set
secik.max_load_factor(0.25); // ratio liczba_elementow / liczba_bucketow, domyslnie 1.0 (ustaw na INF, jesli chcesz uzywac tylko reserve)
secik.reserve(1<<15); // maksuje liczbe bucketow do danej liczby, zalecana potega 2, np. 1<<22 dla n=1e6
int secik.bucket_count(); // zwraca obecna liczbe bucketow, sluzy do testowania 

// limit_denominator
import fractions
n = int(input())
for i in range(n):
  a = fractions.Fraction(input())
  b = a.limit_denominator(1000 * 1000 * 1000)
  print(b.numerator, b.denominator)
