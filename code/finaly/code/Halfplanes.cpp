const int inf = 1e9;
using pii = pair <int, int>;
using ll = long long;
pii operator-(pii a, pii b){return {a.first - b.first, a.second - b.second};}
pii rot(pii x) {return {x.second, -x.first};}
using hpl = pair <pii, ll>; //(v, m) = {x : sc(x, v) >= m}
ll sc(pii a, pii b) {return a.first * 1ll * b.first + a.second * 1ll * b.second;}
ll ve(pii a, pii b) {return a.first * 1ll * b.second - a.second * 1ll * b.first;}
//wartości rzędu współrzędne ^ 4
__int128 det(hpl a, hpl b, hpl c) {
	return a.second*ve(b.first,c.first)+b.second*ve(c.first,a.first)+c.second*ve(a.first,b.first);
}
bool subset(hpl a, hpl b) {
	if (ve(a.first, b.first) || sc(a.first, b.first) < 0) return false;
	return a.second * (__int128) abs(b.first.first) >= b.second * (__int128) abs(a.first.first) &&
		a.second * (__int128) abs(b.first.second) >= b.second * (__int128) abs(a.first.second);
}
bool disjoint(hpl a, hpl b) {return subset(a, {{-b.first.first, -b.first.second}, -b.second});}
bool around(hpl a, hpl b, hpl c) {
	ll ab = ve(a.first, b.first);
	ll bc = ve(b.first, c.first);
	ll ca = ve(c.first, a.first);
	assert(ab > 0 || bc > 0 || ca > 0);
	return (ab >= 0 && bc >= 0 && ca >= 0);
}
void ang_sort(vector <hpl> &a) {
	if (a.empty()) return;
	hpl mid = a.back();
	a.pop_back();
	vector <hpl> left, right;
	for (hpl c : a)
		if (ve(c.first, mid.first) > 0)
			left.push_back(c);
		else
			right.push_back(c);
	sort(right.begin(), right.end(), [](hpl x, hpl y){return ve(x.first, y.first) > 0;});
	sort(left.begin(), left.end(), [](hpl x, hpl y){return ve(x.first, y.first) > 0;});
	left.push_back(mid);
	left.insert(left.end(), right.begin(), right.end());
	swap(a, left);
}
//Jeśli przecięcie jest nieograniczone, to półproste idące do nieskończoności są wyznaczone
//przez pierwszy i ostatni element zwróconego wektora
//Jeśli przecięcie jest puste, zwraca pusty wektor
vector <hpl> find_hull(vector <hpl> vec) {
	vector <hpl> hull;
	int first = 0;
	ang_sort(vec);
	for (hpl curr : vec) {
		debug << imie(curr);
		if (disjoint(curr, hull.back()))
			return {}; //Przecięcie jest puste
		if (!hull.empty() && subset(hull.back(), curr)) //Case kiedy jedna półpłaszczyzna zawiera się w drugiej, na ogół można wywalić
			continue;
		if (!hull.empty() && subset(curr, hull.back())) //J.w.
			hull.pop_back();
		while (hull.size() - first >= 2u && det(hull.back(), *(hull.end() - 2), curr) <= 0) {
			if (around(hull[hull.size() - 2], hull.back(), curr))
				return {}; //Przecięcie jest puste
			else
				hull.pop_back();
		}
		while (hull.size() - first >= 2u && det(curr, hull[first], hull[first + 1]) >= 0) {
			if (around(curr, hull[first], hull[first + 1]))
				return {}; //Przecięcie jest puste
			else
				first++;
		}
		if (hull.size() - first < 2u || det(hull.back(), curr, hull[first]) < 0)
			hull.push_back(curr);
	}
	return vector <hpl>(hull.begin() + first, hull.end());
}
//Półpłaszczyzna wyznaczona przez prostą xy
hpl make_hpl(pii x, pii y) {
	pii v = rot(y - x);
	assert(sc(v, x) == sc(v, y));
	return {v, sc(x, v)};
}
