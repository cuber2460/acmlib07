#define int ll//jeśli long longi potrzebne
struct Vizing {
  vector<vector<pair<int,int>>> adj;
  map<pair<int,int>, int> edges;
  vector<int> edge_colors;
  vector<vector<int>> color_to;
  vector<vector<int>> color_queue;
  vector<int> unused_color;
  int N, M, K;
 
  Vizing(int size) : adj(size), N(size), M(0) {}
 
  void AddEdge(int u, int v) {
    adj[u].emplace_back(u, M);
    adj[v].emplace_back(v, M);
    edges[make_pair(min(u, v), max(u, v))] = M;
    ++M;
  }                                     //numeracja od zera
                                        //multikrawędzie niebezpieczne
  int MaxDegree() const {               //ogólnie nietestowane
    int answer = 0;
    for (int i = 0; i < N; ++i)
      answer=max(answer, (int)adj[i].size());
    return answer;
  }
 
  void FindFree(int v) {
    while (!color_queue[v].empty()) {
      const int c = color_queue[v].back();
      if (color_to[v][c] == -1) {
        unused_color[v] = c;
        return;
      } else {
        color_queue[v].pop_back();
      }
    }
    assert(false);
  }
 
  void FlipPath(int from, int a, int b) {
    const int to = color_to[from][a];
    color_queue[from].push_back(a);
    color_queue[from].push_back(b);
    color_to[from][b] = -1;
    if (to == -1) { return; }
    FlipPath(to, b, a);
    color_to[from][b] = to;
    color_to[to][b] = from;
    color_to[from][a] = -1;
    FindFree(from);
    FindFree(to);
  }
 
  void ColorEdge(int x, int y) {
    for (int col = 0; col <= K; ++col)
      if (color_to[x][col] == -1 && color_to[y][col] == -1) {
        color_to[x][col] = y;
        color_to[y][col] = x;
        FindFree(x); FindFree(y);
        return;
      }
 
    vector<int> fan{y}, fan_colors{-1};
    vector<bool> has_on(N);
    const int c = unused_color[x];
    int d;
    while (true) {
      d = unused_color[fan.back()];
      if (color_to[x][d] == -1 || d == c) { break; }
      if (has_on[d]) {
        FlipPath(x, d, c);
        FindFree(x);
        return ColorEdge(x, y);
      }
      fan.push_back(color_to[x][d]);
      fan_colors.push_back(d);
      has_on[d] = true;
    }
    fan_colors.push_back(-1);
    for (int i = 0; i < (int)fan.size(); ++i) {
      const int from = fan_colors[i], to = fan_colors[i + 1];
      if (from != -1) {
        color_queue[fan[i]].push_back(from);
        color_queue[x].push_back(from);
        color_to[fan[i]][from] = -1;
      }
      if (to != -1) {
        color_to[fan[i]][to] = x;
        color_to[x][to] = fan[i];
      }
    }
    assert(color_to[x][d] == -1);
    color_to[x][d] = fan.back();
    color_to[fan.back()][d] = x;
    for (int v : fan) { FindFree(v); }
    FindFree(x);
  }
 
  vector<int> ColorGraph() {
    K = MaxDegree();
    edge_colors = vector<int>(M, -1);
    color_to = vector<vector<int>>(N, vector<int>(K + 1, -1));
    vector<int> all_colors(K + 1);
    iota(all_colors.begin(), all_colors.end(), 0);
    color_queue = vector<vector<int>>(N, all_colors);
    unused_color = vector<int>(N);
    for (auto &edge : edges)
      ColorEdge(edge.first.first, edge.first.second);
    for (int v = 0; v < N; ++v)
      for (int c = 0; c <= K; ++c)
        if (color_to[v][c] > v) {
          assert(color_to[color_to[v][c]][c] == v);
          edge_colors[edges[make_pair(v, color_to[v][c])]] = c;
        }
    return edge_colors;
  }
};
#undef int
