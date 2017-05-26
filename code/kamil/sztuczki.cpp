// tablica o indeksach [-nax, nax]
int tab_[nax * 2 + 1];
#define tab (tab_ + nax)

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
