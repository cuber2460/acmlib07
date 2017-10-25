/* DINIC O(n^2*m) lub O(MAXFLOW*(n+m)).                                       */
/* Dla grafów z krawędziami jednostykowymi działa w O(m*sqrt(n)).             */

using ll=long long;

struct flow
{
	int n, zr, uj;
	vector<vector<pair<int,pair<ll*,ll*>>>> graf;
	vector<vector<pair<int,pair<ll*,ll*>>>> dgraf;
	vector<vector<ll>> ori;//do odzyskiwania flowu
	vector<int> odl;
	flow(){}
	flow(int vertex_limit) {
		graf.resize(vertex_limit + 1);
		dgraf.resize(vertex_limit + 1);
		odl.resize(vertex_limit + 1, 0);
		ori.resize(vertex_limit + 1);//do odzyskiwania flowu
	}
	bool iszero(ll v) {
		return (!v);//zmienic dla dabli
	}
	void add_edge(int v, int u, ll lim, bool bi_dir=false) {
		ll *a=new ll(lim);
		ll *b=new ll(lim*bi_dir);
		graf[v].push_back({u,{a, b}});
		graf[u].push_back({v,{b, a}});
		ori[v].push_back(lim);//do odzyskiwania flowu
		ori[u].push_back(lim*bi_dir);//do odzyskiwania flowu
	}
	void bfs() {
		for (int i=1; i<=n; i++) {
			odl[i]=0;
			dgraf[i].clear();
		}
		queue <int> kol;
		kol.push(zr);
		odl[zr]=1;
		while(!kol.empty()) {
			int v=kol.front();
			kol.pop();
			for (auto i : graf[v]) {
				if (!odl[i.first] && !iszero(*i.second.first)) {
					odl[i.first]=odl[v]+1;
					kol.push(i.first);
				}
				if (odl[i.first]==odl[v]+1 && !iszero(*i.second.first))
					dgraf[v].push_back(i);
			}
		}
	}
	ll dfs(int v, ll lim) {
		if (v==uj)
			return lim;
		ll ret=0;
		for (int i=0; i<(int)dgraf[v].size() && !iszero(lim); i++) {
			auto &j=dgraf[v][i];
			ll wez=0;
			if (!iszero(*j.second.first))
				wez=dfs(j.first, min(*j.second.first, lim));
			if (iszero(wez)) {
				swap(dgraf[v][i], dgraf[v].back());
				dgraf[v].pop_back();
				i--;
				continue;
			}
			ret+=wez;
			(*j.second.first)-=wez;
			(*j.second.second)+=wez;
			lim-=wez;
		}
		return ret;
	}
	ll dinic(int nn, int zrzr, int ujuj) {
		n=nn;
		zr=zrzr;
		uj=ujuj;
		ll ret=0;
		while(1) {
			bfs();
			ll sta=ret;
			if (iszero(sta-(ret+=dfs(zr, 1e18))))//cos wiekszego niz maxflow, uwaga na overflow
				break;
		}
		return ret;
	}
	//ponizszych nie trzeba przepisywac
	void clear_memory() {
		for (int i=1; i<=n; i++)
			for (auto j : graf[i])
				delete(j.second.first);
	}
	//do ponizszych najpierw uzyc dinic()
	vector <int> cut() {
		vector <int> ret;
		bfs();
		for (int i=1; i<=n; i++)
			if (odl[i])
				ret.push_back(i);
		return ret;
	}
	map<pair<int,int>,ll> get_flowing() {//tam gdzie plynie 0 moze nie byc zainicjowane
		map<pair<int,int>,ll> ret;
		for (int i=1; i<=n; i++)
			for (int j=0; j<(int)graf[i].size(); j++)
				if ((*graf[i][j].second.first)<ori[i][j])
					ret[{i,graf[i][j].first}]+=ori[i][j]-(*graf[i][j].second.first);
		for (auto i : ret) {
			ll x=min(i.second, ret[{i.first.second,i.first.first}]);
			ret[i.first]-=x;
			ret[{i.first.second,i.first.first}]-=x;
		}
		return ret;
	}
};
