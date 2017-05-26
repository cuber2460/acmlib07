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
