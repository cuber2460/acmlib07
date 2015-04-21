//sprawdzone:
//	http://main.edu.pl/pl/archive/pa/2012/ren 0.28s/25s
//	http://community.topcoder.com/stat?c=problem_statement&pm=7726&rd=10787
//	http://community.topcoder.com/stat?c=problem_statement&pm=8143&rd=10789

#include<cstdio>
#include<algorithm>
#include<vector>
#include<cstring>
#include<set>
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
const int nax = 607;

//maksymalnego skojarzenia, indeksowanie od 1, wszystko na intach
//w skojx[] bedzie skojarzenie

namespace Hung{
	int n;
	int m[nax][nax];
	int skojx[nax], skojy[nax];
	int lx[nax], ly[nax];
	bool S[nax], T[nax];
	int q[nax], qbeg, qend;
	int slack[nax], prev[nax];
	
	void init(int size) {
		n = size;
		FOR(i,1,n) FOR(j,1,n) m[i][j] = -inf; 
		FOR(i,1,n) skojx[i] = skojy[i] = 0;
	}
	
	void AddEdge(int a, int b, int c) {
		m[a][b] = max(m[a][b], c);
	}
	
	int hungarian() {
		FOR(i,1,n) {
			ly[i] = 0; lx[i] = -inf;
			FOR(j,1,n) lx[i] = max(lx[i], m[i][j]);
		}
		
		REP(k,n) {
			qbeg = qend = 0;
			FOR(i,1,n) S[i] = T[i] = false;
			FOR(i,1,n) if (!skojx[i]) {
				q[qend++] = i; break;
			}
			
			S[q[0]] = true;
			FOR(i,1,n) slack[i] = lx[q[0]] + ly[i] - m[q[0]][i], prev[i] = q[0];
			
			int y = 0;
			while (true) {
				while (qbeg < qend) {
					int x = q[qbeg++]; S[x] = true;
					FOR(i,1,n) if (!T[i]) {
						if (slack[i] > lx[x] + ly[i] - m[x][i]) {
							slack[i] = lx[x] + ly[i] - m[x][i];
							prev[i] = x;
						}
						if (lx[x] + ly[i] == m[x][i]) {
							T[i] = true;
							if (skojy[i])
								q[qend++] = skojy[i];
							else {
								y = i; goto koniec;
							}
						}
					}
				}
				
				int minslack = inf;
				FOR(i,1,n) if (!T[i]) minslack = min(minslack, slack[i]);
				FOR(i,1,n) {
					if (S[i]) lx[i] -= minslack;
					if (T[i]) 
						ly[i] += minslack;
					else
						slack[i] -= minslack;
				}
				FOR(i,1,n) if (!T[i] && !slack[i]) {
					T[i] = true;
					if (skojy[i]) {
						q[qend++] = skojy[i];
					}
					else {
						y = i; goto koniec;
					}
				}
			}
		
		koniec:
			while (y) { 
				skojy[y] = prev[y]; 
				int pom = skojx[prev[y]];
				skojx[prev[y]] = y;
				y = pom; 
			}
		}
		
		int res = 0;
		FOR(i,1,n) res += m[i][skojx[i]];
		return res;	
	}
}

int n,m;
bool in[nax], out[nax];
int cost[nax][nax];

int main(int argc, char * argv[]) {
	debug = argc > 1;

    scanf("%d%d",&n,&m);
    for (int i = 0; i < 2 * n; i++) {
        for (int j = 0; j < 2 * n; j++) {
            if (i >= n && j >= n) {
                cost[i][j] = 0;
            } else {
                cost[i][j] = -inf;
            }
        }
    }
    for (int i = 0; i < m; i++) {
        int x,y,k;
        scanf("%d%d%d", &x, &y, &k);
        cost[x-1][y-1] = -k;
        in[x-1] = true;
        out[y-1] = true;
    }
    for (int i = 0; i < n; i++) {
        if (!in[i] || !out[i]) {
            puts("NIE");
            return 0;
        }
    }
    for (int i = 0; i < n; i++) {
        int outcost = -inf;
        int incost = -inf;
        for (int j = 0; j < n; j++) {
            outcost = max(outcost, cost[i][j]);
            incost = max(incost, cost[j][i]);
        }
        for (int j = n; j < 2 * n; j++) {
            cost[i][j] = outcost;
            cost[j][i] = incost;
        }
    }
    n = 2 * n;
    Hung::init(n);   
    REP(i,n) REP(j,n) {
		Hung::AddEdge(i+1,j+1,cost[i][j]);
	}
	
	printf("%d\n",-Hung::hungarian());
    return 0;
}
