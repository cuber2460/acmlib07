typedef long long T;
struct P {
	T x, y;
	int id;
	P operator-(P b) { return P{x - b.x, y - b.y}; }
	T cross(P b) { return x * b.y - y * b.x; }
	T cross(P b, P c) const { return (b - *this).cross(c - *this); }
	T dot(P b) { return x * b.x + y * b.y; }
	bool inTriangle(const P &a, const P &b, const P &c) const {
#define tmp(a, b) (cross(a, b) > 0)
		return tmp(a, b) == tmp(b, c) && tmp(b, c) == tmp(c, a);
#undef tmp
	}
};
int cmpCircle(P a, P b, P c, P d) {
	P v1 = b - a, v2 = d - a;
	P v3 = b - c, v4 = d - c;
	long double tmp = (long double)abs(v1.cross(v2)) * v3.dot(v4) +
										(long double)v1.dot(v2) * abs(v3.cross(v4));
	if (abs(tmp) < 1e-8) return 0;
	if (tmp == 0) return 0;
	if (tmp > 0) return 1;
	return -1;
}
struct pair_hash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const {
		return p.first * 10000 + p.second;
	}
};
unordered_map<pair<int, int>, pair<int, int>, pair_hash> mt;
set<pair<int, int>> edges;
vector<vector<int>> triangles;
void rec(int a, int c, const vector<P> &points);
void trim(int a, int b) {
	assert(a < b);
	auto it = mt.find({a, b});
	if ((it != mt.end()) && (it->second == make_pair(-1, -1))) mt.erase(it);
}
void change(int a, int b, int from, int to) {
	if (a > b) swap(a, b);
	if (!mt.count({a, b})) mt[{a, b}] = {-1, -1};
	for (int *x : vector<int *>{&mt[{a, b}].first, &mt[{a, b}].second})
		if (*x == from) {
			*x = to;
			trim(a, b);
			return;
		}
	assert(false);
}
void rec(int a, int c, const vector<P> &points) {
	if (a > c) swap(a, c);
	if (!mt.count({a, c})) return;
	int b = mt[{a, c}].first;
	int d = mt[{a, c}].second;
	if (b > d) swap(b, d);
	if (b == -1 || d == -1) return;
	for (int rep = 0; rep < 2; ++rep) {
		if (points[a].inTriangle(points[b], points[c], points[d])) return;
		swap(a, c);
	}
	if (cmpCircle(points[a], points[b], points[c], points[d]) != 1) return;
	assert((!mt.count({b, d})) || (mt[{b, d}] == make_pair(-1, -1)));
	mt[{b, d}] = {a, c};
	trim(b, d);
	mt.erase(make_pair(a, c));
	change(a, b, c, d);change(b, c, a, d);change(a, d, c, b);change(c, d, a, b);
	rec(a, b, points);rec(b, c, points);rec(c, d, points);rec(d, a, points);
}
void addTriangle(int a, int b, int c) {
	change(a, b, -1, c);change(a, c, -1, b);change(b, c, -1, a);
}
void anyTriangulation(vector<P> points) {
	sort(points.begin(), points.end(), [](const P &a, const P &b) {
		return make_pair(a.x, a.y) < make_pair(b.x, b.y);});
	vector<P> upper, lower;
	for (P C : points) {
#define backback(w) w[(int)w.size() - 2]
		while ((int)upper.size() >= 2 && backback(upper).cross(upper.back(), C) > 0) {
			addTriangle(C.id, backback(upper).id, upper.back().id);
			upper.pop_back();
		}
		upper.push_back(C);
		while ((int)lower.size() >= 2 && lower[(int)lower.size() - 2].cross(lower.back(), C) < 0) {
			addTriangle(C.id, backback(lower).id, lower.back().id);
			lower.pop_back();
		}
		lower.push_back(C);
#undef backback
	}
	if(lower.size()==upper.size()&&lower.size()==points.size())assert(false);//all collinear
}
const int nax = 1e6 + 5;
int memo_x[nax], memo_y[nax];
void dewolaj() {
	mt.clear();edges.clear();triangles.clear();
	int n;
	scanf("%d", &n);
	vector<P> points(n);
	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &memo_x[i], &memo_y[i]);
		points[i] = P{memo_x[i], memo_y[i], i};
	}
	mt.reserve(4123123);
	anyTriangulation(points);
	vector<pair<int, int>> init;
	for (auto ppp : mt) init.push_back(ppp.first);
	for (pair<int, int> p : init)
		if (mt.count(p) && mt[p] != make_pair(-1, -1)) rec(p.first, p.second, points);
	for (auto ppp : mt)
		if (ppp.second != make_pair(-1, -1)) {
			int i = ppp.first.first, j = ppp.first.second;
			assert(i != j);
			edges.insert({i, j});
			if (mt[{i, j}].first > j) triangles.push_back(vector<int>{i, j, mt[{i, j}].first});
			if (mt[{i, j}].second > j) triangles.push_back(vector<int>{i, j, mt[{i, j}].second});
		}
	// edges zawiera krawędzie triangulacji
}
