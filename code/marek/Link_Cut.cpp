struct Value {
  void touch(Value* left, Value* right) {}
  void update(Value* left, Value* right) {}
};

struct Splay : Value {
  Splay *l, *r, *p;
  bool flip;

  template<typename ...Args>
  Splay(const Args& ...args)
    : Value(args...), l(nullptr), r(nullptr), p(nullptr), flip(false) {}

  // Sprawdza, czy wierzchołek jest rootem splejowym.
  bool isRoot() { return !p or (p->l != this and p->r != this); }

  void update() { Value::update(l, r); }

  void touch() {
    if (flip) {
      swap(l, r);
      if (l) l->flip = !l->flip;
      if (r) r->flip = !r->flip;
      flip = false;
    }
    Value::touch(l, r);
  }

  static void connect(Splay* p, Splay* c, bool left) {
    (left ? p->l : p->r) = c;
    if (c) c->p = p;
  }

  void rotate() {
    Splay* f = p;
    Splay* t = f->p;
    f->touch();
    touch();
    bool isr = f->isRoot();
    bool left = (this == f->l);
    connect(f, left ? r : l, left);
    connect(this, f, !left);
    if (isr) this->p = t;
    else connect(t, this, f == t->l);
    if (f->r) f->r->touch();
    if (f->l) f->l->touch();
    f->update();
  }

  void splay() {
    while (!isRoot()) {
      Splay* x = p->p;
      if (!p->isRoot()) (((p->l == this) == (x->l == p)) ? p : this)->rotate();
      rotate();
    }
    touch();
    if (l) l->touch();
    if (r) r->touch();
    update();
  }

  static Splay* lastDisc;
  void expose() {
    Splay* x = nullptr;
    lastDisc = nullptr;
    for (Splay* q = this; q; q = q->p) {
      q->splay();
      q->r = x;
      q->update();
      x = q;
      lastDisc = q;
    }
    splay();
  }

  void link(Splay* to) {
    expose();
    assert(l == nullptr);  // to powinien być root
    p = to;
  }

  // Zwraca roota drzewowego (nie splejowego!).
  Splay* root() {
    expose();
    Splay* s = this;
    while (s->l) {
      s = s->l;
      s->touch();
    }
    s->splay();
    return s;
  }

  // Wywołaj funkcję f na poddrzwie reprezentującym ścieżkę od korzenia
  // do wierzchołka. Aby wywołać f na dowolnej ścieżce a-b, wystarczy napisać:
  // Splay* old = a->root(); a->makeRoot(); b->applyF(f); old->makeRoot();
  // Jeśli root jest bez znaczenia, można pominąć zmienną old.
  void applyF(std::function<void(Splay*)> f) {
    expose();
    if (r == nullptr) {
      f(this);
    } else {
      Splay* s = r;
      while (s->l) {
        s = s->l;
        s->touch();
      }
      s->splay();
      f(s->l);
    }
  }

  // Sprawia, że wierzchołek jest rootem w swoim drzewie.
  void makeRoot() {
    applyF([](Splay* s) {
      s->flip = !s->flip;
    });
  }

  // Zakłada, że (*this) nie jest korzeniem drzewa.
  // Usuwa krawędź znajdującą się nad danym wierzchołkiem.
  void cut() {
    expose();
    assert(l != nullptr);  // jest rootem, nie można obciąć...
    Splay* s = l;
    while (s->r) s = s->r;
    s->splay();
    s->r->p = nullptr;
    s->r = nullptr;
  }

  // Zwraca lca(this, v).
  Splay* lca(Splay* v) {
    expose();
    v->expose();
    return lastDisc;
  }

};

Splay* Splay::lastDisc;
