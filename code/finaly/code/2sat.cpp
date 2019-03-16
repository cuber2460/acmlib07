const int nax = 100000;
vector <int> implies[2*nax]; //wymuszenia, 2*x to zmienna 2*x+1 to jej negacja
int sat_val[2*nax],sat_vis[2*nax],sat_sort[2*nax],sat_ile;
inline void sat_or(int a,int b){
  implies[a^1].push_back(b);
  implies[b^1].push_back(a);
}
void sat_dfs_mark(int x){
  sat_vis[x]=1;
	sat_val[x]=sat_val[x^1]==-1;
  for (int i : implies[x]) if (!sat_vis[i]) sat_dfs_mark(i);
}
void sat_dfs(int x){
  sat_vis[x]=1;
  for (int i : implies[x]) if (!sat_vis[i]) sat_dfs(i);
  sat_sort[--sat_ile]=x^1;
}
bool sat2(int n) {//n - liczba zmiennych, zmienne numerujemy od 0
#define REP for (int i = 0; i < 2 * n; ++i)
  sat_ile=2 * n;
  REP sat_vis[i]=0,sat_val[i]=-1;
  REP if (!sat_vis[i]) sat_dfs(i);
  REP sat_vis[i]=0;
  REP if (!sat_vis[sat_sort[i]]) sat_dfs_mark(sat_sort[i]);
  REP if (sat_val[i]) for(int j : implies[i]) if (!sat_val[j]) return 0;
  return 1;
}
