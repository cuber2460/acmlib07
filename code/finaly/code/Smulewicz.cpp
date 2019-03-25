#define int ll//jeśli long longi potrzebne
struct SPFA{
  int n; vector<int> odl, oj, czok;
  vector<vector<pair<int,int>>> d; vector<vector<int>> d2;
  const int inf = 1e9;
  SPFA(int _n):n(_n+1){
    odl.resize(n, inf); oj.resize(n); czok.resize(n);
    d.resize(n); d2.resize(n);
  }
  vector<int> cykl; int root;
  bool us(int nr){
    if(nr == root) return 1;
    czok[nr] = 0;
    for(int ak:d2[nr]){
      if(oj[ak] == nr){
        if(us(ak)){
          cykl.push_back(nr);                  //numeracja od zera
          return 1;
        }
      }
    }
    d2[nr].clear();
    return 0;
  }
  bool licz_sciezki(int s){ // false, gdy z s da sie dojsc do ujemnego cyklu
    vector<int> st, st2;    // znaleziony cykl jest w wektorze cykl
    odl[s] = 0; czok[s] = 1; st.push_back(s);
    while(st.size()){
      for(int i=0; i<(int)st.size(); i++){
        int ak = st[i];
        if(czok[ak]) for(pair<int,int> x:d[ak]){
          int nei, cost; tie(nei, cost) = x;
          if(odl[ak] + cost < odl[nei]){
            root = ak;
            if(us(nei)){
              cykl.push_back(ak); reverse(cykl.begin(), cykl.end());
              return 0;
            }
            odl[nei] = odl[ak] + cost; oj[nei] = ak;
            d2[ak].push_back(nei); czok[nei] = 1;
            st2.push_back(nei);
          }
        }
      }
      st.clear(); swap(st, st2);
    }
    return 1;
  }
  vector<int> ujemny_cykl(){
    for (int i=0; i<n-1; i++) add_edge(n-1, i, 0);
    if (licz_sciezki(n-1)){
      return {};
    } else {
      return cykl;
    }
  }
  void add_edge(int a, int b, int cost){
    d[a].push_back({b, cost});
  }
};
#undef int
