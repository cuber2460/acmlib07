const int inf = 1e9 + 5;
const int nax = 6405;

//c - capacity, v - value
//wierzcholki sa od 0 do n, wszystko na intach 
//wiec wtedy musisz ogarnac odlegloci i takie tam
namespace MinCost{  
  struct Edge{
    int w,c,v,rev;
    Edge(int _w, int _c, int _v, int _rev) :
      w(_w), c(_c), v(_v), rev(_rev)
    {}
  };
  
  int odl[nax], pot[nax], pop[nax], pop_kraw[nax];
  int q[nax], qbeg, qend;
  vector<Edge> v[nax];
  bool bylo[nax];
  queue<int> kolej;
  //priority_queue<pair<int,int> > kolej;
  
  void init(int n) {
    FOR(i,0,n) v[i].clear();
  }
  
  void AddEdge(int a, int b, int cap, int cost) {
    v[a].pb(Edge(b,cap,cost,int(v[b].size()) + (a == b)));
    v[b].pb(Edge(a,0,-cost,int(v[a].size()-1)));
  }
  
  pair<int,int> MinCostMaxFlow(int s, int t, int n) {
    int flow = 0, cost = 0;
    //FOR(i,0,n) pot[i] = 0; 
    //potencjaly zerowe dzialaja dla samych nieujemnych kosztow
    //jak sa ujemne krawedzie to bellman
    
    while (true) {
      FOR(i,0,n) {
        odl[i] = inf;
        bylo[i] = false;
      }
      bylo[s] = true;
      odl[s] = 0;
      kolej.push(s);
      
      //djikstra, mozna napisac na kolejce
      while(!kolej.empty()) {
        int x = kolej.front();
        kolej.pop();
        bylo[x] = false;

        //if (bylo[x])
        //  continue;
        
        /*int najm = inf;
        FOR(i,0,n) if (!bylo[i] && najm > odl[i]) {
          x = i; najm = odl[i];
        }*/
        
        //bylo[x] = true;
        int dl = v[x].size();
        REP(i,dl) if (v[x][i].c > 0 && 
          odl[v[x][i].w] > 
            odl[x] + pot[x] - pot[v[x][i].w] + v[x][i].v) {
          odl[v[x][i].w] = 
            odl[x] + pot[x] - pot[v[x][i].w] + v[x][i].v;
          if (!bylo[v[x][i].w]) {
            kolej.push(v[x][i].w);
            bylo[v[x][i].w] = true;
          }
          //kolej.push(mp(-odl[v[x][i].w], v[x][i].w));
          pop[v[x][i].w] = x; pop_kraw[v[x][i].w] = i; 
        } 
      }
      
      if (odl[t] == inf)
        break;
      
      //FOR(i,0,n) pot[i] += odl[i];
      
      int x = t;
      int cap = inf;
      while (x != s) {
        cap = min(cap, v[pop[x]][pop_kraw[x]].c);
        x = pop[x];
      }
      
      flow += cap;
      x = t;
      while (x != s) {
        cost += v[pop[x]][pop_kraw[x]].v*cap;
        v[pop[x]][pop_kraw[x]].c -= cap;
        v[x][v[pop[x]][pop_kraw[x]].rev].c += cap;
        x = pop[x];
      }
    }
    
    return mp(flow, cost);
  }
};
