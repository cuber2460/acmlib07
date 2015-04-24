struct Value
{
	void touch(Value * left, Value * right) { }
	void update(Value * left, Value * right) { }
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

	/* Sprawia, że wierzchołek jest rootem w swoim drzewie. */
	void makeRoot()
	{
		applyF([] (Splay * s) {
			s->flip = !s->flip;
		});
	}

	/* Zakłada, że (*this) nie jest korzeniem drzewa.
	 * Usuwa krawędź znajdującą się nad danym wierzchołkiem. */
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

/* Wywołaj funkcję f na poddrzewie reprezentującym ścieżkę od a do b.
 * UWAGA! zmienia korzeń drzewa.
 * Jeśli jest to niepoążdane, po wywołaniu funkcji należy zrobić makeRoot() na starym roocie. */
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
