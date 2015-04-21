//testowana na:
//	http://codeforces.com/problemset/problem/286/E 1.7s/8s
//	http://codeforces.com/problemset/problem/300/D 0.186s/3s
//	http://codeforces.com/contest/472/problem/G 1.3s/7s
//	http://codeforces.com/contest/528/problem/D 0.5s/3s

#include<cstdio>
#include<algorithm>
#include<vector>
#include<cstring>
#include<set>
#include<cmath>
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
const int mod = 998244353; //mod - 1 = 2^23 * 7 * 17
const int generator = 15311432; // == 3 ** (7 * 17) 
const int maxczapa = 1<<23;

const ld PI = 3.1415926535897932384626433832795;

struct zespo{
	ld re, im;
	zespo(ld r=0., ld i=0.) : re(r), im(i) {}

	zespo operator+(zespo const& x) const {
		return zespo(re+x.re,im+x.im);
	}
	
	zespo operator-(zespo const& x) const {
		return zespo(re-x.re,im-x.im);
	}
	
	zespo operator*(zespo const& x) const {
		return zespo(re*x.re-im*x.im,re*x.im+im*x.re);
	}
	
	zespo operator/(ld d) const {
		return zespo(re/d,im/d);
	}
};

void fft(zespo *a, bool rev, int n) {
	//najnizszy krok rekurencyjny, symulujemy dzialanie rekurencyjnej 
	//funckji i w ten sposob wilyczamy cos sie bedzie odpierdzielac
	//nie ogarniam jak to dziala, ale jest szybkie
	for (int i=1, j=0; i<n; i++) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}
	
	//wersja ktora ogarniam, troszeczke wolniejsza, (przewal bloki i rekurencyjnie nizej)
	/*for (int przedzial = n >> 1, blok = 1; blok < przedzial; przedzial >>= 1, blok <<= 1) {
		for (int i = przedzial; i < n; i += przedzial<<1)
			for (int j = i; j < i+przedzial; j += blok<<1)
				for (int k = j; k < j+blok; k++)
					swap(a[k - przedzial + blok], a[k]);
	}*/
	
	
	//wykonujemy rekurencje od dolu
	for (int len=2; len<=n; len<<=1) {
		ld kat = 2.0*PI/len * (rev ? -1 : 1);
		zespo pierw(cos(kat),sin(kat)); 
		for (int i=0; i<n; i+=len) {
			zespo w(1,0);
			for (int j=0; j<len/2; ++j) {
				zespo u = a[i+j], v = a[i+j+len/2]*w;
				a[i+j] = u + v;
				a[i+j+len/2] = u - v;
				w = w * pierw;
			}
		}
	}
	
	if (rev) 
		for(int i=0;i<n;i++)		
			a[i] = a[i] / n;
}

zespo a[czapa];

void kw(int n, bool *v) {
	while (n&(n-1)) ++n; n *= 2;
	REP(i,n) a[i] = zespo((ld)v[i],0.0);
	fft(a,false,n);
	REP(i,n) a[i] = a[i]*a[i];
	fft(a,true,n);
}

int n,m,x;
bool v[czapa];

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
			if (round(a[i].re) < 0.5) res.pb(i);
		}
		else {
			if (round(a[i].re) > 0.5) {
				puts("NO");
				return 0;
			}
		}
	}
	puts("YES");
	
	int x = (int)res.size();
	printf("%d\n",(int)res.size());
	REP(i,x) printf("%d ",res[i]); puts("");
	return 0;
}
