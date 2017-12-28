/* HEAVY LIGHT DECOMPOSITION                                                  */
/* Wypełniamy drz listą dzieci każdego wierzchołka, po czym odpalamy          */
/* dfs_roz(root) i dfs_pre(root).                                             */
/* Użycie get_path(v, u) zwróci przedziały odpowiadające ścieżce z v do u.    */
/* Przedziały odpowiadające ścieżce z v do lca mają first>=second, zaś        */
/* te dla ścieżki z lca do u mają first<=second, przedziały są po kolei,      */
/* lca występuje tam dwa razy, najpierw jako second, a zaraz potem jako first.*/

const int nax=100*1007;

vector <int> drz[nax];
int roz[nax], jump[nax], pre[nax], post[nax], fad[nax];
int l;

void dfs_roz(int v)
{
	roz[v]=1;
	for (int &i : drz[v])
	{
		fad[i]=v;
		dfs_roz(i);
		roz[v]+=roz[i];
		if (roz[i]>roz[drz[v][0]])
			swap(i, drz[v][0]);
	}
}

void dfs_pre(int v)
{
	if (!jump[v])
		jump[v]=v;
	l++;
	pre[v]=l;
	if (!drz[v].empty())
		jump[drz[v][0]]=jump[v];
	for (int i : drz[v])
		dfs_pre(i);
	post[v]=l;
}

int lca(int v, int u)
{
	while(jump[v]!=jump[u])
	{
		if (pre[v]<pre[u])
			swap(v, u);
		v=fad[jump[v]];
	}
	return (pre[v]<pre[u] ? v : u);
}

vector < pair <int,int> > path_up(int v, int u)
{
	vector < pair <int,int> > ret;
	while(jump[v]!=jump[u])
	{
		ret.push_back({pre[jump[v]], pre[v]});
		v=fad[jump[v]];
	}
	ret.push_back({pre[u], pre[v]});
	return ret;
}

vector < pair <int,int> > get_path(int v, int u)
{
	int w=lca(v, u);
	auto ret=path_up(v, w);
	auto pom=path_up(u, w);
	for (auto &i : ret)
		swap(i.first, i.second);
	while(!pom.empty())
	{
		ret.push_back(pom.back());
		pom.pop_back();
	}
	return ret;
}
