class MinCostFlow {
  struct MCEdge { int to, cost, flow; MCEdge* next; };
  const int Infty = 1e9 + 100;
  vector<vector<MCEdge*>> adjList;
  int N, Source, Sink;
  VI dist;
  vector<MCEdge*> prev, Edges;
  void spfa() {
    queue<int> Q; vector<bool> onQueue(N); fill(ALL(dist), Infty);
    Q.push(Source); onQueue[Source] = true; dist[Source] = 0;
    while (!Q.empty()) {
      int v = Q.front(); Q.pop(); onQueue[v] = false;
      for (MCEdge *E : adjList[v]) {
        int s = E->to;
        if (E->flow == 0) { continue; }
        int newDist = dist[v] + E->cost;
        if (newDist < dist[s]) {
          dist[s] = newDist;
          prev[s] = E->next;
          if (!onQueue[s]) {
            Q.push(s); onQueue[s] = true;
          }
        }
      }
    }
  }
  int reduce_cost() {
    REP (v, N) for (MCEdge* E: adjList[v]) E->cost += dist[v] - dist[E->to];
    return dist[Sink];
  }
  void dijkstra_shortest_path(){
    fill(ALL(dist), Infty); dist[Source] = 0;
    priority_queue<PII> Q; Q.push(make_pair(0, Source));
    while (!Q.empty()){
      int dst = -Q.top().st, v = Q.top().nd; Q.pop();
      if (dst != dist[v]) { continue; }
      for (MCEdge* E: adjList[v]){
        int s = E->to;
        if (!E->flow) { continue; }
        int newDist = dist[v] + E->cost;
        if(newDist < dist[s]){
          dist[s] = newDist;
          prev[s] = E->next;
          Q.push({-newDist, s});
        }
      }
    }
  }
public:
  MinCostFlow(){}
  MinCostFlow(int _N) : N(_N), dist(_N), prev(_N), adjList(_N) {}
  ~MinCostFlow(){ for (MCEdge* E: Edges) { delete E; } }
  void fit(int size) {
    if (size > N) {
      N = size;
      dist.resize(size); prev.resize(size); adjList.resize(size);
    }
  }
  void add_edge(int u, int v, int flow, int cost){
    fit(max(u, v) + 1);
    MCEdge *E1 = new MCEdge{v, cost, flow}, *E2 = new MCEdge{u, -cost, 0};
    Edges.PB(E1); Edges.PB(E2);
    E1->next = E2; E2->next = E1;
    adjList[u].PB(E1); adjList[v].PB(E2);
  }
  PII get_min_cost_flow(int s, int t){
    fit(max(s, t) + 1);
    Source = s; Sink = t;
    int cost = 0, flow = 0;
    spfa();
    if(dist[Sink] > Infty / 2) { return {0,0}; }
    int sinkCost = reduce_cost();
    while (true) {
      dijkstra_shortest_path();
      if(dist[Sink] > Infty / 2) { break; }
      sinkCost += reduce_cost();
      int maxSend = Infty;
      for(int v = Sink; v != Source; v = prev[v]->to){
        maxSend = min(maxSend, prev[v]->next->flow);
      }
      for(int v = Sink; v != Source; v = prev[v]->to){
        MCEdge *E1 = prev[v], *E2 = E1->next;
        E1->flow += maxSend; E2->flow -= maxSend;
      }
      flow += maxSend;
      cost += maxSend * sinkCost;
    }
    return {flow, cost};
  }
};
