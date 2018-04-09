constexpr bool persistent = true;
struct Node;
struct N {
  int v;
  N(int v_ = 0) : v(v_) {}
  N(Node* n);
  Node* operator->() const;
  operator int() const { return v; }
};
struct Node {
  N l, r;
  int h = 0;
  bool NeedsTouch() { return false; }
  void Touch() {}
  void Update() { assert(!NeedsTouch()); }
  Node(const Node& node) = default;
  Node* ptr() { return this; }
};
constexpr int kMaxNodes = (450 * 1024 * 1024 /* 450MiB */) / sizeof(Node);
int nast_node = 1 /* 0 is reserved for the null Node */;
Node node_ptr[kMaxNodes];
template <typename ...Args> N New(Args&& ...args) {
  assert(nast_node < kMaxNodes);
  node_ptr[nast_node] = Node(forward<Args>(args)...);
  return nast_node++;
}
N::N(Node* n) : v(n - node_ptr) {}                   // Converts index to ptr.
Node* N::operator->() const { return node_ptr + v; } // Converts ptr to index.
N Touch(N n) {
  if (!n or !n->NeedsTouch()) return n;
  if (persistent) {
    if (n->l) n->l = New(*n->l->ptr());
    if (n->r) n->r = New(*n->r->ptr());
  }
  n->Touch();
  return n;
}
N Make(N l, N v, N r) {
  if (persistent) v = New(*v->ptr());
  v->l = l;
  v->r = r;
  v->h = max(l->h, r->h) + 1;
  assert(abs(l->h - r->h) <= 2);
  v->Update();
  return v;
}
N Bal(N l, N v, N r) {
  assert(abs(l->h - r->h) <= 3);
  Touch(l); Touch(r);
  if (l->h > r->h + 2) {
    N ll = l->l, lr = l->r;
    if (ll->h >= lr->h) return Make(ll, l, Make(lr, v, r));
    return Touch(lr), Make(Make(ll, l, lr->l), lr, Make(lr->r, v, r));
  } else if (r->h > l->h + 2) {
    N rr = r->r, rl = r->l;
    if (rr->h >= rl->h) return Make(Make(l, v, rl), r, rr);
    return Touch(rl), Make(Make(l, v, rl->l), rl, Make(rl->r, r, rr));
  } else {
    return Make(l, v, r);
  }
}
N AddLeft(N n, N v) {
  if (!Touch(n)) return Make(0, v, 0);
  return Bal(AddLeft(n->l, v), n, n->r);
}
N AddRight(N n, N v) {
  if (!Touch(n)) return Make(0, v, 0);
  return Bal(n->l, n, AddRight(n->r, v));
}
pair<N, N> RemLeft(N n) {
  if (!Touch(n)->l) return {n->r, n};
  auto s = RemLeft(n->l);
  return {Bal(s.first, n, n->r), s.second};
}
// Joins l, r - trees, v - single vertex that will be overridden even
// in the case of a persistent tree.
N Join(N l, N v, N r) {
  if (!Touch(l)) return AddLeft(r, v);
  if (!Touch(r)) return AddRight(l, v);
  if (l->h > r->h + 2) return Bal(l->l, l, Join(l->r, v, r));
  if (r->h > l->h + 2) return Bal(Join(l, v, r->l), r, r->r);
  return Make(l, v, r);
}
N Merge(N l, N r) {
  if (!l or !r) return r + l;
  auto s = RemLeft(r);
  return Join(l, s.second, s.first);
}
pair<N, N> Split(N n, /* Maybe some additional arguments. */) {
  if (!Touch(n)) return {0, 0};
  if (/* Condition for checking if n belongs to the left tree. */) {
    auto s = Split(n->r, /* Some args. */);
    return {Join(n->l, n, s.first), s.second};
  } else /* n belongs to the right tree. */ {
    auto s = Split(n->l, /* Some args. */);
    return {s.first, Join(s.second, n, n->r)};
  }
}
