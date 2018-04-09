#define int long long//jeśli long longi potrzebne
struct DMST {
  int N;
  vector<int> eFrom, eTo, eCost, ePrev, visited, cycle, parent;
  vector<vector<int>> cycles, adj, curEdge;
  int Root, fstEdge;

  DMST(int V) : N(V), visited(2*V), parent(2*V), cycles(2*V), adj(2*V),
        curEdge(2*V, vector<int>(2*V, -1)) {}

  void addEdge(int u, int v, int c, int prev = -1) {
    if (prev != -1) {
      if (curEdge[u][v] != -1) {
        int id = curEdge[u][v];
        if (eCost[id] > c) { eCost[id] = c; ePrev[id] = prev; }
        return;
      }
    }
    int id = (int)eFrom.size();
    if (u == v) {
      u = v = c = -1;
    } else {
      adj[u].push_back(id);
      curEdge[u][v] = id;
    }
    eFrom.push_back(u);
    eTo.push_back(v);
    eCost.push_back(c);
    ePrev.push_back(prev);
  }

  bool dfsCyc(int v) {
    if (v == Root) { return false; }
    visited[v] = 1;
    cycle.push_back(parent[v]);
    int p = eFrom[parent[v]];
    if (visited[p] == 1) { fstEdge = parent[p]; }
    bool res = visited[p] == 1 || (!visited[p] && dfsCyc(p));
    visited[v] = 2;
    return res;
  }

  vector<int> compute(int root) {
    Root = root;
    vector<bool> current(2 * N), onCycle(2 * N);
    vector<int> best(2 * N);
    fill_n(current.begin(), N, true);
    int curSz = N;
    
    while (true) {
      fill(best.begin(), best.end(), Infty);
      fill(onCycle.begin(), onCycle.end(), false);

      for (int i = 0; i < 2 * N; i++) {
        if (!current[i]) { continue; }
        for (int e : adj[i]) {
          int v = eTo[e], c = eCost[e];
          if (v != root && current[v] && c < best[v]) {
            best[v] = c; parent[v] = e;
          }
        }
      }
      fill(visited.begin(), visited.end(), 0);
      for (int i = 0; i < 2 * N; i++) {
        if (current[i] && !visited[i]) {
          cycle.clear();
          if (dfsCyc(i)) { break; } else { cycle.clear(); }
        }
      }
      if (cycle.empty()) { break; }
      cycle.erase(cycle.begin(), find(cycle.begin(), cycle.end(), fstEdge));
      cycles[curSz] = cycle;
      for (int v : cycle) { onCycle[eFrom[v]] = true; }
      
      for (int v = 0; v < 2 * N; v++) {
        if (!current[v]) { continue; }
        vector<int> edges = adj[v];
        for (int e : edges) {
          int s = eTo[e], c = eCost[e];
          if (!current[s]) { continue; }
          if (!(onCycle[v] ^ onCycle[s])) { continue; }
          if (onCycle[s]) { c -= best[s]; }
          addEdge(onCycle[v] ? curSz : v, onCycle[s] ? curSz : s, c, e);
        }
      }

      for (int v : cycle) { current[eFrom[v]] = false; }
      current[curSz++] = true;
    }

    for (int cyc = curSz - 1; cyc >= N; cyc--) {
      for (int v : cycles[cyc]) { parent[eTo[v]] = v; }
      int e = ePrev[parent[cyc]];
      parent[eTo[e]] = e;
      for (int v = 0; v < 2 * N; v++) {
        if (v != root && eFrom[parent[v]] == cyc) {
          parent[v] = ePrev[parent[v]];
        }
      }
    }
    parent[root] = -1;
    return vector<int>(parent.begin(), parent.begin() + N);
  }
  
  int getValue(vector<int> sol) {
    int total = 0;
    for (int i = 0; i < N; i++) { if (i != Root) { total += eCost[sol[i]]; } }
    return total;
  }

  const int Infty = 1e9;
};
#undef int
