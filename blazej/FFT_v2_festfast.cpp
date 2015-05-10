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

/* NTT
const int mod = 7340033;
const int czapa = 4096;
const int generator = 3;

int n,k,q;
ll dp[31][nax], pom[czapa], odwr[czapa], w[czapa];

ll pot(ll a, ll b) {
	ll res = 1;
	while (b) {
		if (b&1)
			res = (res*a)%mod;
		a = (a*a)%mod;
		b /= 2;
	}
	return res;
}

void fft(ll *a, ll *p, int n) {
	for (int i=1, j=0; i<n; i++) {
		int bit = n >> 1;
		for (; j>=bit; bit>>=1)
			j -= bit;
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}
	
	for (int len=2; len<=n; len<<=1) {
		for (int i=0; i<n; i+=len) {
			for (int j=0; j<len/2; ++j) {
				ll u = a[i+j], v = (a[i+j+len/2]*p[j*(n/len)])%mod;
				a[i+j] = u + v;
				a[i+j+len/2] = u - v;
			}
		}
	}
}


void init() {
	
	ll pierw = pot(generator, (mod-1)/czapa);
	w[0] = 1;
	FOR(i,1,czapa-1)
		w[i] = (w[i-1]*pierw)%mod;
	
	REP(i,czapa)
		odwr[i] = pot(w[i], mod-2);
	
	ll pod = pot(czapa, mod-2);
	
	dp[0][0] = 1;
	FOR(i,1,30) {
		REP(j,czapa) pom[j] = 0;
		
		FOR(j,0,1000)
			pom[j] = dp[i-1][j];
		
		fft(pom, w, czapa);
		
		REP(j,czapa) {
			pom[j] *= pom[j];
			pom[j] %= mod;
			pom[j] *= pom[j];
			pom[j] %= mod;
		}	
		
		fft(pom, odwr, czapa);
		
		FOR(j,1,1000)
			dp[i][j] = ((pom[j-1]*pod)%mod+mod)%mod;
		dp[i][0] = 1;
	}
}
*/

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
