const int nax=100*1007;
vector <int> graf[nax], farg[nax];
int ost[nax], bylo[nax], post[nax], spo[nax], counter, coudfs;
vector <vector<pair<int,int>>> mer;
void dfs1(int v) {
	if (bylo[v])
		return;
	bylo[v]=1;
	for (int i : graf[v]) dfs1(i);
	coudfs--;
	post[coudfs]=v;
}
void dfs2(int v, int s) {
	if (spo[v]>=0)
		return;
	spo[v]=s;
	for (int i : farg[v]) dfs2(i, s);
}
void rek(int l, int r, vector <pair<pair<int,int>,int>> &kra) {
	if (l>r) return;
	counter++;
	vector <int> ver;
	for (auto i : kra) {
		if (ost[i.first.first]<counter) {
			ver.push_back(i.first.first);
			ost[i.first.first]=counter;
		}
		if (ost[i.first.second]<counter) {
			ver.push_back(i.first.second);
			ost[i.first.second]=counter;
		}
	}
	for (int i : ver) {
		bylo[i]=0; spo[i]=-1;
		graf[i].clear(); farg[i].clear();
	}
	int s=(l+r)>>1;
	for (auto i : kra) {
		if (i.second<=s) {
			graf[i.first.first].push_back(i.first.second);
			farg[i.first.second].push_back(i.first.first);
		}
	}
	coudfs=ver.size();
	for (int i : ver)
		dfs1(i);
	for (int i=0; i<(int)ver.size(); i++)
		dfs2(post[i], post[i]);
	for (int i : ver)
		if (i!=spo[i])
			mer[s].push_back({i, spo[i]});
	vector <pair<pair<int,int>,int>> lew, pra;
	for (auto i : kra) {
		if (spo[i.first.first]==spo[i.first.second])
			lew.push_back(i);
		else
			pra.push_back({{spo[i.first.first],spo[i.first.second]}, i.second});
	}
	rek(l, s-1, lew); rek(s+1, r, pra);
}
void sss(vector <pair<int,int>> kra) {
	mer.clear();
	mer.resize(kra.size());
	vector <pair<pair<int,int>,int>>daj;
	for (int i=0; i<(int)kra.size(); i++) {
		daj.push_back({kra[i], i});
		ost[kra[i].first]=-1;
		ost[kra[i].second]=-1;
	}
	counter=0;
	rek(0, (int)kra.size()-1, daj);
}
