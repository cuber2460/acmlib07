struct MinCost {
	struct kra {
		int cel, *prze1, *prze2;
		ll koszt;
	};
	int n=0, zr, uj;
	const ll inf=1e9;
	vector <vector <kra>> graf;
	vector <int> bylo, aktu;
	vector <ll> odl, pamodl;
	void vert(int v) {
		if (v>n) {
			n=v;
			graf.resize(n);
			bylo.resize(n);
			aktu.resize(n);
			odl.resize(n);
			pamodl.resize(n);
		}
	}
	void add_edge(int v, int u, int prze, ll koszt) {
		vert(v+1); vert(u+1);
		kra ret1{u, new int(prze), new int(0), koszt};
		kra ret2{v, ret1.prze2, ret1.prze1, -koszt};
		graf[v].push_back(ret1);
		graf[u].push_back(ret2);
	}
	void spfa() {
		for (int i=0; i<n; i++) {
			aktu[i]=1;
			pamodl[i]=inf;
		}
		aktu[zr]=pamodl[zr]=0;
		queue <int> kol;
		kol.push(zr);
		while(!kol.empty()) {
			int v=kol.front();
			kol.pop();
			if (aktu[v])
				continue;
			aktu[v]=1;
			for (kra i : graf[v]) {
				if (*i.prze1 && pamodl[v]+i.koszt<pamodl[i.cel]) {
					pamodl[i.cel]=pamodl[v]+i.koszt;
					aktu[i.cel]=0;
					kol.push(i.cel);
				}
			}
		}
	}
	void dij() {
		for (int i=0; i<n; i++)
			odl[i]=inf;
		priority_queue < pair <ll,int> > dijks;
		dijks.push({0, zr});
		while(!dijks.empty()) {
			ll dis=-dijks.top().first;
			int v=dijks.top().second;
			dijks.pop();
			if (odl[v]!=inf)
				continue;
			odl[v]=pamodl[v]+dis;
			for (auto j : graf[v])
				if ((*j.prze1) && odl[j.cel]==inf)
					dijks.push({-(dis+pamodl[v]-pamodl[j.cel]+j.koszt), j.cel});
		}
	}
	int dfs(int v) {
		if (v==uj)
			return 1;
		bylo[v]=1;
		for (int i=0; i<(int)graf[v].size(); i++) {
			if (!bylo[graf[v][i].cel] && (*graf[v][i].prze1) &&
			odl[v]+graf[v][i].koszt==odl[graf[v][i].cel] && dfs(graf[v][i].cel)) {
				(*graf[v][i].prze1)--;
				(*graf[v][i].prze2)++;
				return 1;
			}
		}
		return 0;
	}
	pair <int,ll> flow(int zrzr, int ujuj) {
		zr=zrzr; uj=ujuj;
		vert(zr+1); vert(uj+1);
		spfa();
		pair <int,ll> ret{0, 0};
		while(1) {
			dij();
			for (int i=0; i<n; i++)
				bylo[i]=0;
			if (!dfs(zr))
				break;
			ret.first++;
			ret.second+=odl[uj];
		}
		return ret;
	}
};
