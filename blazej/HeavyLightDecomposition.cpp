//dziala dla n >= 2
#define czapa 131072
#define MAXN 100007

int n,a,b,k;
int ile[MAXN],gleb[MAXN],wyn[MAXN];
int ojc[MAXN];

int ilosc;
int id[MAXN], st[MAXN], heavy[MAXN], fat[MAXN];
int d[2*czapa];

vector<PII> v[MAXN];

int find(int f) {
	if (fat[f] == f) return f;
	fat[f] = find(fat[f]);
	return fat[f];
}

void dfs(int x, int f) {
	int kt = -1, najw = 0;
	
	ile[x] = 1; ojc[x] = f;
	gleb[x] = gleb[f] + 1;
	
	FOREACH(it,v[x]) if (it->ST != ojc[x]) {
		dfs(it->ST,x);
		ile[x] += ile[it->ST];
		if (ile[it->ST] > najw) {
			najw = ile[it->ST];
			kt = it->ST;
		}
	}
	
	heavy[x] = kt;
	if (kt != -1)
		fat[kt] = x;
}

int lca(int u, int v) {
	while (true) {
		int a = find(u);
		int b = find(v);
		//printf("%d %d %d %d\n",u,v,a,b);
		if (a == b) 
			return gleb[u] < gleb[v] ? u : v;
		else {
			if (gleb[a] > gleb[b])
				u = ojc[a]; 
			else
				v = ojc[b];
		}
	}
}

void insert3(int a, int b) {
	//printf("insercik %d %d\n",a,b);
	a += czapa;
	b += czapa;
	
	++d[a];
	if (a != b) 
		++d[b];
	
	//printf("%d %d\n",a,b);
	while (a/2 != b/2) {
		//puts("KK");
		//printf("%d %d\n",a,b);
		if (!(a&1)) ++d[a^1];
		if (b&1) ++d[b^1];
		
		a >>= 1;
		b >>= 1;
	}
}

int query2(int a) {
	//printf("query2 %d\n",a);
	a += czapa;
	int res = 0;

	while (a) {
		res += d[a];
		a >>= 1;
	}
	
	return res;
}

void insert2(int d, int g) {
	while (d != g) {
		if (heavy[ojc[d]] == d) {
			int p = find(d);
			if (gleb[p] < gleb[g]) p = g;
			insert3(id[ojc[d]],id[p]);
			d = p;
		}
		else {
			++st[d];
			d = ojc[d];
		}
	}
}

void insert(int a, int b) {
	int p = lca(a,b);
	insert2(a,p);
	insert2(b,p);
}

int query(int x) {
	if (heavy[ojc[x]] != x) {
		return st[x]; 
	}
	return query2(id[ojc[x]]);
}

void dfs2(int x) {
	FOREACH(it,v[x]) if (it->ST != ojc[x]) {
		dfs2(it->ST);
		wyn[it->ND] = query(it->ST);
	}
}

void prepare() {
	FOR(i,1,n) fat[i] = i;
	dfs(1,0);
	//FOR(i,1,n) printf("%d ",fat[i]); puts("");
	
	int wsk = 1;
	FOR(i,1,n) if (heavy[i] == -1) {
		int x = i;
		while (heavy[ojc[x]] == x) {
			x = ojc[x];
			id[x] = wsk++;
		}
	}
}

int main(){
	scanf("%d",&n);
	REP(i,n-1) {
		scanf("%d%d",&a,&b);
		v[a].PB(MP(b,i));
		v[b].PB(MP(a,i));
	}
	prepare();
	
	scanf("%d",&k);
	REP(i,k) {
		scanf("%d%d",&a,&b);
		insert(a,b);
	}
	
	dfs2(1);	
	REP(i,n-1) printf("%d ",wyn[i]); puts("");
	return 0;
}
