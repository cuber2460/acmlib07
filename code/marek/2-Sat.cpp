struct Sat {
  int n, ile;
  vector<vector<int>> imp;
  vector<bool> vis;
  vector<int> val, sort;

  void DfsMark(int x) {
    vis[x] = false;
    val[x] = (val[x ^ 1] == -1);
    for (int i : imp[x]) if (vis[i]) DfsMark(i);
  }
  void Dfs(int x) {
    vis[x] = true;
    for (int i : imp[x ^ 1]) if (!vis[i ^ 1]) Dfs(i ^ 1);
    sort[--ile] = x;
  }
  Sat(int m) : n(m * 2), ile(n), imp(n), vis(n), val(n, -1), sort(n) {}
  void Or(int a, int b) {
    imp[a ^ 1].push_back(b);
    imp[b ^ 1].push_back(a);
  }
  bool Run() {
    for (int i = 0; i < n; i++) if (!vis[i]) Dfs(i);
    for (int i : sort) if (vis[i]) DfsMark(i);
    for (int i = 0; i < n; i++)
      if (val[i]) for (int x : imp[i]) if (!val[x]) return false;
    return true;
  }
};
int main() {
  Sat sat(3);
  sat.Or(2 * 0 + 1, 2 * 1 + 0);  // !x_0 or  x_1
  sat.Or(2 * 1 + 0, 2 * 2 + 1);  //  x_1 or !x_2
  debug() << imie(sat.Run());
  debug() << imie(sat.val); // [0, 1, 1, 0, 0, 1] = [x0, !x0, x1, !x1, x2, !x2]
}
