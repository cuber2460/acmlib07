#define int ll//jeśli long longi potrzebne
struct GomoryHu {
  vector<vector< pair<int,int> >> graph, tree;
  vector<vector<int>> nodes;
  vector<bool> visited;                     //wymaga naszego dinica
  vector<int> groupId, contrId;             //numeracja od zera
  int wnode, n;
  GomoryHu(int N) : graph(N), visited(N), groupId(N), contrId(N), tree(N), n(N) {}
  void addEdge(int u, int v, int cap) {
    graph[u].emplace_back(v, cap);
    graph[v].emplace_back(u, cap);
  }
  void dfs(int v, int type) {
    visited[v] = true; contrId[v] = type;
    for (auto P : tree[v]) { if (!visited[P.first]) { dfs(P.first, type); } }
  }
  vector <pair<pair<int,int>,int>> run() {
    vector<int> allNodes(n);
    iota(allNodes.begin(), allNodes.end(), 0);
    nodes = vector<vector<int>>{allNodes};
    tree = vector<vector<pair<int,int>>>(n);
    fill(groupId.begin(), groupId.end(), 0);
    for (int step = 1; step < n; step++) {
      Flow flow;
      for (int i = 0; i < (int)nodes.size(); i++) {
        if ((int)nodes[i].size() > 1) { wnode = i; break; }
      }
      fill(visited.begin(), visited.end(), false);
      visited[wnode] = true;
      for (auto P : tree[wnode]) { dfs(P.first, nodes[P.first][0]); }
      for (int v = 0; v < n; v++) {
        int a = groupId[v] == wnode ? v : contrId[groupId[v]];
        for (auto& P : graph[v]) {
          int b = groupId[P.first] == wnode ? P.first : contrId[groupId[P.first]];
          if (a != b) { flow.add_edge(a, b, P.second); }
        }
      }
      int a = nodes[wnode][0], b = nodes[wnode][1], f = flow.dinic(a, b);
      auto pom = flow.cut();
      vector <bool> cut(n, false);
      for (int i : pom)
        cut[i]=1;
      for (int v = 0; v < step; v++) {
        if (v == wnode) { continue; }
        for (auto& P : tree[v]) {
          if (P.first == wnode && !cut[contrId[v]]) { P.first = step; }
        }
      }
      vector<pair<int,int>> PA, PB;
      for (auto& P : tree[wnode]) { (cut[contrId[P.first]] ? PA : PB).push_back(P); }
      tree[wnode] = PA; tree[step] = PB;
      tree[wnode].emplace_back(step, f);
      tree[step].emplace_back(wnode, f);
      vector<int> A, B;
      for (int v : nodes[wnode]) {
        (cut[v] ? A : B).push_back(v);
        if (!cut[v]) { groupId[v] = step; }
      }
      nodes[wnode] = A;
      nodes.push_back(B);
    }
    vector <pair<pair<int,int>,int>> res;
    for (int i = 0; i < n; i++)
      for (auto P : tree[i])
        if (nodes[i][0]<nodes[P.first][0])
          res.push_back({{nodes[i][0], nodes[P.first][0]}, P.second});
    return res;
  }
};
#undef int
