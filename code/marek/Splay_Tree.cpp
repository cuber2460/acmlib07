struct N {
  int c[3] = {0 /* Left */, 0 /* Right */, 0 /* Parent */};
  N() = default;  // The node must be default constructible.

  // User data.
  int key;
  N(int key) : key(key) {}
  void Touch(N* left, N* right)  {}
  // Update must leave the node in a state that doesn't require a "Touch".
  void Update(N* left, N* right) {}

  // Called instead of a destructor.  Should free any resource associated with
  // only this node.  Usually empty function is enough.
  void Clear() {}
};

#define C(v, x)   d[(v) - 1].c[x]
#define P(v)      d[(v) - 1].c[2]
#define X(v)      (C(P(v), 1) == v)

vector<N> d;     // Vector of all nodes (data).
vector<int> fd;  // Vector of indices of nodes that were freed.

template <typename ...A>
int New(A&& ...a) {
  if (fd.empty()) {
    d.emplace_back(forward<A>(a)...);
    return d.size();
  }
  const int i = fd.back();
  fd.pop_back();
  d[i - 1] = N(forward<A>(a)...);
  return i;
}

void ReserveAtLeast(int n) {
  const int shortage = n - (int) fd.size();
  if (shortage > 0) {
    for (int i = 1; i <= shortage; i++) fd.push_back(d.size() + i);
    d.resize(d.size() + shortage);
  }
}

void Delete(int v) { d[v - 1].Clear(); fd.push_back(v); }
void ClearAll()    { d.clear(); fd.clear(); }

 N*    Get(int v) { return v ? &d[v - 1] : nullptr; }
int  Touch(int v) { if (v) d[v - 1].Touch (Get(C(v, 0)), Get(C(v, 1))); return v; }
int Update(int v) { if (v) d[v - 1].Update(Get(C(v, 0)), Get(C(v, 1))); return v; }

// Rotates the edge above node @v.  Assumes that @v has a parent, that all
// nodes above (and including) @v are touched, and that all direct children
// of the path from @v to root are touched and updated.
void Rotate(int v) {
  const int p = P(v);
  const int x = X(v);
  if (P(v) = P(p)) C(P(p), X(p)) = v;
  if (C(p, x) = C(v, x ^ 1)) P(C(p, x)) = p;
  P(C(v, x ^ 1) = p) = v;
  Update(p);
}

// Moves @v to the root.
void Splay(int v) {
  if (!v) return;
  Touch(C(v, 0));
  Touch(C(v, 1));
  while (P(v)) {
    const int p = P(v);
    Touch(C(p, X(v) ^ 1));
    if (P(p)) {
      Touch(C(P(p), X(p) ^ 1));
      Rotate(X(v) == X(p) ? p : v);
    }
    Rotate(v);
  }
  Update(v);
}

// Sets the next node as a root (in direction @x: 0 - left, 1 - right).
// If @v is the last node, does nothing and returns false.
bool Next(int& v, int x = 1) {
  if (!v or !C(Touch(v), x)) return false;
  v = C(v, x);
  while (C(Touch(v), x ^ 1)) v = C(v, x ^ 1);
  Splay(v);
  return true;
}

// Sets the first (or last when x = 1) node as a root.
void First(int& v, int x = 0) {
  while (C(Touch(v), x)) v = C(v, x);
  Splay(v);
}

// Joins trees @l and @r into a single tree.
// Assumes that max(l) < min(l).
int Join(int l, int r) {
  if (!l or !r) return Touch(l + r);
  First(r);
  C(P(Touch(l)) = r, 0) = l;
  return Update(r);
}

// Splits the tree into two parts: left: (-oo, root), [root, +oo).
pair<int, int> Split(int v) {
  if (!v) return {0, 0};
  const int l = C(Touch(v), 0);
  if (!l) return {0, v};
  C(v, 0) = P(l) = 0;
  Touch(C(v, 1));
  return {l, Update(v)};
}

// Deletes the root node, if the tree is not empty.
void DeleteRoot(int& v) {
  if (!Touch(v)) return;
  if (C(v, 0)) P(C(v, 0)) = 0;
  if (C(v, 1)) P(C(v, 1)) = 0;
  const int result = Join(C(v, 0), C(v, 1));
  Delete(v);
  v = result;
}

// Deletes the whole tree.  @v should no longer be used as a tree.
// NOT TESTED!
void DeleteTree(int v) {
 int tmp = v;
  while (v = tmp) {
    if (tmp = C(v, 0)) C(v, 0) = 0;
    else if (tmp = C(v, 1)) C(v, 1) = 0;
    else tmp = P(v), Delete(v);
  }
}

// This function traveses the tree without splaying.  For each node on the path,
// function @f is called.  If it returns 0, the current node is returned.
// On -1, the left subtree is visited, on 1 the right subtree is visited.
// @p is set to the parent of the last visisted node.
// @v points to the variable that holds the last visited node.
// Nothing is splayed, so its neccessary to call Splay on v afterwards.
// F :: N* -> {-1, 0, 1}
// This function operates on pointers, so note that after calling @New(),
// results returned by this function become invalid.
template <typename F>
void GoDown(int*& v, int& p, F f) {
  p = 0;
  int tmp;
  while (Touch(*v) and (tmp = f(*Get(*v)))) {
    v = &C(p = *v, tmp > 0);
  }
}

// -------------------------------------------------------------------------- //
// ----------------------------- User functions ----------------------------- //
// -------------------------------------------------------------------------- //

#define Cmp(x) [x](N& n) -> int {                \
                 if (x == n.key) return 0;       \
                 else if (x < n.key) return -1;  \
                 else return 1;                  \
               }

N& FindOrCreate(int& v, int key) {
  ReserveAtLeast(1);
  int p, *vptr = &v;
  GoDown(vptr, p, Cmp(key));
  if (!*vptr) P(*vptr = New(key)) = p;
  Splay(v = *vptr);
  return *S::Get(v);
}

// (-oo, a), [a, +oo).
pair<int, int> SplitByKey(int v, int a) {
  if (!v) return {0, 0};
  int p, *vptr = &v;
  GoDown(vptr, p, Cmp(a));
  if ((v = *vptr) or Get(v = p)->key > a) {
    Splay(v);
    return Split(v);
  }
  Splay(p);
  if (Next(p)) return Split(p);
  return {p, 0};
}

// [a, b].
// F :: int& -> Return (if it wants to change the subtree)
// F :: int  -> Return (if it doesn't want to change the subtree)
template <typename F>
auto RangeF(int& v, int a, int b, F f) -> decltype(f(a)) {
  auto lm = SplitByKey(v, a);
  auto mr = SplitByKey(lm.second, b + 1);
  auto l = [&]() { v = Join(lm.first, Join(mr.first, mr.second)); };
  using Y = decltype(l);
  struct Z { Z(Y y) : y(y) {} ~Z() { y(); } Y y; } z(l);
  return f(mr.first);
}
