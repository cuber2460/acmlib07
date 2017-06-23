struct N {
  int c[3];  // c[2] is a parent, but you should not depend on that information.
  unsigned l;
  N() : c{0, 0}, l(rand()) {}
  N(const N& n) = default;
};

constexpr int kMaxDepth = 100;
vector<N> nodes(2);

void Reserve(int cnt) {
  int cap = nodes.capacity();
  while (cap - nodes.size() < cnt) cap *= 2;
  nodes.reserve(cap);
}

template <typename ...Args> int New(Args&& ...args) {
  nodes.emplace_back(forward<Args>(args)...);
  return nodes.size() - 1;
}

N& Get(int x)        { return nodes[x];    }
int& C(int x, int y) { return Get(x).c[y]; }
int& P(int x)        { return C(x, 2);     }

int Cpy(int o) { return true /* persistent */ and o ? New(Get(o)) : o; }

int Touch(int v) {
  if (!v) return v;
  C(v, 0) = Cpy(C(v, 0));
  C(v, 1) = Cpy(C(v, 1));
  // Write the touch function here. You can modify the children and the parent.
  return v;
}

int& TC(int& x) { return x = Touch(Cpy(x)); }

int Update(int v) {
  if (!v) return v;
  // Write the update function here.
  return v;
}

int Merge(int a, int b, int c, int A = 1) {
  if (!TC(c)) return b ? Merge(a, 0, b) : Touch(a);
  if (!TC(a)) swap(a, b);
  if (A) Reserve(kMaxDepth * 2);
  TC(b);
  int p = A, x = 1, y;
  while (a) {
    if ((y = Get(a).l < Get(c).l)) swap(a, c);
    if (b and Get(a).l < Get(b).l) {
      if (x == y) swap(a, c);
      C(b, 0) = Merge(a, 0, C(b, 0), 0);
      C(b, 1) = Merge(C(b, 1), 0, c, 0);
      c = Update(b);
      break;
    }
    P(a) = p;
    p = C(p, x) = a;
    if (!Touch(a = C(p, x ^= y))) swap(a, b);
  }
  C(p, x) = c;
  while (p > 1) p = P(Update(p));
  return C(A, 1);
}

template <typename F /* :: int -> {-1, 0, 1} */>
void Split(int v, int& L, int& M, int& R, const F& f, int A = 0, int B = 0) {
  if (!A) Reserve(kMaxDepth * 2);
  int a = A, b = A, x = 1, y;
  v = Cpy(v);
  C(A, 0) = C(A, 1) = 0;
  while (Touch(v)) {
    if (!(y = (f(v) << A) + B)) {
      if (!x) swap(a, b);
      Split(C(v, 0), C(a, 1), M, C(v, 0), f, 1, +1);
      Split(C(v, 1), C(v, 1), M, C(b, 0), f, 1, -1);
      break;
    }
    if (1 - y != x * 2) x ^= 1, swap(a, b);
    a = C(P(v) = a, x) = v;
    swap(v = 0, C(a, x));
  }
  while (a > 1) a = P(Update(a));
  while (b > 1) b = P(Update(b));
  L = C(A, 1); M = Update(v); R = C(A, 0);
}
