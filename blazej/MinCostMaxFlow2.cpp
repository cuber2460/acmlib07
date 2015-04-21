#include<cstdio>
#include<algorithm>
#include<vector>
#include<cstring>
#include<set>
#include<queue>
#include<assert.h>
using namespace std;
#define FOR(i,a,b) for(int i = a; i <= b; ++i)
#define FORD(i,a,b) for(int i = a; i >= b; --i)
#define REP(i,n) FOR(i,0,(n)-1)
#define RI(i,n) FOR(i,1,n)
#define pb push_back
#define mp make_pair
#define st first
#define nd second
#define mini(a,b) a=min(a,b)
#define maxi(a,b) a=max(a,b)
bool debug;
typedef vector<int> vi;
typedef long long ll;
typedef long double ld;
typedef pair<int,int> pii;
const int inf = 1e9 + 5;
const int nax = 6405;

//sprawdzone:
//	http://main.edu.pl/pl/archive/pa/2012/ren 4.5s/25s
//	http://codeforces.com/contest/321/problem/B 0.06/2s
//	http://codeforces.com/problemset/problem/277/E 1s/5s

//c - capacity, v - value
//wierzcholki sa od 0 do n, wszystko na intach 
//wiec wtedy musisz ogarnac odlegloci i takie tam
namespace MinCost{	
	struct Edge{
		int w,c,v,rev;
		Edge(int _w, int _c, int _v, int _rev) :
			w(_w), c(_c), v(_v), rev(_rev)
		{}
	};
	
	int odl[nax], pot[nax], pop[nax], pop_kraw[nax];
	int q[nax], qbeg, qend;
	vector<Edge> v[nax];
	bool bylo[nax];
	queue<int> kolej;
	//priority_queue<pair<int,int> > kolej;
	
	void init(int n) {
		FOR(i,0,n) v[i].clear();
	}
	
	void AddEdge(int a, int b, int cap, int cost) {
		v[a].pb(Edge(b,cap,cost,int(v[b].size()) + (a == b)));
		v[b].pb(Edge(a,0,-cost,int(v[a].size()-1)));
	}
	
	pair<int,int> MinCostMaxFlow(int s, int t, int n) {
		int flow = 0, cost = 0;
		//FOR(i,0,n) pot[i] = 0; 
		//potencjaly zerowe dzialaja dla samych nieujemnych kosztow
		//jak sa ujemne krawedzie to bellman
		
		while (true) {
			FOR(i,0,n) {
				odl[i] = inf;
				bylo[i] = false;
			}
			bylo[s] = true;
			odl[s] = 0;
			kolej.push(s);
			
			//djikstra, mozna napisac na kolejce
			while(!kolej.empty()) {
				int x = kolej.front();
				kolej.pop();
				bylo[x] = false;

				//if (bylo[x])
				//	continue;
				
				/*int najm = inf;
				FOR(i,0,n) if (!bylo[i] && najm > odl[i]) {
					x = i; najm = odl[i];
				}*/
				
				//bylo[x] = true;
				int dl = v[x].size();
				REP(i,dl) if (v[x][i].c > 0 && odl[v[x][i].w] > odl[x] + pot[x] - pot[v[x][i].w] + v[x][i].v) {
					odl[v[x][i].w] = odl[x] + pot[x] - pot[v[x][i].w] + v[x][i].v;
					if (!bylo[v[x][i].w]) {
						kolej.push(v[x][i].w);
						bylo[v[x][i].w] = true;
					}
					//kolej.push(mp(-odl[v[x][i].w], v[x][i].w));
					pop[v[x][i].w] = x; pop_kraw[v[x][i].w] = i; 
				} 
			}
			
			if (odl[t] == inf)
				break;
			
			//FOR(i,0,n) pot[i] += odl[i];
			
			int x = t;
			int cap = inf;
			while (x != s) {
				cap = min(cap, v[pop[x]][pop_kraw[x]].c);
				x = pop[x];
			}
			
			flow += cap;
			x = t;
			while (x != s) {
				cost += v[pop[x]][pop_kraw[x]].v*cap;
				v[pop[x]][pop_kraw[x]].c -= cap;
				v[x][v[pop[x]][pop_kraw[x]].rev].c += cap;
				x = pop[x];
			}
		}
		
		return mp(flow, cost);
	}
};

using namespace MinCost;

const int naxn = 87;

int n,m;
int t[naxn][naxn];

int main(int argc, char * argv[]) {
	debug = argc > 1;
    scanf("%d%d",&n,&m);
    init(n*m+1);
    FOR(i,1,n) FOR(j,1,m)
		scanf("%d",&t[i][j]);
	
	FOR(i,1,n) FOR(j,1,m) {
		int val = (i-1)*m + j;
		if ((i+j)&1) {
			MinCost::AddEdge(0,val,1,0);
			if (i > 1)
				AddEdge(val, val-m, 1, t[i][j] != t[i-1][j]);
			if (j > 1)
				AddEdge(val, val-1, 1, t[i][j] != t[i][j-1]);
			if (i < n)
				AddEdge(val, val+m, 1, t[i][j] != t[i+1][j]);
			if (j < m)
				AddEdge(val, val+1, 1, t[i][j] != t[i][j+1]);
		}
		else
			MinCost::AddEdge(val,n*m+1,1,0);
	}
	printf("%d\n",MinCost::MinCostMaxFlow(0,n*m+1,n*m+1).nd);
    return 0;
}
