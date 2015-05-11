#define MAXN 5007
#define RMAX (MAXN+MAXN/2)

using namespace std;

typedef long long LL; 

int ojc[RMAX],drzewo[RMAX],skoj[RMAX];
queue<int> zew;
queue<pair<int,int> > zew_kraw;
vector<int> v[RMAX],cykl[MAXN/2], sciezka, sciezka_pom;
bool bylo[RMAX],akt[RMAX],czy_zew[RMAX];

void dfs1(int x, int kon, vector<int> &v) {
	if (x == kon) return;
	dfs1(ojc[x],kon,v);
	v.PB(x);
}

void dfs2(int x, int kon, vector<int> &v) {
	if (x == kon) return;
	v.PB(x);
	dfs2(ojc[x],kon,v);
}

void dfs3(int x, bool v) {
	if (x == -1) return;
	bylo[x] = v;
	dfs3(ojc[x],v);
}

void AddEdge(int a, int b) {
	v[a].PB(b);
	v[b].PB(a);
}

void rozwin(int x, int n) {
	if (skoj[x]) {
		skoj[skoj[x]] = cykl[x-n-1].back();
		skoj[cykl[x-n-1].back()] = skoj[x];
	}
	cykl[x-n-1].clear();
}

void Edmonds(int n) {
	//heureza
	srand(n);
	FOR(i,1,n) if (!skoj[i]) {
		random_shuffle(ALL(v[i]));
		FOREACH(it,v[i]) if (!skoj[*it]) {
			skoj[*it] = i;
			skoj[i] = *it;
			break;
		}
	}
	
	int ile = n;
	bool dasie = true;
	while (dasie) {
		dasie = false;
		
		//czyszczenie
		while (!zew_kraw.empty()) zew_kraw.pop();
		while (!zew.empty()) zew.pop();
		FOR(i,1,n) {
			akt[i] = true; ojc[i] = 0; drzewo[i] = 0; czy_zew[i] = false;
			while (!v[i].empty() && v[i].back() > n) v[i].pop_back();
		}
		
		FOR(i,1,n) if (!skoj[i]) {
			ojc[i] = -1; czy_zew[i] = true;
			drzewo[i] = i; zew.push(i);
		} 
		
		while ((!zew.empty() || !zew_kraw.empty()) && !dasie) {
			if (!zew.empty()) {
				int x = zew.front();
				zew.pop();
				//printf("badamy zewnetrzny wierzcholek %d\n",x); 
			
				FOREACH(it,v[x]) if (ojc[*it] == 0) {
					ojc[*it] = x;
					ojc[skoj[*it]] = *it;
					drzewo[*it] = drzewo[skoj[*it]] = drzewo[x];
					zew.push(skoj[*it]);
					czy_zew[skoj[*it]] = true;
					//printf("dodajemy %d %d\n",*it,skoj[*it]);
				}
				else if (czy_zew[*it] && akt[*it]) zew_kraw.push(MP(x,*it));
			}
			else {
				int x = zew_kraw.front().ST;
				int y = zew_kraw.front().ND;
				zew_kraw.pop();
				if (akt[x] && akt[y]) {
					//printf("zewnetrzne wierzcholki %d %d sa polaczone\n",x,y);
					if (drzewo[x] != drzewo[y]) {
						//puts("sciezka");
						dasie = true;
					
						sciezka.clear(); 
						dfs1(x,-1,sciezka);
						dfs2(y,-1,sciezka);
						
						FORD(i,ile,n+1) {
							//FOREACH(it,sciezka) printf("%d ",*it); puts("");
							if (skoj[i]) {
								skoj[skoj[i]] = cykl[i-n-1].back();
								skoj[cykl[i-n-1].back()] = skoj[i];
							}
							int dl = sciezka.size();
							REP(j,dl) if (sciezka[j] == i) {
								int dlug = int(cykl[i-n-1].size());
								if (j&1) {
									int poz = -1;
									REP(k,dlug) FOREACH(it2,v[cykl[i-n-1][k]]) 
										if (*it2 == sciezka[j-1]) poz = k;
									if ((dlug-poz)&1) FOR(k,poz,dlug-1) sciezka_pom.PB(cykl[i-n-1][k]);
									else {
										FORD(k,poz,0) sciezka_pom.PB(cykl[i-n-1][k]);
										sciezka_pom.PB(cykl[i-n-1][dlug-1]);
									} 
								}
								else {
									sciezka_pom.PB(cykl[i-n-1].back());
									int poz = -1;
									REP(k,dlug) FOREACH(it2,v[cykl[i-n-1][k]]) 
										if (*it2 == sciezka[j+1]) poz = k;
									if (poz != dlug-1) {
										if (poz&1) FOR(k,0,poz) sciezka_pom.PB(cykl[i-n-1][k]); 
											else FORD(k,dlug-2,poz) sciezka_pom.PB(cykl[i-n-1][k]); 
									}
								}
							}
							else sciezka_pom.PB(sciezka[j]);
							
							cykl[i-n-1].clear(); sciezka = sciezka_pom;
							sciezka_pom.clear();
						}
						//FOREACH(it,sciezka) printf("%d ",*it); puts("");
						int dl = sciezka.size();
						REP(i,dl/2) {
							skoj[sciezka[2*i]] = sciezka[2*i+1];
							skoj[sciezka[2*i+1]] = sciezka[2*i];
						}  
						ile = n;
					}
					else {
						++ile;
						//printf("cykl %d\n",ile);
						czy_zew[ile] = akt[ile] = true; zew.push(ile);
						ojc[ile] = -1; drzewo[ile] = drzewo[x];
						
						dfs3(x,true); int kon = y; 
						while (!bylo[kon]) kon = ojc[kon];
						dfs3(x,false);
						
						int wsk = kon; skoj[wsk] = 0;
						int oj = -1;
						while (ojc[wsk] != -1) {
							int pom1 = ojc[wsk]; int pom2 = ojc[pom1];
							skoj[pom1] = pom2; skoj[pom2] = pom1;
							ojc[pom1] = wsk; ojc[wsk] = oj;
							wsk = pom2; oj = pom1;
						}
						ojc[wsk] = oj;
						
						dfs1(x,kon,cykl[ile-n-1]);
						dfs2(y,kon,cykl[ile-n-1]);
						cykl[ile-n-1].PB(kon);
						FOREACH(it,cykl[ile-n-1]) {
							akt[*it] = false;
							FOREACH(it2,v[*it]) bylo[*it2] = true;
						}
						FOR(i,1,ile-1) {
							if (bylo[i]) {
								bylo[i] = false;
								if (akt[i]) {
									AddEdge(i,ile);
									if (drzewo[i] == drzewo[ile] && !akt[ojc[i]]) ojc[i] = ile;
								}
							}
						}
					}	
				}
			}
		}
		FORD(i,ile,n+1) rozwin(i,n);
		ile = n;
	}
}

int main(){
	int n,m,a,b;
	scanf("%d%d",&n,&m);
	REP(i,m) {
		scanf("%d%d",&a,&b);
		AddEdge(a,b);
	}
	Edmonds(n);
	puts("");
	FOR(i,1,n) printf("%d %d\n",i,skoj[i]);
	return 0;
}
