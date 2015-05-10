const int inf = 1e9 + 5;
const int nax = 1e3 + 5;

namespace Flow {
	
int odl[nax], q[nax];
int m[nax][nax];
vector<int>::iterator pocz[nax];
vector<int> v[nax];

void init(int n) {
	//printf("init %d\n",n);
	FOR(i,0,n) v[i].clear();
	FOR(i,0,n) FOR(j,0,n) m[i][j] = 0;
}

void AddEdge(int a, int b, int c) {
	//printf("dodaj krawedz od %d do %d za %d\n",a,b,c);
	v[a].pb(b);
	v[b].pb(a);
	m[a][b] += c;
}

bool bst(int s, int t, int n) {
	FOR(i,0,n) odl[i] = inf;
	odl[s] = 0;
	
	int qbeg = 0, qend = 0;
	q[qend++] = s;
	while (qbeg < qend) {
		int x = q[qbeg++];
		for (auto j: v[x]) if (m[x][j] > 0 && odl[j] == inf) {
			odl[j] = odl[x] + 1;
			q[qend++] = j;
		}
	}
	
	return odl[t] != inf;
}

int flow(int x, int t, int maximum) {
	if (x == t || maximum == 0)
		return maximum;
	
	int res = 0;
	for (vector<int>::iterator &it = pocz[x]; it != v[x].end(); it++) 
		if (m[x][*it] > 0 && odl[*it] == odl[x] + 1) {
			int y = flow(*it,t,min(maximum,m[x][*it]));
			maximum -= y;
			m[x][*it] -= y;
			m[*it][x] += y;
			res += y;
			
			if (maximum == 0)
				return res;
		}
	
	return res;
}

int MaxFlow(int s, int t, int n) {
	int res = 0;
	while (bst(s,t,n)) {
		FOR(i,0,n) pocz[i] = v[i].begin();
		res += flow(s,t,inf);
	}
	return res;
}

}

int main(int argc, char * argv[]) {
	return 0;
}
