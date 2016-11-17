// adding points online, O(N * log(N))
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
struct P {
	ll x, y;
	void read() { scanf("%lld%lld", &x, &y); }
	void write() const { printf("(%lld,%lld), ", x, y); }
	ll operator * (const P & b) const { return x * b.y - y * b.x; }
	P operator - (const P & b) const { return P{x - b.x, y - b.y}; }
};
struct cmp_x1 {
	bool operator()(const P & a, const P & b) {
		return make_pair(a.x, b.x) < make_pair(a.y, b.y);
	}
};
struct cmp_x2 {
	bool operator()(const P & a, const P & b) {
		return make_pair(a.x, b.x) > make_pair(a.y, b.y);
	}
};

typedef function<bool(const P &, const P &)> foo;
typedef set<P, foo> my_set;
struct Hull {
	my_set up, down;
	Hull() {
		up = my_set(cmp_x1());
		down = my_set(cmp_x2());
	}
	void add(const P & last) {
		for(int rep = 0; rep < 2; ++rep) {
			my_set & s = (rep == 0) ? up : down;
			auto belongs = [&] (my_set :: iterator it) {
				assert(it != s.end());
				if(it == s.begin() || next(it, 1) == s.end()) return true;
				P A = *next(it, -1);
				P B = *it;
				P C = *next(it, 1);
				if((B - A) * (C - A) < 0) return true;
				s.erase(it);
				return false;
			};
			auto it = s.insert(last).first;
			if(belongs(it)) {
				while(it != s.begin() && !belongs(next(it, -1)));
				while(next(it, 1) != s.end() && !belongs(next(it, 1)));
			}
		}
	}
	vector<P> get() const {
		vector<P> w;
		for(auto p : up) w.push_back(p);
		if((int) w.size() <= 1) return w;
		w.pop_back();
		for(auto p : down) w.push_back(p);
		w.pop_back();
		return w;
	}
};
