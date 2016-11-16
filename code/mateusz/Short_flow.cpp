/* FLOW O(MAXFLOW*(n+m)).                                                     */
/* Wolny, njbardziej pałowy, nadaje się, gdy wartość maksymalnego przepływu   */
/* jest mała (np. rzędu n), krótszy kod niż w przypadku dinica,               */
/* fclear() nie trzeba przepisywać jeśli liczymy flow tylko raz, wynik        */
/* do odzyskania jest w fflow[][]                                             */
const int fn=1007;
vector <int> fgraf[fn];
int flim[fn][fn];
int fflow[fn][fn];
int fstart[fn][fn];
bool fbylo[fn];
//dodaje krawędź z v do u o przepustowości lim, v!=u
inline void add_edge(int v, int u, int lim, bool bi_dir)
{
	fgraf[v].push_back(u);
	fgraf[u].push_back(v);
	flim[u][v]=bi_dir*(flim[v][u]=lim);
	fstart[u][v]=bi_dir*(fstart[v][u]=lim);
}
int fdfs(int v, int u, int mini)
{
	if (fbylo[v] || !mini) return 0;
	if (v==u) return mini;
	fbylo[v]=1;
	for (int c : fgraf[v])
	{
		int pom=fdfs(c, u, min(mini, flim[v][c]));
		if (pom)
		{
			flim[v][c]-=pom;
			flim[c][v]+=pom;
			return pom;
		}
	}
	return 0;
}
//liczba wierzchołków, numer źródła i ujścia
//wszystko indeksowane od zera
int go_flow(int n, int v, int u)
{
	int ret=0, pom=0;
	while(pom!=(ret+=fdfs(v, u, 1000*1000*1000)))//stała większa niż jakakolwiek przpustowość
	{
		pom=ret;
		for (int i=1; i<=n; i++)
			fbylo[i]=0;
	}
	for (int i=1; i<=n; i++)
		for (int j=1; j<=n; j++)
			fflow[i][j]=max(0, fstart[i][j]-flim[i][j]);//TODO tu nie działa odzyskiwanie
	return ret;
}
//jeśli chcemy więcej razy puszczać flow,
//to czyścimy tym aktualne wartości
//rem_edges oznacza, czy chcemy usunąć krawędzie
void fclear(int n, bool rem_edges)
{
	for (int i=1; i<=n; i++)
		for (int j=1; j<=n; j++)
			flim[i][j]=fflow[i][j]=fbylo[i]=0;
	if (rem_edges)
		for (int i=1; i<=n; i++)
			fgraf[i].clear();
}
