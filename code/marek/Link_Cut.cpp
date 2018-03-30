struct Splay {
  Splay *l = nullptr, *r = nullptr, *p = nullptr;
  bool flip = false;
  int roz = 1;    // SUBTREE  Rozmiar poddrzewa.
  int axroz = 1;  // SUBTREE  Pomocniczny rozmiar poddrzewa.

  void update() {
    assert(!flip and (!l or !l->flip) and (!r or !r->flip));
    axroz = roz;               // SUBTREE
    if (l) axroz += l->axroz;  // SUBTREE
    if (r) axroz += r->axroz;  // SUBTREE
  }

  void touch() {
    if (flip) {
      swap(l, r);
      if (l) l->flip = !l->flip;
      if (r) r->flip = !r->flip;
      flip = false;
    }
  }

  bool sroot() { return !p or (p->l != this and p->r != this); }
  void connect(Splay* c, bool left) { (left ? l : r) = c; if (c) c->p = this; }

  void rotate() {
    Splay* f = p;
    Splay* t = f->p;
    const bool isr = f->sroot();
    const bool left = (this == f->l);
    f->connect(left ? r : l, left);
    connect(f, !left);
    if (isr) p = t;
    else t->connect(this, f == t->l);
    f->update();
  }

  void push() {
    sroot() ? touch() : p->push();
    if (l) l->touch(); if (r) r->touch();
  }

  void splay() {
    push();
    while (!sroot()) {
      Splay* x = p->p;
      if (!p->sroot()) (((p->l == this) == (x->l == p)) ? p : this)->rotate();
      rotate();
    }
    update();
  }

  // Przenosi wierzchołek do korzenia. Prawe dziecko v będzie równe nullptr.
  // Aby zrobić coś na ścieżce od korzenia do v:
  //   >> v->expose();
  //   >> /* v reprezentuje ścieżkę, można z nim robić co się chce.
  //   >>  * Jeśli się odwiedzi wierzchołki poniżej v, to na najniższym
  //   >>  * trzeba wywołać ->splay(); */
  // Aby znaleźć LCA u i v (muszą być w tym samym poddrzewie):
  //   >> u->expose(); lca = v->expose();
  Splay* expose() {
    Splay *q = this, *x = nullptr;
    while (q) {
      q->splay();
      if (q->r) q->roz += q->r->axroz;  // SUBTREE
      if (x) q->roz -= x->axroz;        // SUBTREE
      q->r = x;
      q->update();
      x = q;
      q = q->p;
    }
    splay();
    return x;
  }

  // Zwraca roota drzewowego (nie splejowego!).
  Splay* root() {
    expose();
    Splay* s = this;
    while (s->touch(), s->l) s = s->l;
    s->splay();
    return s;
  }

  // Zakłada, że (*this) nie jest korzeniem drzewa.
  // Usuwa krawędź znajdującą się nad danym wierzchołkiem.
  void cut() {
    expose(); assert(l /* Nie jest rootem. */);
    Splay* s = l;
    while (s->touch(), s->r) s = s->r;
    s->splay(); s->r->p = nullptr; s->r = nullptr;
  }

  void link(Splay* to) {
    expose(); assert(!l /* Jest rootem. */);
    p = to;
    p->expose();        // SUBTREE
    p->roz += axroz;    // SUBTREE
    p->axroz += axroz;  // SUBTREE
  }

  // Sprawia, że wierzchołek jest rootem w swoim drzewie oraz jest "exposed()".
  void make_root() { expose(); flip = !flip; touch(); }
};
