const int N=100000;
VI implies[2*N]; //wymuszenia, 2*x to zmienna 2*x+1 to jej zaprzeczenie
int sat_val[2*N],sat_vis[2*N],sat_sort[2*N],sat_ile;

inline void sat_or(int a,int b){
  implies[a^1].PB(b);
  implies[b^1].PB(a);
}

void sat_dfs_mark(int x){
  sat_vis[x]=1; sat_val[x]=sat_val[x^1]==-1;
  FOREACH(it,implies[x]) if (!sat_vis[*it]) sat_dfs_mark(*it);
}

void sat_dfs(int x){
  sat_vis[x]=1;
  FOREACH(it,implies[x^1]) if (!sat_vis[*it^1]) sat_dfs(*it^1);
  sat_sort[sat_ile++]=x;
}

/*przekazujemy liczbe zmiennych*/
int sat2(int n){
  sat_ile=0;
  REP(i,2*n) sat_vis[i]=0,sat_val[i]=-1;
  REP(i,2*n) if (!sat_vis[i]) sat_dfs(i);
  REP(i,2*n) sat_vis[i]=0;
  FORD(i,2*n-1,0) if (!sat_vis[sat_sort[i]]) sat_dfs_mark(sat_sort[i]);
  REP(i,2*n) if (sat_val[i]) FOREACH(it,implies[i]) if (!sat_val[*it]) return 0;
  return 1;
}

 //Begin of the code
int main(){
  int n=3;
  sat_or(2*0,2*1+1); //x_0 or !x_1
  sat_or(2*2+1,2*1); //!x_2 or x_1 
  printf("%d\n",sat2(n));
  REP(i,2*n) printf("%d ",sat_val[i]);
  printf("\n");
  return 0;
}
