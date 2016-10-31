/* DINIC O(n^2*m) lub O(MAXFLOW*(n+m)).                                       */
/* Szybkie, można ryzykować, w razie czego shufflowac krawędzie.              */
/* Używamy funkcji add_edge().                                                */
/* Używamy funkcji dinic(), która modyfikuje krawędzie i zwraca wartość       */
/* maksymalnego przepływu.                                                    */
/* Na krawędziach zostaje flow, do odtwarzania wyniku.                        */
/* W większości miejsc są inty, ale łatwo zmienić na long longi.              */

// TODO(Mateusz): Zrobić z tego template, żeby można było łatwo zmieniać
// typ przepustowości na krawędziach (tzn, żeby jeszcze łatwiej było
// "zmienić na long longi").
struct flow {
  struct edge {  // Można dodawać atrybuty w celu identyfikacji krawędzi.
    int cel = 0;
    int lim = 0;
    int flow = 0;
    edge* odw;
  };

  //liczba wierzcholkow, numer zrodla i numer ujscia, numerowane od 1
  int n, zr, uj;

  vector<vector<edge*>> graf;
  vector<vector<edge*>> dgraf;
  vector<int> odl;

  flow() {}

  flow(int nn, int zrzr, int ujuj) {
    n = nn;
    zr = zrzr;
    uj = ujuj;
    graf.resize(n + 1);
    dgraf.resize(n + 1);
    odl.resize(n + 1, 0);
  }

  // Dodaje krawędź od @v do @u o przepustowosci @lim.
  // TODO(Mateusz): Czym jest @bi_dir? Nie powinno być boolem?
  void add_edge(int v, int u, int lim, int bi_dir) {
    edge* wte = new edge();
    edge* wtamte = new edge();
    wte->odw = wtamte;
    wtamte->odw = wte;
    wte->lim = lim;
    if (bi_dir) wtamte->lim = lim;
    wte->cel = u;
    wtamte->cel = v;
    graf[v].push_back(wte);
    graf[u].push_back(wtamte);
  }

  // TODO(Mateusz): Opisać co to robi (np. stwierdzić, że @n musi być ustawione
  // zanim się wywoła tę metodę).
  void clear() {
    for (int i = 1; i <= n; i++) {
      odl[i] = 0;
      dgraf[i].clear();
    }
  }

  void bfs() {
    queue<int> kol;
    int u;
    kol.push(zr);
    odl[zr] = 1;
    while (!kol.empty()) {
      u = kol.front();
      kol.pop();
      for (int i = 0; i < graf[u].size(); i++) {
        if (!odl[graf[u][i]->cel] && graf[u][i]->lim) {
          odl[graf[u][i]->cel] = odl[u] + 1;
          kol.push(graf[u][i]->cel);
        }
        if (odl[graf[u][i]->cel] == odl[u] + 1)
          dgraf[u].push_back(graf[u][i]);
      }
    }
  }

  int dfs(int v, int mini) {
    if (v==uj) return mini;
    for (int i = 0; i < dgraf[v].size(); i++) {
      if (dgraf[v][i]->lim) {
        const int ret = dfs(dgraf[v][i]->cel, min(mini, dgraf[v][i]->lim));
        if (!ret) {
          swap(dgraf[v][i], dgraf[v].back());
          dgraf[v].pop_back();
          i--;
          continue;
        }
        dgraf[v][i]->lim -= ret;
        dgraf[v][i]->odw->lim += ret;
        dgraf[v][i]->flow += ret;
        const int pom = min(dgraf[v][i]->flow, dgraf[v][i]->odw->flow);
        dgraf[v][i]->flow -= pom;
        dgraf[v][i]->odw->flow -= pom;
        return ret;
      } else {
        swap(dgraf[v][i], dgraf[v].back());
        dgraf[v].pop_back();
        i--;
      }
    }
    return 0;
  }

  ll dinic() {
    ll ret = 0;
    while (true) {
      clear();
      bfs();
      ll bier1 = 0;
      while (true) {
        // TODO(Mateusz) - co to za stała w ogóle?
        ll bier2 = dfs(zr, 1000 * 1000 * 1000);
        if (!bier2) break;
        bier1 += bier2;
      }
      if (!bier1) break;
      ret += bier1;
    }
    return ret;
  }
};
