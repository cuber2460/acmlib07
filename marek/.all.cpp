/*****************************************************************************/
/*                                                                           */
/*                                  linkcut                                  */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

class Splay;

int getId(Splay * s);
void wypisz();

struct Value
{
	int size;
	int czarne;
	bool color;
	bool flop;

	Value() : size(1), czarne(false), color(

	void touch(Value * left, Value * right)
	{
		if(flop)
		{
			color = !color;
			czarne = size - czarne;
			flop = !flop;
			if(left) left->flop = !left->flop;
			if(right) right->flop = !right->flop;
		}
	}

	void update(Value * left, Value * right)
	{
		size = 1;
		czarne = color ? 1 : 0;
		if(left)
		{
			size += left->size;
			czarne += left->czarne;
		}
		if(right)
		{
			size += right->size;
			czarne += right->czarne;
		}
	}
};

struct Splay : Value
{
	Splay *l, *r, *p;
	bool flip;

	template<typename ...Args>
	Splay(const Args & ...args)
		: Value(args...)
		,l(nullptr) ,r(nullptr) ,p(nullptr)
		,flip(false)
	{
	}

	bool isRoot()
	{
		return !p || (p->l != this && p->r != this);
	}

	void touch()
	{
		if(flip)
		{
			swap(l, r);
			if(l) l->flip = !l->flip;
			if(r) r->flip = !r->flip;
			flip = false;
		}
		Value::touch(l, r);
	}

	void update()
	{
		Value::update(l, r);
	}

	static void connect(Splay * p, Splay * c, bool left)
	{
		(left ? p->l : p->r) = c;
		if(c) c->p = p;
	}

	/* Zakłada, że nie jest ,,rootem'' */
	void rotate()
	{
		Splay * f = p;
		Splay * t = f->p;
		f->touch();
		touch();
		bool isr = f->isRoot();
		bool left = (this == f->l);
		connect(f, left ? r : l, left);
		connect(this, f, !left);
		if(isr) this->p = t;
		else connect(t, this, f == t->l);
		if(f->r) f->r->touch();
		if(f->l) f->l->touch();
		f->update();
	}

	void splay()
	{
		while(!isRoot())
		{
			Splay * r = p->p;
			if(!p->isRoot()) (((p->l == this) == (r->l == p)) ? p : this)->rotate();
			rotate();
		}
		touch();
		if(l) l->touch();
		if(r) r->touch();
		update();
	}

	/* Podnosi wierzchołek do góry (będzie korzeniem splaya, w którym znajduje się korzeń drzewa :P). */
	static Splay * lastDisc;
	void expose()
	{
		Splay * r = nullptr;
		lastDisc = nullptr;
		for(Splay * q = this; q; q = q->p)
		{
			q->splay();
			q->r = r;
			q->update();
			r = q;
			lastDisc = q;
		}
		splay();
	}

	void link(Splay * to)
	{
		expose();
		assert(l == nullptr); // to powinien być root
		p = to;
	}

	Splay * root()
	{
		expose();
		Splay * s = this;
		while(s->l)
		{
			s = s->l;
			s->touch();
		}
		s->splay();
		return s;
	}

	/* Wywołaj funkcję f na poddrzwie reprezentującym ścieżkę od korzenia do wierzchołka. */
	void applyF(std::function<void(Splay*)> f)
	{
		expose();
		if(r == nullptr) f(this);
		else
		{
			Splay * s = r;
			while(s->l)
			{
				s = s->l;
				s->touch();
			}
			s->splay();
			f(s->l);
		}
	}

	void makeRoot()
	{
		applyF([] (Splay * s) {
			s->flip = !s->flip;
		});
	}

	void cut()
	{
		expose();
		assert(l != nullptr); // jest rootem, nie można obciąć...
		Splay * s = l;
		while(s->r) s = s->r;
		s->splay();
		s->r->p = nullptr;
		s->r = nullptr;
	}

	/* Zwraca lca(this, v) */
	Splay * lca(Splay * v)
	{
		expose();
		v->expose();
		return lastDisc;
	}

};

Splay * Splay::lastDisc;

/* Wywołaj funkcję f na poddrzewie reprezentującym ścieżkę od a do b. */
void applyF(Splay * a, Splay * b, std::function<void(Splay*)> f)
{
	a->makeRoot();
	b->applyF(f);
}

int nast = 0;
Splay spleje[100005];

template<typename ...Args>
Splay * newTree(const Args & ...args)
{
	return &spleje[nast++];
	//return new Splay(args...);
}

void deleteTree(Splay * t)
{
	if(t->l) deleteTree(t->l);
	if(t->r) deleteTree(t->r);
	delete t;
}

int n;
Splay * tab[100005];
map<long, int> ids;

int getId(Splay * s)
{
	return ids[(long) s];
}

void wypisz()
{
}

vector<int> graf[100005];

int dfs(int w, int o)
{
	for(int i : graf[w])
		if(i != o)
			dfs(i, w);
	if(o) tab[w]->link(tab[o]);
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 1; i <= n; i++)
		tab[i] = newTree();
	for(int i = 1; i < n; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		graf[a].emplace_back(b);
		graf[b].emplace_back(a);
	}
	dfs(1, 0);
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                   splay                                   */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

typedef int Key;

struct Splay
{
	Key key; // cena
	Splay *l, *r, *p;
	int size; /* SIZE */
	bool flip; /* FLIP */
	bool fliped; /* FLIP */

	Splay(const Key & key = -1) : key(key), l(nullptr), r(nullptr), p(nullptr), flip(false), fliped(false) {}
	#define Splay_comp(a, b, c) ((a < b) != c->fliped) /* FLIP */
	//#define Splay_comp(a, b, c) (a < b) /* ! FLIP */
};

/*
// Runs in O(n) time!!!!
Splay * root(Splay * p)
{
	while(p->p) p = p->p;
	return p;
}

void dajKey(Splay * p)
{
	if(p == nullptr) printf("(nil)");
	else printf("[%d]", p->key);
}

void wypisz(Splay * p)
{
	if(p == nullptr) return;
	printf("(key = %d, val = %d, najw = %d, spu = %d)", p->key, p->val, p->najw, p->spu);
	printf(" L: "); dajKey(p->l);
	printf(" R: "); dajKey(p->r);
	printf("\n");
	wypisz(p->l);
	wypisz(p->r);
}

void touch(Splay * n);

void dajPary(Splay * p)
{
	if(p == nullptr) return;
	touch(p);
	dajPary(p->l);
	printf(" (%d, %d)", p->key, p->val);
	dajPary(p->r);
}
*/

int used_allo = 0;
//vector<Splay> allo;
Splay allo[200005];

inline Splay * newSplay(const Key & key)
{
	//if(used_allo == (int) allo.size())
		//allo.resize(max(1, (int) allo.size()) * 2);
	allo[used_allo] = Splay(key);
	return &allo[used_allo++];
}

void touch(Splay * n)
{
	if(!n) return;
	/* FLIP */
	if(n->flip)
	{
		swap(n->l, n->r);
		if(n->l) n->l->flip = ! n->l->flip;
		if(n->r) n->r->flip = ! n->r->flip;
		n->flip = false;
		n->fliped = ! n->fliped;
	}
}

void update(Splay * n)
{
	if(!n) return;
	n->size = 1; /* SIZE */
	if(n->l) n->size += n->l->size; /* SIZE */
	if(n->r) n->size += n->r->size; /* SIZE */

	//assert(!(n->l) || !(n->l->spu));
	//assert(!(n->r) || !(n->r->spu));
}

void connect(Splay * p, Splay * s, bool left)
{
	(left ? p->l : p->r) = s;
	if(s) s->p = p;
}

void rotate(Splay * p)
{
	Splay * q = p->p;
	Splay * r = q->p;
	bool left = (p == q->l);
	touch(left ? q->r : q->l);
	connect(q, left ? p->r : p->l, left);
	connect(p, q, !left);
	if(r) connect(r, p, (r->l == q));
	else p->p = nullptr;
	update(q);
}

void splay(Splay * p)
{
	touch(p->l);
	touch(p->r);
	while(p->p)
	{
		Splay * q = p->p;
		Splay * r = q->p;
		touch((q->l == p) ? q->r : q->l);
		if(r) rotate(((q->l == p) == (r->l == q)) ? q : p);
		rotate(p);
	}
	update(p);
}

Splay * snext(Splay * p)
{
	if(!p || !p->r) return nullptr;
	p = p->r;
	while(p->l) p = p->l;
	splay(p);
	return p;
}

Splay * sprev(Splay * p)
{
	if(!p || !p->l) return nullptr;
	p = p->l;
	while(p->r) p = p->r;
	splay(p);
	return p;
}

Splay * find_first(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->l;
	}
	if(q) splay(q);
	return q;
}

Splay * find_last(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * join(Splay * l, Splay * r)
{
	if(!r) return l;
	r = find_first(r);
	touch(l);
	touch(r);
	connect(r, l, true);
	update(r);
	return r; }

// left < key, key <= right
void split(Splay * p, const Key & key, Splay * & left, Splay * & right)
{
	if(!p) { left = right = nullptr; return; }
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	splay(q);
	bool brig = Splay_comp(q->key, key, q);
	left = brig ? q : q->l;
	right = brig ? q->r : q;
	(brig ? q->r : q->l) = nullptr;
	if(left) left->p = nullptr;
	if(right) right->p = nullptr;
	update(q);
}

// dorzuca _wierzcholek_ s (wierzcholek, nie drzewo!!!)
Splay * insert(Splay * p, Splay * s)
{
	Splay *left, *right;
	split(p, s->key, left, right);
	right = find_first(right);
	if(right && !Splay_comp(s->key, right->key, right))
	{
		connect(s, right->l, true);
		connect(s, right->r, false);
		touch(right->l);
		touch(right->r);
		update(s);
		return join(left, s);
	}
	touch(left);
	touch(right);
	connect(s, left, true);
	connect(s, right, false);
	update(s);
	return s;
}

Splay * erase(Splay * p, const Key & key)
{
	Splay *left, *right;
	split(p, key, left, right);
	right = find_first(right);
	if(right && right->key == key && (right = right->r))
		right->p = nullptr;
	return join(left, right);
}

/* SIZE */
Splay * find_kth(Splay * p, int k) // indexed from 0
{
	Splay * q = nullptr;
	touch(p);
	while(p)
	{
		q = p;
		touch(p->l); touch(p->r);
		if(p->l && k < p->l->size) p = p->l;
		else if((k -= ((p->l) ? p->l->size : 0) + 1) == -1) break;
		else p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * find(Splay * p, const Key & key)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	if(q) splay(q);
	return q;
}

Splay * find_or_create(Splay * p, const Key & key)
{
	p = find(p, key);
	if(!p || Splay_comp(key, p->key, p) || Splay_comp(p->key, key, p))
		return insert(p, newSplay(key));
	return p;
}

/* Apllies a functiona on a tree representing the range [a, b) */
Splay * get_range(Splay * p, const Key & a, const Key & b, std::function<void(Splay*)> f)
{
	Splay *left, *middle, *right;
	split(p, a, left, middle);
	split(middle, b, middle, right);
	touch(middle);
	f(middle);
	return join(left, join(middle, right));
}


int main()
{
	int w, h, n;
	scanf("%d%d%d", &w, &h, &n);
	Splay *H = nullptr, *najH = nullptr, *V = nullptr, *najV = nullptr;
	H = insert(H, newSplay(0));
	H = insert(H, newSplay(h));
	najH = insert(najH, newSplay(h));
	V = insert(V, newSplay(0));
	V = insert(V, newSplay(w));
	najV = insert(najV, newSplay(w));
	while(n--)
	{
		char c; int x;
		scanf(" %c", &c);
		scanf("%d", &x);
		Splay * & A = (c == 'H') ? H : V;
		Splay * & najA = (c == 'H') ? najH : najV;
		A = insert(A, newSplay(x));
		A = find(A, x);
		auto nas = snext(A);
		auto pop = sprev(sprev(nas));
		najA = erase(najA, nas->key - pop->key);
		najA = insert(najA, newSplay(x - pop->key));
		najA = insert(najA, newSplay(nas->key - x));
		printf("usuwam %d\n", nas->key - pop->key);
		printf("dodaje1 %d\n", x - pop->key);
		printf("dodaje2 %d\n", nas->key - x);

		najH = find_last(najH);
		najV = find_last(najV);
		typedef long long int ll;
		printf("%lld\n", ((ll) najH->key) * (najV->key));
	}
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                  linkcut                                  */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

class Splay;

int getId(Splay * s);
void wypisz();

struct Value
{
	int size;
	int czarne;
	bool color;
	bool flop;

	Value() : size(1), czarne(false), color(

	void touch(Value * left, Value * right)
	{
		if(flop)
		{
			color = !color;
			czarne = size - czarne;
			flop = !flop;
			if(left) left->flop = !left->flop;
			if(right) right->flop = !right->flop;
		}
	}

	void update(Value * left, Value * right)
	{
		size = 1;
		czarne = color ? 1 : 0;
		if(left)
		{
			size += left->size;
			czarne += left->czarne;
		}
		if(right)
		{
			size += right->size;
			czarne += right->czarne;
		}
	}
};

struct Splay : Value
{
	Splay *l, *r, *p;
	bool flip;

	template<typename ...Args>
	Splay(const Args & ...args)
		: Value(args...)
		,l(nullptr) ,r(nullptr) ,p(nullptr)
		,flip(false)
	{
	}

	bool isRoot()
	{
		return !p || (p->l != this && p->r != this);
	}

	void touch()
	{
		if(flip)
		{
			swap(l, r);
			if(l) l->flip = !l->flip;
			if(r) r->flip = !r->flip;
			flip = false;
		}
		Value::touch(l, r);
	}

	void update()
	{
		Value::update(l, r);
	}

	static void connect(Splay * p, Splay * c, bool left)
	{
		(left ? p->l : p->r) = c;
		if(c) c->p = p;
	}

	/* Zakłada, że nie jest ,,rootem'' */
	void rotate()
	{
		Splay * f = p;
		Splay * t = f->p;
		f->touch();
		touch();
		bool isr = f->isRoot();
		bool left = (this == f->l);
		connect(f, left ? r : l, left);
		connect(this, f, !left);
		if(isr) this->p = t;
		else connect(t, this, f == t->l);
		if(f->r) f->r->touch();
		if(f->l) f->l->touch();
		f->update();
	}

	void splay()
	{
		while(!isRoot())
		{
			Splay * r = p->p;
			if(!p->isRoot()) (((p->l == this) == (r->l == p)) ? p : this)->rotate();
			rotate();
		}
		touch();
		if(l) l->touch();
		if(r) r->touch();
		update();
	}

	/* Podnosi wierzchołek do góry (będzie korzeniem splaya, w którym znajduje się korzeń drzewa :P). */
	static Splay * lastDisc;
	void expose()
	{
		Splay * r = nullptr;
		lastDisc = nullptr;
		for(Splay * q = this; q; q = q->p)
		{
			q->splay();
			q->r = r;
			q->update();
			r = q;
			lastDisc = q;
		}
		splay();
	}

	void link(Splay * to)
	{
		expose();
		assert(l == nullptr); // to powinien być root
		p = to;
	}

	Splay * root()
	{
		expose();
		Splay * s = this;
		while(s->l)
		{
			s = s->l;
			s->touch();
		}
		s->splay();
		return s;
	}

	/* Wywołaj funkcję f na poddrzwie reprezentującym ścieżkę od korzenia do wierzchołka. */
	void applyF(std::function<void(Splay*)> f)
	{
		expose();
		if(r == nullptr) f(this);
		else
		{
			Splay * s = r;
			while(s->l)
			{
				s = s->l;
				s->touch();
			}
			s->splay();
			f(s->l);
		}
	}

	void makeRoot()
	{
		applyF([] (Splay * s) {
			s->flip = !s->flip;
		});
	}

	void cut()
	{
		expose();
		assert(l != nullptr); // jest rootem, nie można obciąć...
		Splay * s = l;
		while(s->r) s = s->r;
		s->splay();
		s->r->p = nullptr;
		s->r = nullptr;
	}

	/* Zwraca lca(this, v) */
	Splay * lca(Splay * v)
	{
		expose();
		v->expose();
		return lastDisc;
	}

};

Splay * Splay::lastDisc;

/* Wywołaj funkcję f na poddrzewie reprezentującym ścieżkę od a do b. */
void applyF(Splay * a, Splay * b, std::function<void(Splay*)> f)
{
	a->makeRoot();
	b->applyF(f);
}

int nast = 0;
Splay spleje[100005];

template<typename ...Args>
Splay * newTree(const Args & ...args)
{
	return &spleje[nast++];
	//return new Splay(args...);
}

void deleteTree(Splay * t)
{
	if(t->l) deleteTree(t->l);
	if(t->r) deleteTree(t->r);
	delete t;
}

int n;
Splay * tab[100005];
map<long, int> ids;

int getId(Splay * s)
{
	return ids[(long) s];
}

void wypisz()
{
}

vector<int> graf[100005];

int dfs(int w, int o)
{
	for(int i : graf[w])
		if(i != o)
			dfs(i, w);
	if(o) tab[w]->link(tab[o]);
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 1; i <= n; i++)
		tab[i] = newTree();
	for(int i = 1; i < n; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		graf[a].emplace_back(b);
		graf[b].emplace_back(a);
	}
	dfs(1, 0);
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                   splay                                   */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

typedef int Key;

struct Splay
{
	Key key; // cena
	Splay *l, *r, *p;
	int size; /* SIZE */
	bool flip; /* FLIP */
	bool fliped; /* FLIP */

	Splay(const Key & key = -1) : key(key), l(nullptr), r(nullptr), p(nullptr), flip(false), fliped(false) {}
	#define Splay_comp(a, b, c) ((a < b) != c->fliped) /* FLIP */
	//#define Splay_comp(a, b, c) (a < b) /* ! FLIP */
};

/*
// Runs in O(n) time!!!!
Splay * root(Splay * p)
{
	while(p->p) p = p->p;
	return p;
}

void dajKey(Splay * p)
{
	if(p == nullptr) printf("(nil)");
	else printf("[%d]", p->key);
}

void wypisz(Splay * p)
{
	if(p == nullptr) return;
	printf("(key = %d, val = %d, najw = %d, spu = %d)", p->key, p->val, p->najw, p->spu);
	printf(" L: "); dajKey(p->l);
	printf(" R: "); dajKey(p->r);
	printf("\n");
	wypisz(p->l);
	wypisz(p->r);
}

void touch(Splay * n);

void dajPary(Splay * p)
{
	if(p == nullptr) return;
	touch(p);
	dajPary(p->l);
	printf(" (%d, %d)", p->key, p->val);
	dajPary(p->r);
}
*/

int used_allo = 0;
//vector<Splay> allo;
Splay allo[200005];

inline Splay * newSplay(const Key & key)
{
	//if(used_allo == (int) allo.size())
		//allo.resize(max(1, (int) allo.size()) * 2);
	allo[used_allo] = Splay(key);
	return &allo[used_allo++];
}

void touch(Splay * n)
{
	if(!n) return;
	/* FLIP */
	if(n->flip)
	{
		swap(n->l, n->r);
		if(n->l) n->l->flip = ! n->l->flip;
		if(n->r) n->r->flip = ! n->r->flip;
		n->flip = false;
		n->fliped = ! n->fliped;
	}
}

void update(Splay * n)
{
	if(!n) return;
	n->size = 1; /* SIZE */
	if(n->l) n->size += n->l->size; /* SIZE */
	if(n->r) n->size += n->r->size; /* SIZE */

	//assert(!(n->l) || !(n->l->spu));
	//assert(!(n->r) || !(n->r->spu));
}

void connect(Splay * p, Splay * s, bool left)
{
	(left ? p->l : p->r) = s;
	if(s) s->p = p;
}

void rotate(Splay * p)
{
	Splay * q = p->p;
	Splay * r = q->p;
	bool left = (p == q->l);
	touch(left ? q->r : q->l);
	connect(q, left ? p->r : p->l, left);
	connect(p, q, !left);
	if(r) connect(r, p, (r->l == q));
	else p->p = nullptr;
	update(q);
}

void splay(Splay * p)
{
	touch(p->l);
	touch(p->r);
	while(p->p)
	{
		Splay * q = p->p;
		Splay * r = q->p;
		touch((q->l == p) ? q->r : q->l);
		if(r) rotate(((q->l == p) == (r->l == q)) ? q : p);
		rotate(p);
	}
	update(p);
}

Splay * snext(Splay * p)
{
	if(!p || !p->r) return nullptr;
	p = p->r;
	while(p->l) p = p->l;
	splay(p);
	return p;
}

Splay * sprev(Splay * p)
{
	if(!p || !p->l) return nullptr;
	p = p->l;
	while(p->r) p = p->r;
	splay(p);
	return p;
}

Splay * find_first(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->l;
	}
	if(q) splay(q);
	return q;
}

Splay * find_last(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * join(Splay * l, Splay * r)
{
	if(!r) return l;
	r = find_first(r);
	touch(l);
	touch(r);
	connect(r, l, true);
	update(r);
	return r; }

// left < key, key <= right
void split(Splay * p, const Key & key, Splay * & left, Splay * & right)
{
	if(!p) { left = right = nullptr; return; }
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	splay(q);
	bool brig = Splay_comp(q->key, key, q);
	left = brig ? q : q->l;
	right = brig ? q->r : q;
	(brig ? q->r : q->l) = nullptr;
	if(left) left->p = nullptr;
	if(right) right->p = nullptr;
	update(q);
}

// dorzuca _wierzcholek_ s (wierzcholek, nie drzewo!!!)
Splay * insert(Splay * p, Splay * s)
{
	Splay *left, *right;
	split(p, s->key, left, right);
	right = find_first(right);
	if(right && !Splay_comp(s->key, right->key, right))
	{
		connect(s, right->l, true);
		connect(s, right->r, false);
		touch(right->l);
		touch(right->r);
		update(s);
		return join(left, s);
	}
	touch(left);
	touch(right);
	connect(s, left, true);
	connect(s, right, false);
	update(s);
	return s;
}

Splay * erase(Splay * p, const Key & key)
{
	Splay *left, *right;
	split(p, key, left, right);
	right = find_first(right);
	if(right && right->key == key && (right = right->r))
		right->p = nullptr;
	return join(left, right);
}

/* SIZE */
Splay * find_kth(Splay * p, int k) // indexed from 0
{
	Splay * q = nullptr;
	touch(p);
	while(p)
	{
		q = p;
		touch(p->l); touch(p->r);
		if(p->l && k < p->l->size) p = p->l;
		else if((k -= ((p->l) ? p->l->size : 0) + 1) == -1) break;
		else p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * find(Splay * p, const Key & key)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	if(q) splay(q);
	return q;
}

Splay * find_or_create(Splay * p, const Key & key)
{
	p = find(p, key);
	if(!p || Splay_comp(key, p->key, p) || Splay_comp(p->key, key, p))
		return insert(p, newSplay(key));
	return p;
}

/* Apllies a functiona on a tree representing the range [a, b) */
Splay * get_range(Splay * p, const Key & a, const Key & b, std::function<void(Splay*)> f)
{
	Splay *left, *middle, *right;
	split(p, a, left, middle);
	split(middle, b, middle, right);
	touch(middle);
	f(middle);
	return join(left, join(middle, right));
}


int main()
{
	int w, h, n;
	scanf("%d%d%d", &w, &h, &n);
	Splay *H = nullptr, *najH = nullptr, *V = nullptr, *najV = nullptr;
	H = insert(H, newSplay(0));
	H = insert(H, newSplay(h));
	najH = insert(najH, newSplay(h));
	V = insert(V, newSplay(0));
	V = insert(V, newSplay(w));
	najV = insert(najV, newSplay(w));
	while(n--)
	{
		char c; int x;
		scanf(" %c", &c);
		scanf("%d", &x);
		Splay * & A = (c == 'H') ? H : V;
		Splay * & najA = (c == 'H') ? najH : najV;
		A = insert(A, newSplay(x));
		A = find(A, x);
		auto nas = snext(A);
		auto pop = sprev(sprev(nas));
		najA = erase(najA, nas->key - pop->key);
		najA = insert(najA, newSplay(x - pop->key));
		najA = insert(najA, newSplay(nas->key - x));
		printf("usuwam %d\n", nas->key - pop->key);
		printf("dodaje1 %d\n", x - pop->key);
		printf("dodaje2 %d\n", nas->key - x);

		najH = find_last(najH);
		najV = find_last(najV);
		typedef long long int ll;
		printf("%lld\n", ((ll) najH->key) * (najV->key));
	}
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                  linkcut                                  */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

class Splay;

int getId(Splay * s);
void wypisz();

struct Value
{
	int size;
	int czarne;
	bool color;
	bool flop;

	Value() : size(1), czarne(false), color(

	void touch(Value * left, Value * right)
	{
		if(flop)
		{
			color = !color;
			czarne = size - czarne;
			flop = !flop;
			if(left) left->flop = !left->flop;
			if(right) right->flop = !right->flop;
		}
	}

	void update(Value * left, Value * right)
	{
		size = 1;
		czarne = color ? 1 : 0;
		if(left)
		{
			size += left->size;
			czarne += left->czarne;
		}
		if(right)
		{
			size += right->size;
			czarne += right->czarne;
		}
	}
};

struct Splay : Value
{
	Splay *l, *r, *p;
	bool flip;

	template<typename ...Args>
	Splay(const Args & ...args)
		: Value(args...)
		,l(nullptr) ,r(nullptr) ,p(nullptr)
		,flip(false)
	{
	}

	bool isRoot()
	{
		return !p || (p->l != this && p->r != this);
	}

	void touch()
	{
		if(flip)
		{
			swap(l, r);
			if(l) l->flip = !l->flip;
			if(r) r->flip = !r->flip;
			flip = false;
		}
		Value::touch(l, r);
	}

	void update()
	{
		Value::update(l, r);
	}

	static void connect(Splay * p, Splay * c, bool left)
	{
		(left ? p->l : p->r) = c;
		if(c) c->p = p;
	}

	/* Zakłada, że nie jest ,,rootem'' */
	void rotate()
	{
		Splay * f = p;
		Splay * t = f->p;
		f->touch();
		touch();
		bool isr = f->isRoot();
		bool left = (this == f->l);
		connect(f, left ? r : l, left);
		connect(this, f, !left);
		if(isr) this->p = t;
		else connect(t, this, f == t->l);
		if(f->r) f->r->touch();
		if(f->l) f->l->touch();
		f->update();
	}

	void splay()
	{
		while(!isRoot())
		{
			Splay * r = p->p;
			if(!p->isRoot()) (((p->l == this) == (r->l == p)) ? p : this)->rotate();
			rotate();
		}
		touch();
		if(l) l->touch();
		if(r) r->touch();
		update();
	}

	/* Podnosi wierzchołek do góry (będzie korzeniem splaya, w którym znajduje się korzeń drzewa :P). */
	static Splay * lastDisc;
	void expose()
	{
		Splay * r = nullptr;
		lastDisc = nullptr;
		for(Splay * q = this; q; q = q->p)
		{
			q->splay();
			q->r = r;
			q->update();
			r = q;
			lastDisc = q;
		}
		splay();
	}

	void link(Splay * to)
	{
		expose();
		assert(l == nullptr); // to powinien być root
		p = to;
	}

	Splay * root()
	{
		expose();
		Splay * s = this;
		while(s->l)
		{
			s = s->l;
			s->touch();
		}
		s->splay();
		return s;
	}

	/* Wywołaj funkcję f na poddrzwie reprezentującym ścieżkę od korzenia do wierzchołka. */
	void applyF(std::function<void(Splay*)> f)
	{
		expose();
		if(r == nullptr) f(this);
		else
		{
			Splay * s = r;
			while(s->l)
			{
				s = s->l;
				s->touch();
			}
			s->splay();
			f(s->l);
		}
	}

	void makeRoot()
	{
		applyF([] (Splay * s) {
			s->flip = !s->flip;
		});
	}

	void cut()
	{
		expose();
		assert(l != nullptr); // jest rootem, nie można obciąć...
		Splay * s = l;
		while(s->r) s = s->r;
		s->splay();
		s->r->p = nullptr;
		s->r = nullptr;
	}

	/* Zwraca lca(this, v) */
	Splay * lca(Splay * v)
	{
		expose();
		v->expose();
		return lastDisc;
	}

};

Splay * Splay::lastDisc;

/* Wywołaj funkcję f na poddrzewie reprezentującym ścieżkę od a do b. */
void applyF(Splay * a, Splay * b, std::function<void(Splay*)> f)
{
	a->makeRoot();
	b->applyF(f);
}

int nast = 0;
Splay spleje[100005];

template<typename ...Args>
Splay * newTree(const Args & ...args)
{
	return &spleje[nast++];
	//return new Splay(args...);
}

void deleteTree(Splay * t)
{
	if(t->l) deleteTree(t->l);
	if(t->r) deleteTree(t->r);
	delete t;
}

int n;
Splay * tab[100005];
map<long, int> ids;

int getId(Splay * s)
{
	return ids[(long) s];
}

void wypisz()
{
}

vector<int> graf[100005];

int dfs(int w, int o)
{
	for(int i : graf[w])
		if(i != o)
			dfs(i, w);
	if(o) tab[w]->link(tab[o]);
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 1; i <= n; i++)
		tab[i] = newTree();
	for(int i = 1; i < n; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		graf[a].emplace_back(b);
		graf[b].emplace_back(a);
	}
	dfs(1, 0);
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                   splay                                   */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

typedef int Key;

struct Splay
{
	Key key; // cena
	Splay *l, *r, *p;
	int size; /* SIZE */
	bool flip; /* FLIP */
	bool fliped; /* FLIP */

	Splay(const Key & key = -1) : key(key), l(nullptr), r(nullptr), p(nullptr), flip(false), fliped(false) {}
	#define Splay_comp(a, b, c) ((a < b) != c->fliped) /* FLIP */
	//#define Splay_comp(a, b, c) (a < b) /* ! FLIP */
};

/*
// Runs in O(n) time!!!!
Splay * root(Splay * p)
{
	while(p->p) p = p->p;
	return p;
}

void dajKey(Splay * p)
{
	if(p == nullptr) printf("(nil)");
	else printf("[%d]", p->key);
}

void wypisz(Splay * p)
{
	if(p == nullptr) return;
	printf("(key = %d, val = %d, najw = %d, spu = %d)", p->key, p->val, p->najw, p->spu);
	printf(" L: "); dajKey(p->l);
	printf(" R: "); dajKey(p->r);
	printf("\n");
	wypisz(p->l);
	wypisz(p->r);
}

void touch(Splay * n);

void dajPary(Splay * p)
{
	if(p == nullptr) return;
	touch(p);
	dajPary(p->l);
	printf(" (%d, %d)", p->key, p->val);
	dajPary(p->r);
}
*/

int used_allo = 0;
//vector<Splay> allo;
Splay allo[200005];

inline Splay * newSplay(const Key & key)
{
	//if(used_allo == (int) allo.size())
		//allo.resize(max(1, (int) allo.size()) * 2);
	allo[used_allo] = Splay(key);
	return &allo[used_allo++];
}

void touch(Splay * n)
{
	if(!n) return;
	/* FLIP */
	if(n->flip)
	{
		swap(n->l, n->r);
		if(n->l) n->l->flip = ! n->l->flip;
		if(n->r) n->r->flip = ! n->r->flip;
		n->flip = false;
		n->fliped = ! n->fliped;
	}
}

void update(Splay * n)
{
	if(!n) return;
	n->size = 1; /* SIZE */
	if(n->l) n->size += n->l->size; /* SIZE */
	if(n->r) n->size += n->r->size; /* SIZE */

	//assert(!(n->l) || !(n->l->spu));
	//assert(!(n->r) || !(n->r->spu));
}

void connect(Splay * p, Splay * s, bool left)
{
	(left ? p->l : p->r) = s;
	if(s) s->p = p;
}

void rotate(Splay * p)
{
	Splay * q = p->p;
	Splay * r = q->p;
	bool left = (p == q->l);
	touch(left ? q->r : q->l);
	connect(q, left ? p->r : p->l, left);
	connect(p, q, !left);
	if(r) connect(r, p, (r->l == q));
	else p->p = nullptr;
	update(q);
}

void splay(Splay * p)
{
	touch(p->l);
	touch(p->r);
	while(p->p)
	{
		Splay * q = p->p;
		Splay * r = q->p;
		touch((q->l == p) ? q->r : q->l);
		if(r) rotate(((q->l == p) == (r->l == q)) ? q : p);
		rotate(p);
	}
	update(p);
}

Splay * snext(Splay * p)
{
	if(!p || !p->r) return nullptr;
	p = p->r;
	while(p->l) p = p->l;
	splay(p);
	return p;
}

Splay * sprev(Splay * p)
{
	if(!p || !p->l) return nullptr;
	p = p->l;
	while(p->r) p = p->r;
	splay(p);
	return p;
}

Splay * find_first(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->l;
	}
	if(q) splay(q);
	return q;
}

Splay * find_last(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * join(Splay * l, Splay * r)
{
	if(!r) return l;
	r = find_first(r);
	touch(l);
	touch(r);
	connect(r, l, true);
	update(r);
	return r; }

// left < key, key <= right
void split(Splay * p, const Key & key, Splay * & left, Splay * & right)
{
	if(!p) { left = right = nullptr; return; }
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	splay(q);
	bool brig = Splay_comp(q->key, key, q);
	left = brig ? q : q->l;
	right = brig ? q->r : q;
	(brig ? q->r : q->l) = nullptr;
	if(left) left->p = nullptr;
	if(right) right->p = nullptr;
	update(q);
}

// dorzuca _wierzcholek_ s (wierzcholek, nie drzewo!!!)
Splay * insert(Splay * p, Splay * s)
{
	Splay *left, *right;
	split(p, s->key, left, right);
	right = find_first(right);
	if(right && !Splay_comp(s->key, right->key, right))
	{
		connect(s, right->l, true);
		connect(s, right->r, false);
		touch(right->l);
		touch(right->r);
		update(s);
		return join(left, s);
	}
	touch(left);
	touch(right);
	connect(s, left, true);
	connect(s, right, false);
	update(s);
	return s;
}

Splay * erase(Splay * p, const Key & key)
{
	Splay *left, *right;
	split(p, key, left, right);
	right = find_first(right);
	if(right && right->key == key && (right = right->r))
		right->p = nullptr;
	return join(left, right);
}

/* SIZE */
Splay * find_kth(Splay * p, int k) // indexed from 0
{
	Splay * q = nullptr;
	touch(p);
	while(p)
	{
		q = p;
		touch(p->l); touch(p->r);
		if(p->l && k < p->l->size) p = p->l;
		else if((k -= ((p->l) ? p->l->size : 0) + 1) == -1) break;
		else p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * find(Splay * p, const Key & key)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	if(q) splay(q);
	return q;
}

Splay * find_or_create(Splay * p, const Key & key)
{
	p = find(p, key);
	if(!p || Splay_comp(key, p->key, p) || Splay_comp(p->key, key, p))
		return insert(p, newSplay(key));
	return p;
}

/* Apllies a functiona on a tree representing the range [a, b) */
Splay * get_range(Splay * p, const Key & a, const Key & b, std::function<void(Splay*)> f)
{
	Splay *left, *middle, *right;
	split(p, a, left, middle);
	split(middle, b, middle, right);
	touch(middle);
	f(middle);
	return join(left, join(middle, right));
}


int main()
{
	int w, h, n;
	scanf("%d%d%d", &w, &h, &n);
	Splay *H = nullptr, *najH = nullptr, *V = nullptr, *najV = nullptr;
	H = insert(H, newSplay(0));
	H = insert(H, newSplay(h));
	najH = insert(najH, newSplay(h));
	V = insert(V, newSplay(0));
	V = insert(V, newSplay(w));
	najV = insert(najV, newSplay(w));
	while(n--)
	{
		char c; int x;
		scanf(" %c", &c);
		scanf("%d", &x);
		Splay * & A = (c == 'H') ? H : V;
		Splay * & najA = (c == 'H') ? najH : najV;
		A = insert(A, newSplay(x));
		A = find(A, x);
		auto nas = snext(A);
		auto pop = sprev(sprev(nas));
		najA = erase(najA, nas->key - pop->key);
		najA = insert(najA, newSplay(x - pop->key));
		najA = insert(najA, newSplay(nas->key - x));
		printf("usuwam %d\n", nas->key - pop->key);
		printf("dodaje1 %d\n", x - pop->key);
		printf("dodaje2 %d\n", nas->key - x);

		najH = find_last(najH);
		najV = find_last(najV);
		typedef long long int ll;
		printf("%lld\n", ((ll) najH->key) * (najV->key));
	}
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                  linkcut                                  */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

class Splay;

int getId(Splay * s);
void wypisz();

struct Value
{
	int size;
	int czarne;
	bool color;
	bool flop;

	Value() : size(1), czarne(false), color(

	void touch(Value * left, Value * right)
	{
		if(flop)
		{
			color = !color;
			czarne = size - czarne;
			flop = !flop;
			if(left) left->flop = !left->flop;
			if(right) right->flop = !right->flop;
		}
	}

	void update(Value * left, Value * right)
	{
		size = 1;
		czarne = color ? 1 : 0;
		if(left)
		{
			size += left->size;
			czarne += left->czarne;
		}
		if(right)
		{
			size += right->size;
			czarne += right->czarne;
		}
	}
};

struct Splay : Value
{
	Splay *l, *r, *p;
	bool flip;

	template<typename ...Args>
	Splay(const Args & ...args)
		: Value(args...)
		,l(nullptr) ,r(nullptr) ,p(nullptr)
		,flip(false)
	{
	}

	bool isRoot()
	{
		return !p || (p->l != this && p->r != this);
	}

	void touch()
	{
		if(flip)
		{
			swap(l, r);
			if(l) l->flip = !l->flip;
			if(r) r->flip = !r->flip;
			flip = false;
		}
		Value::touch(l, r);
	}

	void update()
	{
		Value::update(l, r);
	}

	static void connect(Splay * p, Splay * c, bool left)
	{
		(left ? p->l : p->r) = c;
		if(c) c->p = p;
	}

	/* Zakłada, że nie jest ,,rootem'' */
	void rotate()
	{
		Splay * f = p;
		Splay * t = f->p;
		f->touch();
		touch();
		bool isr = f->isRoot();
		bool left = (this == f->l);
		connect(f, left ? r : l, left);
		connect(this, f, !left);
		if(isr) this->p = t;
		else connect(t, this, f == t->l);
		if(f->r) f->r->touch();
		if(f->l) f->l->touch();
		f->update();
	}

	void splay()
	{
		while(!isRoot())
		{
			Splay * r = p->p;
			if(!p->isRoot()) (((p->l == this) == (r->l == p)) ? p : this)->rotate();
			rotate();
		}
		touch();
		if(l) l->touch();
		if(r) r->touch();
		update();
	}

	/* Podnosi wierzchołek do góry (będzie korzeniem splaya, w którym znajduje się korzeń drzewa :P). */
	static Splay * lastDisc;
	void expose()
	{
		Splay * r = nullptr;
		lastDisc = nullptr;
		for(Splay * q = this; q; q = q->p)
		{
			q->splay();
			q->r = r;
			q->update();
			r = q;
			lastDisc = q;
		}
		splay();
	}

	void link(Splay * to)
	{
		expose();
		assert(l == nullptr); // to powinien być root
		p = to;
	}

	Splay * root()
	{
		expose();
		Splay * s = this;
		while(s->l)
		{
			s = s->l;
			s->touch();
		}
		s->splay();
		return s;
	}

	/* Wywołaj funkcję f na poddrzwie reprezentującym ścieżkę od korzenia do wierzchołka. */
	void applyF(std::function<void(Splay*)> f)
	{
		expose();
		if(r == nullptr) f(this);
		else
		{
			Splay * s = r;
			while(s->l)
			{
				s = s->l;
				s->touch();
			}
			s->splay();
			f(s->l);
		}
	}

	void makeRoot()
	{
		applyF([] (Splay * s) {
			s->flip = !s->flip;
		});
	}

	void cut()
	{
		expose();
		assert(l != nullptr); // jest rootem, nie można obciąć...
		Splay * s = l;
		while(s->r) s = s->r;
		s->splay();
		s->r->p = nullptr;
		s->r = nullptr;
	}

	/* Zwraca lca(this, v) */
	Splay * lca(Splay * v)
	{
		expose();
		v->expose();
		return lastDisc;
	}

};

Splay * Splay::lastDisc;

/* Wywołaj funkcję f na poddrzewie reprezentującym ścieżkę od a do b. */
void applyF(Splay * a, Splay * b, std::function<void(Splay*)> f)
{
	a->makeRoot();
	b->applyF(f);
}

int nast = 0;
Splay spleje[100005];

template<typename ...Args>
Splay * newTree(const Args & ...args)
{
	return &spleje[nast++];
	//return new Splay(args...);
}

void deleteTree(Splay * t)
{
	if(t->l) deleteTree(t->l);
	if(t->r) deleteTree(t->r);
	delete t;
}

int n;
Splay * tab[100005];
map<long, int> ids;

int getId(Splay * s)
{
	return ids[(long) s];
}

void wypisz()
{
}

vector<int> graf[100005];

int dfs(int w, int o)
{
	for(int i : graf[w])
		if(i != o)
			dfs(i, w);
	if(o) tab[w]->link(tab[o]);
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 1; i <= n; i++)
		tab[i] = newTree();
	for(int i = 1; i < n; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		graf[a].emplace_back(b);
		graf[b].emplace_back(a);
	}
	dfs(1, 0);
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                   splay                                   */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

typedef int Key;

struct Splay
{
	Key key; // cena
	Splay *l, *r, *p;
	int size; /* SIZE */
	bool flip; /* FLIP */
	bool fliped; /* FLIP */

	Splay(const Key & key = -1) : key(key), l(nullptr), r(nullptr), p(nullptr), flip(false), fliped(false) {}
	#define Splay_comp(a, b, c) ((a < b) != c->fliped) /* FLIP */
	//#define Splay_comp(a, b, c) (a < b) /* ! FLIP */
};

/*
// Runs in O(n) time!!!!
Splay * root(Splay * p)
{
	while(p->p) p = p->p;
	return p;
}

void dajKey(Splay * p)
{
	if(p == nullptr) printf("(nil)");
	else printf("[%d]", p->key);
}

void wypisz(Splay * p)
{
	if(p == nullptr) return;
	printf("(key = %d, val = %d, najw = %d, spu = %d)", p->key, p->val, p->najw, p->spu);
	printf(" L: "); dajKey(p->l);
	printf(" R: "); dajKey(p->r);
	printf("\n");
	wypisz(p->l);
	wypisz(p->r);
}

void touch(Splay * n);

void dajPary(Splay * p)
{
	if(p == nullptr) return;
	touch(p);
	dajPary(p->l);
	printf(" (%d, %d)", p->key, p->val);
	dajPary(p->r);
}
*/

int used_allo = 0;
//vector<Splay> allo;
Splay allo[200005];

inline Splay * newSplay(const Key & key)
{
	//if(used_allo == (int) allo.size())
		//allo.resize(max(1, (int) allo.size()) * 2);
	allo[used_allo] = Splay(key);
	return &allo[used_allo++];
}

void touch(Splay * n)
{
	if(!n) return;
	/* FLIP */
	if(n->flip)
	{
		swap(n->l, n->r);
		if(n->l) n->l->flip = ! n->l->flip;
		if(n->r) n->r->flip = ! n->r->flip;
		n->flip = false;
		n->fliped = ! n->fliped;
	}
}

void update(Splay * n)
{
	if(!n) return;
	n->size = 1; /* SIZE */
	if(n->l) n->size += n->l->size; /* SIZE */
	if(n->r) n->size += n->r->size; /* SIZE */

	//assert(!(n->l) || !(n->l->spu));
	//assert(!(n->r) || !(n->r->spu));
}

void connect(Splay * p, Splay * s, bool left)
{
	(left ? p->l : p->r) = s;
	if(s) s->p = p;
}

void rotate(Splay * p)
{
	Splay * q = p->p;
	Splay * r = q->p;
	bool left = (p == q->l);
	touch(left ? q->r : q->l);
	connect(q, left ? p->r : p->l, left);
	connect(p, q, !left);
	if(r) connect(r, p, (r->l == q));
	else p->p = nullptr;
	update(q);
}

void splay(Splay * p)
{
	touch(p->l);
	touch(p->r);
	while(p->p)
	{
		Splay * q = p->p;
		Splay * r = q->p;
		touch((q->l == p) ? q->r : q->l);
		if(r) rotate(((q->l == p) == (r->l == q)) ? q : p);
		rotate(p);
	}
	update(p);
}

Splay * snext(Splay * p)
{
	if(!p || !p->r) return nullptr;
	p = p->r;
	while(p->l) p = p->l;
	splay(p);
	return p;
}

Splay * sprev(Splay * p)
{
	if(!p || !p->l) return nullptr;
	p = p->l;
	while(p->r) p = p->r;
	splay(p);
	return p;
}

Splay * find_first(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->l;
	}
	if(q) splay(q);
	return q;
}

Splay * find_last(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * join(Splay * l, Splay * r)
{
	if(!r) return l;
	r = find_first(r);
	touch(l);
	touch(r);
	connect(r, l, true);
	update(r);
	return r; }

// left < key, key <= right
void split(Splay * p, const Key & key, Splay * & left, Splay * & right)
{
	if(!p) { left = right = nullptr; return; }
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	splay(q);
	bool brig = Splay_comp(q->key, key, q);
	left = brig ? q : q->l;
	right = brig ? q->r : q;
	(brig ? q->r : q->l) = nullptr;
	if(left) left->p = nullptr;
	if(right) right->p = nullptr;
	update(q);
}

// dorzuca _wierzcholek_ s (wierzcholek, nie drzewo!!!)
Splay * insert(Splay * p, Splay * s)
{
	Splay *left, *right;
	split(p, s->key, left, right);
	right = find_first(right);
	if(right && !Splay_comp(s->key, right->key, right))
	{
		connect(s, right->l, true);
		connect(s, right->r, false);
		touch(right->l);
		touch(right->r);
		update(s);
		return join(left, s);
	}
	touch(left);
	touch(right);
	connect(s, left, true);
	connect(s, right, false);
	update(s);
	return s;
}

Splay * erase(Splay * p, const Key & key)
{
	Splay *left, *right;
	split(p, key, left, right);
	right = find_first(right);
	if(right && right->key == key && (right = right->r))
		right->p = nullptr;
	return join(left, right);
}

/* SIZE */
Splay * find_kth(Splay * p, int k) // indexed from 0
{
	Splay * q = nullptr;
	touch(p);
	while(p)
	{
		q = p;
		touch(p->l); touch(p->r);
		if(p->l && k < p->l->size) p = p->l;
		else if((k -= ((p->l) ? p->l->size : 0) + 1) == -1) break;
		else p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * find(Splay * p, const Key & key)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	if(q) splay(q);
	return q;
}

Splay * find_or_create(Splay * p, const Key & key)
{
	p = find(p, key);
	if(!p || Splay_comp(key, p->key, p) || Splay_comp(p->key, key, p))
		return insert(p, newSplay(key));
	return p;
}

/* Apllies a functiona on a tree representing the range [a, b) */
Splay * get_range(Splay * p, const Key & a, const Key & b, std::function<void(Splay*)> f)
{
	Splay *left, *middle, *right;
	split(p, a, left, middle);
	split(middle, b, middle, right);
	touch(middle);
	f(middle);
	return join(left, join(middle, right));
}


int main()
{
	int w, h, n;
	scanf("%d%d%d", &w, &h, &n);
	Splay *H = nullptr, *najH = nullptr, *V = nullptr, *najV = nullptr;
	H = insert(H, newSplay(0));
	H = insert(H, newSplay(h));
	najH = insert(najH, newSplay(h));
	V = insert(V, newSplay(0));
	V = insert(V, newSplay(w));
	najV = insert(najV, newSplay(w));
	while(n--)
	{
		char c; int x;
		scanf(" %c", &c);
		scanf("%d", &x);
		Splay * & A = (c == 'H') ? H : V;
		Splay * & najA = (c == 'H') ? najH : najV;
		A = insert(A, newSplay(x));
		A = find(A, x);
		auto nas = snext(A);
		auto pop = sprev(sprev(nas));
		najA = erase(najA, nas->key - pop->key);
		najA = insert(najA, newSplay(x - pop->key));
		najA = insert(najA, newSplay(nas->key - x));
		printf("usuwam %d\n", nas->key - pop->key);
		printf("dodaje1 %d\n", x - pop->key);
		printf("dodaje2 %d\n", nas->key - x);

		najH = find_last(najH);
		najV = find_last(najV);
		typedef long long int ll;
		printf("%lld\n", ((ll) najH->key) * (najV->key));
	}
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                  linkcut                                  */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

class Splay;

int getId(Splay * s);
void wypisz();

struct Value
{
	int size;
	int czarne;
	bool color;
	bool flop;

	Value() : size(1), czarne(false), color(

	void touch(Value * left, Value * right)
	{
		if(flop)
		{
			color = !color;
			czarne = size - czarne;
			flop = !flop;
			if(left) left->flop = !left->flop;
			if(right) right->flop = !right->flop;
		}
	}

	void update(Value * left, Value * right)
	{
		size = 1;
		czarne = color ? 1 : 0;
		if(left)
		{
			size += left->size;
			czarne += left->czarne;
		}
		if(right)
		{
			size += right->size;
			czarne += right->czarne;
		}
	}
};

struct Splay : Value
{
	Splay *l, *r, *p;
	bool flip;

	template<typename ...Args>
	Splay(const Args & ...args)
		: Value(args...)
		,l(nullptr) ,r(nullptr) ,p(nullptr)
		,flip(false)
	{
	}

	bool isRoot()
	{
		return !p || (p->l != this && p->r != this);
	}

	void touch()
	{
		if(flip)
		{
			swap(l, r);
			if(l) l->flip = !l->flip;
			if(r) r->flip = !r->flip;
			flip = false;
		}
		Value::touch(l, r);
	}

	void update()
	{
		Value::update(l, r);
	}

	static void connect(Splay * p, Splay * c, bool left)
	{
		(left ? p->l : p->r) = c;
		if(c) c->p = p;
	}

	/* Zakłada, że nie jest ,,rootem'' */
	void rotate()
	{
		Splay * f = p;
		Splay * t = f->p;
		f->touch();
		touch();
		bool isr = f->isRoot();
		bool left = (this == f->l);
		connect(f, left ? r : l, left);
		connect(this, f, !left);
		if(isr) this->p = t;
		else connect(t, this, f == t->l);
		if(f->r) f->r->touch();
		if(f->l) f->l->touch();
		f->update();
	}

	void splay()
	{
		while(!isRoot())
		{
			Splay * r = p->p;
			if(!p->isRoot()) (((p->l == this) == (r->l == p)) ? p : this)->rotate();
			rotate();
		}
		touch();
		if(l) l->touch();
		if(r) r->touch();
		update();
	}

	/* Podnosi wierzchołek do góry (będzie korzeniem splaya, w którym znajduje się korzeń drzewa :P). */
	static Splay * lastDisc;
	void expose()
	{
		Splay * r = nullptr;
		lastDisc = nullptr;
		for(Splay * q = this; q; q = q->p)
		{
			q->splay();
			q->r = r;
			q->update();
			r = q;
			lastDisc = q;
		}
		splay();
	}

	void link(Splay * to)
	{
		expose();
		assert(l == nullptr); // to powinien być root
		p = to;
	}

	Splay * root()
	{
		expose();
		Splay * s = this;
		while(s->l)
		{
			s = s->l;
			s->touch();
		}
		s->splay();
		return s;
	}

	/* Wywołaj funkcję f na poddrzwie reprezentującym ścieżkę od korzenia do wierzchołka. */
	void applyF(std::function<void(Splay*)> f)
	{
		expose();
		if(r == nullptr) f(this);
		else
		{
			Splay * s = r;
			while(s->l)
			{
				s = s->l;
				s->touch();
			}
			s->splay();
			f(s->l);
		}
	}

	void makeRoot()
	{
		applyF([] (Splay * s) {
			s->flip = !s->flip;
		});
	}

	void cut()
	{
		expose();
		assert(l != nullptr); // jest rootem, nie można obciąć...
		Splay * s = l;
		while(s->r) s = s->r;
		s->splay();
		s->r->p = nullptr;
		s->r = nullptr;
	}

	/* Zwraca lca(this, v) */
	Splay * lca(Splay * v)
	{
		expose();
		v->expose();
		return lastDisc;
	}

};

Splay * Splay::lastDisc;

/* Wywołaj funkcję f na poddrzewie reprezentującym ścieżkę od a do b. */
void applyF(Splay * a, Splay * b, std::function<void(Splay*)> f)
{
	a->makeRoot();
	b->applyF(f);
}

int nast = 0;
Splay spleje[100005];

template<typename ...Args>
Splay * newTree(const Args & ...args)
{
	return &spleje[nast++];
	//return new Splay(args...);
}

void deleteTree(Splay * t)
{
	if(t->l) deleteTree(t->l);
	if(t->r) deleteTree(t->r);
	delete t;
}

int n;
Splay * tab[100005];
map<long, int> ids;

int getId(Splay * s)
{
	return ids[(long) s];
}

void wypisz()
{
}

vector<int> graf[100005];

int dfs(int w, int o)
{
	for(int i : graf[w])
		if(i != o)
			dfs(i, w);
	if(o) tab[w]->link(tab[o]);
}

int main()
{
	int n;
	scanf("%d", &n);
	for(int i = 1; i <= n; i++)
		tab[i] = newTree();
	for(int i = 1; i < n; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		graf[a].emplace_back(b);
		graf[b].emplace_back(a);
	}
	dfs(1, 0);
	return 0;
}
/*****************************************************************************/
/*                                                                           */
/*                                   splay                                   */
/*                                                                           */
/*****************************************************************************/
#include <bits/stdc++.h>

using namespace std;

typedef int Key;

struct Splay
{
	Key key; // cena
	Splay *l, *r, *p;
	int size; /* SIZE */
	bool flip; /* FLIP */
	bool fliped; /* FLIP */

	Splay(const Key & key = -1) : key(key), l(nullptr), r(nullptr), p(nullptr), flip(false), fliped(false) {}
	#define Splay_comp(a, b, c) ((a < b) != c->fliped) /* FLIP */
	//#define Splay_comp(a, b, c) (a < b) /* ! FLIP */
};

/*
// Runs in O(n) time!!!!
Splay * root(Splay * p)
{
	while(p->p) p = p->p;
	return p;
}

void dajKey(Splay * p)
{
	if(p == nullptr) printf("(nil)");
	else printf("[%d]", p->key);
}

void wypisz(Splay * p)
{
	if(p == nullptr) return;
	printf("(key = %d, val = %d, najw = %d, spu = %d)", p->key, p->val, p->najw, p->spu);
	printf(" L: "); dajKey(p->l);
	printf(" R: "); dajKey(p->r);
	printf("\n");
	wypisz(p->l);
	wypisz(p->r);
}

void touch(Splay * n);

void dajPary(Splay * p)
{
	if(p == nullptr) return;
	touch(p);
	dajPary(p->l);
	printf(" (%d, %d)", p->key, p->val);
	dajPary(p->r);
}
*/

int used_allo = 0;
//vector<Splay> allo;
Splay allo[200005];

inline Splay * newSplay(const Key & key)
{
	//if(used_allo == (int) allo.size())
		//allo.resize(max(1, (int) allo.size()) * 2);
	allo[used_allo] = Splay(key);
	return &allo[used_allo++];
}

void touch(Splay * n)
{
	if(!n) return;
	/* FLIP */
	if(n->flip)
	{
		swap(n->l, n->r);
		if(n->l) n->l->flip = ! n->l->flip;
		if(n->r) n->r->flip = ! n->r->flip;
		n->flip = false;
		n->fliped = ! n->fliped;
	}
}

void update(Splay * n)
{
	if(!n) return;
	n->size = 1; /* SIZE */
	if(n->l) n->size += n->l->size; /* SIZE */
	if(n->r) n->size += n->r->size; /* SIZE */

	//assert(!(n->l) || !(n->l->spu));
	//assert(!(n->r) || !(n->r->spu));
}

void connect(Splay * p, Splay * s, bool left)
{
	(left ? p->l : p->r) = s;
	if(s) s->p = p;
}

void rotate(Splay * p)
{
	Splay * q = p->p;
	Splay * r = q->p;
	bool left = (p == q->l);
	touch(left ? q->r : q->l);
	connect(q, left ? p->r : p->l, left);
	connect(p, q, !left);
	if(r) connect(r, p, (r->l == q));
	else p->p = nullptr;
	update(q);
}

void splay(Splay * p)
{
	touch(p->l);
	touch(p->r);
	while(p->p)
	{
		Splay * q = p->p;
		Splay * r = q->p;
		touch((q->l == p) ? q->r : q->l);
		if(r) rotate(((q->l == p) == (r->l == q)) ? q : p);
		rotate(p);
	}
	update(p);
}

Splay * snext(Splay * p)
{
	if(!p || !p->r) return nullptr;
	p = p->r;
	while(p->l) p = p->l;
	splay(p);
	return p;
}

Splay * sprev(Splay * p)
{
	if(!p || !p->l) return nullptr;
	p = p->l;
	while(p->r) p = p->r;
	splay(p);
	return p;
}

Splay * find_first(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->l;
	}
	if(q) splay(q);
	return q;
}

Splay * find_last(Splay * p)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * join(Splay * l, Splay * r)
{
	if(!r) return l;
	r = find_first(r);
	touch(l);
	touch(r);
	connect(r, l, true);
	update(r);
	return r; }

// left < key, key <= right
void split(Splay * p, const Key & key, Splay * & left, Splay * & right)
{
	if(!p) { left = right = nullptr; return; }
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	splay(q);
	bool brig = Splay_comp(q->key, key, q);
	left = brig ? q : q->l;
	right = brig ? q->r : q;
	(brig ? q->r : q->l) = nullptr;
	if(left) left->p = nullptr;
	if(right) right->p = nullptr;
	update(q);
}

// dorzuca _wierzcholek_ s (wierzcholek, nie drzewo!!!)
Splay * insert(Splay * p, Splay * s)
{
	Splay *left, *right;
	split(p, s->key, left, right);
	right = find_first(right);
	if(right && !Splay_comp(s->key, right->key, right))
	{
		connect(s, right->l, true);
		connect(s, right->r, false);
		touch(right->l);
		touch(right->r);
		update(s);
		return join(left, s);
	}
	touch(left);
	touch(right);
	connect(s, left, true);
	connect(s, right, false);
	update(s);
	return s;
}

Splay * erase(Splay * p, const Key & key)
{
	Splay *left, *right;
	split(p, key, left, right);
	right = find_first(right);
	if(right && right->key == key && (right = right->r))
		right->p = nullptr;
	return join(left, right);
}

/* SIZE */
Splay * find_kth(Splay * p, int k) // indexed from 0
{
	Splay * q = nullptr;
	touch(p);
	while(p)
	{
		q = p;
		touch(p->l); touch(p->r);
		if(p->l && k < p->l->size) p = p->l;
		else if((k -= ((p->l) ? p->l->size : 0) + 1) == -1) break;
		else p = p->r;
	}
	if(q) splay(q);
	return q;
}

Splay * find(Splay * p, const Key & key)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay_comp(key, p->key, p)) p = p->l;
		else if(Splay_comp(p->key, key, p)) p = p->r;
		else break;
	}
	if(q) splay(q);
	return q;
}

Splay * find_or_create(Splay * p, const Key & key)
{
	p = find(p, key);
	if(!p || Splay_comp(key, p->key, p) || Splay_comp(p->key, key, p))
		return insert(p, newSplay(key));
	return p;
}

/* Apllies a functiona on a tree representing the range [a, b) */
Splay * get_range(Splay * p, const Key & a, const Key & b, std::function<void(Splay*)> f)
{
	Splay *left, *middle, *right;
	split(p, a, left, middle);
	split(middle, b, middle, right);
	touch(middle);
	f(middle);
	return join(left, join(middle, right));
}


int main()
{
	int w, h, n;
	scanf("%d%d%d", &w, &h, &n);
	Splay *H = nullptr, *najH = nullptr, *V = nullptr, *najV = nullptr;
	H = insert(H, newSplay(0));
	H = insert(H, newSplay(h));
	najH = insert(najH, newSplay(h));
	V = insert(V, newSplay(0));
	V = insert(V, newSplay(w));
	najV = insert(najV, newSplay(w));
	while(n--)
	{
		char c; int x;
		scanf(" %c", &c);
		scanf("%d", &x);
		Splay * & A = (c == 'H') ? H : V;
		Splay * & najA = (c == 'H') ? najH : najV;
		A = insert(A, newSplay(x));
		A = find(A, x);
		auto nas = snext(A);
		auto pop = sprev(sprev(nas));
		najA = erase(najA, nas->key - pop->key);
		najA = insert(najA, newSplay(x - pop->key));
		najA = insert(najA, newSplay(nas->key - x));
		printf("usuwam %d\n", nas->key - pop->key);
		printf("dodaje1 %d\n", x - pop->key);
		printf("dodaje2 %d\n", nas->key - x);

		najH = find_last(najH);
		najV = find_last(najV);
		typedef long long int ll;
		printf("%lld\n", ((ll) najH->key) * (najV->key));
	}
	return 0;
}
