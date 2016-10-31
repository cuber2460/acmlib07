typedef int Key; /* Potrzebuje operatora < */
const bool SplayMulti = true; /* Czy równe klucze mogą być na drzewie? */

struct Value
{
	void touch(Value * left, Value * right) { }
	void update(Value * left, Value * right) { }
};

struct Splay : Value
{
	Key key;
	Splay *l, *r, *p;
	int size; /* SIZE */

	template<typename ...Args>
	Splay(const Key & key, const Args & ...args)
		: Value(args...) ,key(key)
		,l(nullptr) ,r(nullptr) ,p(nullptr)
		,size(1) /* SIZE */
	{
	}

	static bool comp(const Key & a, const Key & b)
	{
		return a < b;
	}
};

/* Splay allocator */
//Splay allo[200005]; int used_allo = 0; /* FIXED SIZE */
template<typename ...Args>
Splay * newSplay(const Key & key, const Args & ...args)
{
	return new Splay(key, args...); /* DYNAMIC ALLOCATION */
	//return &(allo[used_allo++] = Splay(key, args...));
}

/* Splay deallocator */
void deleteSplay(Splay * p)
{
	delete p;
}

void touch(Splay * n)
{
	if(!n) return;
}

void update(Splay * n)
{
	if(!n) return;
	n->update(n->l, n->r);
	n->size = 1; /* SIZE */
	if(n->l) n->size += n->l->size; /* SIZE */
	if(n->r) n->size += n->r->size; /* SIZE */
}

/* Tworzy krawędź p->s, left określa, czy jest to prawa, czy lewa krawędź */
void connect(Splay * p, Splay * s, bool left)
{
	(left ? p->l : p->r) = s;
	if(s) s->p = p;
}

/* Wykonuje rotację względem wierzchołka p (zakłada, że p ma ojca) */
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

/* Przenosi element p do korzenia */
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

/* Zwraca splay, w którego korzeniu znajduje się element następujący po korzeniu drzewa p
 * Jeśli taki element nie istnieje, zwraca nullptr */
Splay * snext(Splay * p)
{
	if(!p || !p->r) return nullptr;
	p = p->r;
	while(p->l) p = p->l;
	splay(p);
	return p;
}

/* Zwraca splay, w którego korzeniu znajduje się element poprzedziający korzeń drzewa p
 * Jeśli taki element nie istnieje, zwraca nullptr */
Splay * sprev(Splay * p)
{
	if(!p || !p->l) return nullptr;
	p = p->l;
	while(p->r) p = p->r;
	splay(p);
	return p;
}

/* Zwraca następny wierzchołek, nie zmieniając korzenia.
 * Jeśli taki element nie istnieje, zwraca nullptr */
Splay * getnext(Splay * p)
{
	if(!p) return nullptr;
	Splay * s = snext(p);
	splay(p);
	return s;
}

/* Zwraca poprzedni wierzchołek, nie zmieniając korzenia.
 * Jeśli taki element nie istnieje, zwraca nullptr */
Splay * getprev(Splay * p)
{
	if(!p) return nullptr;
	Splay * s = sprev(p);
	splay(p);
	return s;
}

/* Znajduje najmniejszy element na drzewie i umieszcza go w korzeniu */
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

/* Znajduje największy element na drzewie i umieszcza go w korzeniu */
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

/* Przenosi do korzenia element o kluczu key.
 * Jeśli taki nie istnieje, przenosci któregoś z sąsiadów (nie wiadomo, którego) */
Splay * find(Splay * p, const Key & key)
{
	Splay * q = nullptr;
	while(p)
	{
		touch(q = p);
		if(Splay::comp(key, p->key)) p = p->l;
		else if(Splay::comp(p->key, key)) p = p->r;
		else break;
	}
	if(q) splay(q);
	return q;
}

/* Łączy dwa drzewa l i r w jedno drzewo.
 * Zakłada, że największa wartość w drzewie l <= najmniejsza wartosć w drzewie r */
Splay * join(Splay * l, Splay * r)
{
	if(!r) return l;
	r = find_first(r);
	touch(l);
	touch(r);
	connect(r, l, true);
	update(r);
	return r;
}

/* Rozdziela drzewo p na dwa poddrzewa:
 * left:  (-oo, key)
 * right: [key, +oo)
 */
void split(Splay * p, const Key & key, Splay * & left, Splay * & right)
{
	if(!p) { left = right = nullptr; return; }
	p = find(p, key);
	bool brig = Splay::comp(p->key, key);
	left = brig ? p : p->l;
	right = brig ? p->r : p;
	(brig ? p->r : p->l) = nullptr;
	if(left) left->p = nullptr;
	if(right) right->p = nullptr;
	update(p);
}

/* Dodaje element (key, args...) do drzewa i umieszcza go w korzeniu.
 * Jeśli SplayMulti == false (czyli powtórzone klucze są niedopuszczone), a key znajduje się na drzewie,
 *   to ten element zostanie nadpisany (zarówno key jak i args...).
 * Jeśli SplayMulti == true, to element zostanie dorzucony przed inne elementy o tym samym kluczu. */
template<typename ...Args>
Splay * insert(Splay * p, const Key & key, const Args & ...args)
{
	Splay *left, *right;
	split(p, key, left, right);
	if(!SplayMulti && (right = find_first(right)) && !Splay::comp(key, right->key))
	{
		touch(right);
		Splay * l = right->l;
		Splay * r = right->r;
		*right = Splay(key, args...);
		connect(right, l, true);
		connect(right, r, false);
		touch(right->l);
		touch(right->r);
		update(right);
		return find(join(left, right), key);
	}
	Splay * s = newSplay(key, args...);
	connect(s, left, true);
	connect(s, right, false);
	touch(left);
	touch(right);
	update(s);
	return s;
}

/* Usuwa korzeń drzewa p i zwraca korzeń nowego drzewa. */
Splay * erase(Splay * p)
{
	assert(p != nullptr);
	splay(p);
	if(p->l) p->l->p = nullptr;
	if(p->r) p->r->p = nullptr;
	return join(p->l, p->r);
}

/* Usuwa _jeden_ element o kluczu key z drzewa (lub zero jeśli takich elementów nie ma)
 * Jeśli er != nullptr, to zapisana tam będzie informacja, czy jakiś element został usunięty */
Splay * erase(Splay * p, const Key & key, bool * er = nullptr)
{
	if(er) *er = false; /* ER */
	Splay *left, *right;
	split(p, key, left, right);
	right = find_first(right);
	if(right && !Splay::comp(key, right->key))
	{
		Splay * r = right->r;
		if(r) r->p = nullptr;
		deleteSplay(right);
		if(er) *er = true; /* ER */
		right = r;
	}
	return join(left, right);
}

/* Usuwa wszystkie elementy o kluczu key z drzewa
 * Jeśli SplayMulti == false, to funkcja jest równoważna funkcji erase.
 * Jeśli cnt != nullptr, to zapisana tam będzie informacja ile elementów zostało usuniętych. */
Splay * eraseAll(Splay * p, const Key & key, int * cnt = nullptr)
{
	if(cnt) *cnt = 0; /* CNT */
	bool x;
	do
	{
		p = erase(p, key, &x);
		if(cnt) (*cnt)++; /* CNT */
	}
	while(x);
	return p;
}

/* SIZE */
/* Przesuń k-ty element do korzenia.
 * Jeśli na drzewie nie ma aż k elementów, to przenieś ostatni. */
Splay * find_kth(Splay * p, int k) // k indeksowane od 0
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

/* Jeśli istnieje element o kluczu key, to przenosi go do korzenia
 * Jeśli taki element nie istnieje, to go tworzy i przenosi do korzenia */
template<typename ...Args>
Splay * find_or_create(Splay * p, const Key & key, const Args & ...args)
{
	p = find(p, key);
	if(!p || Splay::comp(key, p->key) || Splay::comp(p->key, key))
		return insert(p, key, args...);
	return p;
}

/* Wycina poddrzewo reprezentujące przedział [a, b) i wywołuje na nim funkcję f.
 * Funkcja f może dowolnie zmodyfikować dane jej poddrzewo (oby tylko klucze nie uciekły poza przedział [a, b)),
 *   a następnie zwrócić wskaźnik na nowe poddrzewo.
 * Na koniec całe drzewo jest z powrotem sklejane. */
Splay * get_range(Splay * p, const Key & a, const Key & b, std::function<Splay*(Splay*)> f)
{
	Splay *left, *middle, *right;
	split(p, a, left, middle);
	split(middle, b, middle, right);
	touch(middle);
	return join(left, join(f(middle), right));
}
