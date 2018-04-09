typedef long long ll;
struct P {
	ll x, y; int id;
	void read(int _id) { id = _id; scanf("%lld%lld", &x, &y); }
	void write() const { printf("(%lld,%lld)", x, y); }
	ll operator * (const P & b) const { return x * b.y - y * b.x; }
	P operator + (const P & b) const { return P{x + b.x, y + b.y}; }
	P operator - (const P & b) const { return P{x - b.x, y - b.y}; }
	bool operator < (const P & b) const {
		return make_pair(x, make_pair(y, id))
			< make_pair(b.x, make_pair(b.y, id));
	}
	bool operator == (const P & b) const {
		return make_pair(x, y) == make_pair(b.x, b.y); }
	bool under(const P & a, const P & b) const {
		return (b - a) * (*this - a) <= 0;
	}
};
struct Node {
	int a_size, b_size;
	P *a, *b, *first;
	Node *L, *R;
	Node() { a_size=b_size=0; a=b=first=0; L=R=0; }
	Node(P *p) { *this = Node(); a_size=1; first=p; }
	Node(Node *l, Node *r) { *this = Node(); L=l; R=r; act(); }
	int size() const { return a_size + b_size; }
	bool empty() const { return !first; }
	bool leaf() const { return !L; }
	void act() {
		if(leaf()) return;
		if(!L->empty()) first = L->first;
		else if(!R->empty()) first = R->first;
		else first = NULL;
		if(L->empty() || R->empty()) {
			a = L->first; // possibly NULL
			a_size = L->size();
			b = R->first;
			b_size = R->size();
		}
		else {
			a_size = b_size = 1;
			act(L, R); // modifies a_size and b_size
		}
	}
	private : void act(Node *inter1, Node *inter2) {
		/*if(inter1->leaf() && inter2->leaf()) {*/ //if there is no size()
		if(inter1->size() == 1 && inter2->size() == 1) {
			a = inter1->first;
			b = inter2->first;
			return;
		}
		P *A = inter1->a, *B = inter1->b;
		P *C = inter2->a, *D = inter2->b;
		if(inter1->L && (!B || (A && C && B->under(*A, *C))))
			return act(inter1->L, inter2);
		if(!C || (B && D && C->under(*B, *D)))
			return act(inter1, inter2->R);
		auto x = inter2->first->x;
		long double x1 = B->x - (A ? A->x : 0), x2 = (D ? D->x : 0) - C->x;
		if(!A || (A && D && (A->y - C->y) * x1 * x2 + x2 * (B->y - A->y)
				* (x - A->x) - x1 * (D->y - C->y) * (x - C->x) > 0)) {
			a_size += inter1->size() - inter1->R->size();
			return act(inter1->R, inter2);
		}
		b_size += inter2->size() - inter2->L->size();
		return act(inter1, inter2->L);
		/*long double x1 = B->x - A->x, x2 = D->x - C->x;
		#define remA {a_size += inter1->size() - inter1->R->size(); \
				return act(inter1 -> R, inter2);}
		#define remB return act(inter1 -> L, inter2)
		#define remC return act(inter1, inter2 -> R)
		#define remD {b_size += inter2->size() - inter2->L->size(); \
				return act(inter1, inter2 -> L);}
		if(!A) remA; if(!B) remB; if(!C) remC; if(!D) remD;
		if(B -> underLine(*A, *C)) remB;
		if(C -> underLine(*B, *D)) remC;
		auto x = inter2->first->x;
		long double x1 = B->x - A->x, x2 = D->x - C->x;
		if((A->y - C->y) * x1 * x2 + x2 * (B->y - A->y) * (x - A->x)
			- x1 * (D->y - C->y) * (x - C->x) > 0) remA else remD;*/
	} public :
	ll query(ll mul) const { // maximize mul*x+y
		auto evaluate = [&] (const P & p) { return mul * p.x + p.y; };
		// if(empty()) return +-INFINITY;
		if(size() == 1) return evaluate(*first);
		if(L -> empty()) return R -> query(mul);
		if(R -> empty()) return L -> query(mul);
		return (evaluate(*a) > evaluate(*b) ? L : R) -> query(mul);
	}
	// returns true if something was removed
	bool remove(ll A, ll B, ll C) { // cut points above Ax+By+C=0
		if(empty()) return false;
		auto evaluate = [&] (const P & p) { return A * p.x + B * p.y; };
		if(leaf()) {
			if(evaluate(*first) >= -C) {
				*this = Node();
				return true;
			}
			return false;
		}
		auto left = a ? evaluate(*a) : 0, right = b ? evaluate(*b) : 0;
		bool modified = 0;
		if(a && (!b || left > right || left >= -C))
			modified |= L->remove(A,B,C);
		if(b && (!a || left < right || right >= -C))
			modified |= R->remove(A,B,C);
		if(modified) act();
		return modified;
	}
	void getHull(int a_skip, int b_skip, vector<P> & w) const {
		if(a_skip + b_skip >= size()) return;
		if(size() == 1) {
			assert(first);
			w.push_back(*first);
			return;
		}
		assert(L && R);
		L->getHull(a_skip, max(0, b_skip - b_size) + L->size() - a_size, w);
		R->getHull(max(0, a_skip - a_size) + R->size() - b_size, b_skip, w);
	}
};
const int UPPER = 0, LOWER = 1;
const int REPS = 2; // 1 means computing UPPER only
long long done[1 << 20];
struct Hull {
	vector<Node> tr[2];
	vector<P> all;
	int pot;
	int getID(const P & a, int type) const {
		int tmp = lower_bound(all.begin(), all.end(), a) - all.begin();
		assert(all[tmp] == a); // I didn't get it in the constructor
		return tmp;
	}
	Hull(vector<P> _all) : all(_all) {
		sort(all.begin(), all.end());
		all.resize(unique(all.begin(), all.end()) - all.begin());
		pot = 1;
		while(pot < (int) all.size()) pot *= 2;
		for(int rep = 0; rep < REPS; ++rep) {
			vector<Node> & t = tr[rep];
			t.clear();
			t.resize(2 * pot);
			for(int i = 1; i < pot; ++i) {
				t[i].L = & t[2*i];
				t[i].R = & t[2*i+1];
			}
		}
		for(int i = 0; i < (int) all.size(); ++i) todo.push(pot + i);
	}
	priority_queue<int> todo;
	void change(const P & last) {
		for(int type = 0; type < REPS; ++type) {
			int id = getID(last, type);
			P * which_point = & all[id];
			if(type == LOWER) id = (int) all.size() - 1 - id;
			Node & tmp = tr[type][pot+id];
			if(tmp.empty()) {
				tmp.a_size = 1;
				tmp.first = which_point;
			}
			else {
				tmp.a_size = 0;
				tmp.first = NULL;
			}
			todo.push(pot + id);
		}
	}
	vector<P> get() {
		static long long T = 0;
		++T;
		while(!todo.empty()) {
			int id = todo.top();
			todo.pop();
			if(done[id] == T) continue;
			done[id] = T;
			tr[0][id].act();
			tr[1][id].act();
			if(id != 1) todo.push(id / 2);
		}
		todo = priority_queue<int>();
		// ---
		vector<P> w;
		// printf("should = %d\n", (int) tr[0][1].size());
		tr[0][1].getHull(0, 0, w);
		if((int) w.size() <= 1) return w;
		w.pop_back();
		tr[1][1].getHull(0, 0, w);
		w.pop_back();
		return w;
	}
};
const int nax = 6e5 + 5;
int type[nax], val[nax]; P p[nax];
int main() {
	int T;
	scanf("%d", &T);
	while(T--) {
		int n;
		scanf("%d", &n);
		vector<P> w(n);
		for(int i = 0; i < n; ++i) {
			w[i].read(i);
			w[i].id = i + 1;
		}
		Hull hull(w);
		set<P> s;
		for(int i = 0; i < n; ++i) {
			if(!s.count(w[i])) {
				s.insert(w[i]);
				// printf("(%d,%d)\n", (int) w[i].x, (int) w[i].y);
				hull.change(w[i]);
			}
		}
		w = hull.get();
	}
}
