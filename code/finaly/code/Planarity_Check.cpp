#define PB push_back
#define SZ(x) ((int)(x).size())
#define FOR(i,a,b) for (int i = (a); i <= (b); ++i)
#define VI vector<int>               //numeracja od zera
struct FU {                          //nie dawać multikrawędzi ani pętli
  VI p; vector<bool> b;              //nie odpalać IsPlanar() ponad raz
  FU() {}
  FU(int n) : p(n), b(n) { iota(p.begin(), p.end(), 0); }
  pair<int, bool> Find(int v) {
    if (p[v] == v) { return {v, 0}; }
    auto res = Find(p[v]); res.second ^= b[v];
    p[v] = res.first; b[v] = res.second; return res;
  }
  bool Union(int x, int y, bool flip) {
    bool xb, yb; tie(x, xb) = Find(x); tie(y, yb) = Find(y);
    if (x == y) { return !(xb ^ yb ^ flip); }
    p[x] = y; b[x] = xb ^ yb ^ flip; return true;
  }
};
struct PlanarityTest {
  int N, M, tm;
  vector<VI> adj, dn, up; vector<pair<int,int>> e_up;
  vector<bool> vis; VI low, pre;
  FU fu;
  void DfsLow(int v, int p) {
    vis[v] = true; low[v] = pre[v] = tm++;
    for (int s : adj[v]) {
      if (s == p) { continue; }
      if (!vis[s]) {
        dn[v].PB(s); DfsLow(s, v); low[v] = min(low[v], low[s]);
      } else if (pre[s] < pre[v]) {
        up[v].PB(SZ(e_up)); e_up.PB({v, s});
        low[v] = min(low[v], pre[s]);
      }
    }
  }
  VI Interlace(const VI &ids, int lo) {
    VI ans;
    for (int e : ids) if (pre[e_up[e].second] > lo) ans.PB(e);
    return ans;
  }
  bool AddFU(const VI &a, const VI &b) {
    FOR (k, 1, SZ(a) - 1) if (!fu.Union(a[k - 1], a[k], 0)) { return false; }
    FOR (k, 1, SZ(b) - 1) if (!fu.Union(b[k - 1], b[k], 0)) { return false; }
    if (SZ(a) && SZ(b) && !fu.Union(a[0], b[0], 1)) { return false; }
    return true;
  }
  bool DfsPlanar(int v, int p) {
    for (int s : dn[v]) if (!DfsPlanar(s, v)) { return false; }
    auto sz = SZ(dn[v]);
    FOR (i, 0, sz - 1) {
      FOR (j, i + 1, sz - 1) {
        auto a = Interlace(up[dn[v][i]], low[dn[v][j]]);
        auto b = Interlace(up[dn[v][j]], low[dn[v][i]]);
        if (!AddFU(a, b)) { return false; }
      }
      for (int j : up[v]) {
        if (e_up[j].first != v) { continue; }
        auto a = Interlace(up[dn[v][i]], pre[e_up[j].second]);
        auto b = Interlace({j}, low[dn[v][i]]);
        if (!AddFU(a, b)) { return false; }
      }
    }
    for (int s : dn[v]) {
      for (int idx : up[s]) {
        if (pre[e_up[idx].second] < pre[p]) { up[v].PB(idx); }
      }
      up[s].clear(); up[s].shrink_to_fit();
    }
    return true;
  }
  PlanarityTest(int n) : N(n), M(0), adj(n) {}
  void AddEdge(int u, int v) { adj[u].PB(v); adj[v].PB(u); ++M; }
  bool IsPlanar() {
    if (N <= 3) { return true; }
    if (M > 3 * N - 6) { return false; }
    vis = vector<bool>(N);
    up = dn = vector<VI>(N);
    low = pre = VI(N);
    FOR (v, 0, N - 1) if (!vis[v]) {
      e_up.clear(); tm = 0; DfsLow(v, -1);
      fu = FU(SZ(e_up));
      if (!DfsPlanar(v, -1)) { return false; }
    }
    return true;
  }
};
