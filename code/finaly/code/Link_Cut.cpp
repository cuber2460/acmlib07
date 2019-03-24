struct Splay {
  Splay *l = 0, *r = 0, *p = 0;
  bool flip = false; // Wywal jak nie używasz make_root.
  int roz = 1;       // SUBTREE  Rozmiar poddrzewa. Wszystko z SUBTREE opcjonalne.
  int axroz = 1;    // SUBTREE  Pomocniczny rozmiar poddrzewa.
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
    Splay* f = p; Splay* t = f->p;
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
  Splay* expose() {    // v będzie korzeniem splaya zawierającego ścieżkę do korzenia. Prawe dziecko 
    Splay *q = this, *x = 0;  // będzie nullem. Jak zejdziemy w dół, to potem trzeba zrobić splay(). 
    while (q) {                                          // LCA(u, v): u->expose(); ret v->expose();
      q->splay();
      if (q->r) q->roz += q->r->axroz;  // SUBTREE
      if (x) q->roz -= x->axroz;        // SUBTREE
      q->r = x;  q->update();
      x = q;  q = q->p;
    }
    splay();
    return x;
  }
  Splay* root() {  // Zwraca roota drzewowego (nie splejowego!).
    expose();
    Splay* s = this;
    while (s->touch(), s->l) s = s->l;
    s->splay();
    return s;
  }
  void cut() {  // Usuwa krawędź znajdującą się nad danym wierzchołkiem.
    expose(); assert(l /* Nie jest rootem. */);
    Splay* s = l;
    while (s->touch(), s->r) s = s->r;
    s->splay();  s->r->p = 0;  s->r = 0;
  }
  void link(Splay* to) {
    expose(); assert(!l /* Jest rootem. */);
    p = to;
    p->expose();  p->roz += axroz;  p->axroz += axroz;  // SUBTREE
  }
  // Sprawia, że wierzchołek jest rootem w logicznym i w splayowym drzewie.
  void make_root() { expose(); flip = !flip; touch(); }
};
