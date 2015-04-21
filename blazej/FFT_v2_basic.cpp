//testowana na:
//	http://codeforces.com/problemset/problem/286/E 4.5s/8s, 
//			wersja NTT 3^n za wolna ;_;


#include<cstdio>
#include<algorithm>
#include<vector>
#include<cstring>
#include<set>
#include<complex>
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
typedef double ld;
typedef pair<int,int> pii;
const int inf = 1e9 + 5;
const int nax = 1e5 + 5;
const int czapa = 2097152;

const ld PI = 3.1415926535897932384626433832795;
complex<ld> y2[czapa];

void fft(complex<ld> *a, complex<ld> *y, bool rev,
		int n, int p = 0, int s = 1, int q = 0) {
	
	if (n == 1) {
		y[q] = a[p];
		return;
	}
	
	complex<ld> e = 1, en = rev ? exp(complex<ld>(0,-2.*PI / (ld)n)) : exp(complex<ld>(0,2.*PI / (ld)n));
	fft(a, y, rev, n/2, p, s*2, q);
	fft(a, y, rev, n/2, p+s, s*2, q+n/2);
	REP(k,n/2) {
		y2[k] = y[q+k] + e*y[q+n/2+k];
		y2[n/2 + k] = y[q+k] - e*y[q+n/2+k];
		e *= en;
	}
	REP(k,n)
		y[q+k] = y2[k];
}

complex<ld> y[czapa], a[czapa];

void kw(int n, bool *v) {
	while (n&(n-1)) ++n; n *= 2;
	REP(i,n) a[i] = complex<ld>((ld)v[i],0.0);
	fft(a,y,false,n);
	REP(i,n) y[i] = y[i]*y[i];
	fft(y,a,true,n);
	REP(i,n) a[i] /= ld(n);
}

int n,m,x;
bool v[czapa+7];

int main(int argc, char * argv[]) {
	debug = argc > 1;
	
	scanf("%d%d",&n,&m);
	REP(i,n) {
		int x;
		scanf("%d",&x);
		v[x] = true;
	}
	
	kw(m+1,v);
	vector<int> res; 
	FOR(i,1,m) {
		if (v[i]) {
			if (round(a[i].real()) < 0.5) res.pb(i);
		}
		else {
			if (round(a[i].real()) > 0.5) {
				puts("NO");
				return 0;
			}
		}
	}
	puts("YES");
	printf("%d\n",(int)res.size());
	for (auto i: res) printf("%d ",i); puts("");
	return 0;
}
