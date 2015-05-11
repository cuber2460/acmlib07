const int inf = 1e9 + 5;
const int nax = 2e5 + 5;

int n,m,a,b,czas,ile,iles;
int wejscie[nax], ojc[nax], low[nax];
bool bylo[nax], punkt[nax];
vector<int> v[nax], dwuspojne[nax];

int dfs(int x) {
  ++czas;
  int ile_syn = 0;
  wejscie[x] = low[x] = czas;
  bylo[x] = true;
  for (auto i: v[x]) {
    if (!bylo[i]) {
      ++ile_syn;
      ojc[i] = x;
      low[x] = min(low[x], dfs(i));
    }
    else
      if (i != ojc[x])
        low[x] = min(low[x], wejscie[i]);
  }
  
  if (!ojc[x])
    punkt[x] = ile_syn > 1;
  else
    for (auto i: v[x]) if (ojc[i] == x && low[i] >= wejscie[x])
      punkt[x] = true;
          
  return low[x];
}

void dfs4(int x) {
  dwuspojne[iles].pb(x);
  for (auto i: v[x]) if (ojc[i] == x && low[i] < wejscie[x])
    dfs4(i);
}

void dfs3(int x) {
  //printf("robie %d\n",x);
  if (!ojc[x] || punkt[x]) {
    for (auto i:v[x]) if (ojc[i] == x && low[i] == wejscie[x]) {
      dwuspojne[iles].pb(x);
      dfs4(i);
      ++iles;
    }
  }
  
  for (auto i: v[x]) if (ojc[i] == x)
    dfs3(i);
}


int main(int argc, char * argv[]) {
  scanf("%d%d",&n,&m);
  REP(i,m) {
    scanf("%d%d",&a,&b);
    v[a].pb(b);
    v[b].pb(a);
  }
  
  FOR(i,1,n) if (!bylo[i]) {
    dfs(i);
    dfs3(i);
  }
  return 0;
}
