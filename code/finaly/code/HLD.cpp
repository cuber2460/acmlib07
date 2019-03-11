vector <int> drz[nax];
int roz[nax], jump[nax], pre[nax], post[nax], fad[nax], czas;
void dfs_roz(int v) {
	roz[v]=1;                        // drz[] ma nie zawierać krawędzi
	for (int &i : drz[v]) {          // do ojca.
		fad[i]=v;                    // Init:
		dfs_roz(i);                  // dfs_roz(root);
		roz[v]+=roz[i];              // dfs_pre(root);
		if (roz[i]>roz[drz[v][0]])   // Użycie get_path(v, u) zwróci
			swap(i, drz[v][0]);      // przedziały odpowiadające ścieżce
	}                                // z v do u. Przedziały odpowiadające
}                                    // ścieżce z v do lca mają
void dfs_pre(int v)                  // first>=second, zaś te dla ścieżki
{                                    // z lca do u mają first<=second.
	if (!jump[v])                    // Przedziały są po kolei.
		jump[v]=v;                   // Lca występuje w nich dwa razy,
	pre[v]=(++czas);                 // najpierw jako second,
	if (!drz[v].empty())             // a zaraz potem jako first.
		jump[drz[v][0]]=jump[v];
	for (int i : drz[v])
		dfs_pre(i);
	post[v]=czas;
}
int lca(int v, int u) {
	while(jump[v]!=jump[u]) {
		if (pre[v]<pre[u])
			swap(v, u);
		v=fad[jump[v]];
	}
	return (pre[v]<pre[u] ? v : u);
}
vector < pair <int,int> > path_up(int v, int u) {
	vector < pair <int,int> > ret;
	while(jump[v]!=jump[u]) {
		ret.push_back({pre[jump[v]], pre[v]});
		v=fad[jump[v]];
	}
	ret.push_back({pre[u], pre[v]});
	return ret;
}
vector < pair <int,int> > get_path(int v, int u) {
	int w=lca(v, u);
	auto ret=path_up(v, w);
	auto pom=path_up(u, w);
	for (auto &i : ret)
		swap(i.first, i.second);
	while(!pom.empty()) {
		ret.push_back(pom.back());
		pom.pop_back();
	}
	return ret;
}
