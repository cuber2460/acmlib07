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
const int nax = 1e6 + 5;

inline int fastMax(int x, int y) { return (((y-x)>>(32-1))&(x^y))^y; }
inline int fastMin(int x, int y) { return (((y-x)>>(32-1))&(x^y))^x; }
inline int fastAbs(int n) { return (n ^ (n >> (32-1))) - (n >> (32-1)); }

int n;
int t[1000007];


/*Kod1, n = 100000, losowe dane:
	scanf("%d",&n);
	REP(i,n) scanf("%d",&t[i]);
	
	ll suma = 0;
	REP(i,n) FOR(j,i+1,n-1)
		suma += min(t[i],t[j]);
	printf("%lld\n",suma);
	return 0;

min, min: 5.128s
fastMin, fastMin: 6.258s
*/

/*Kod2, n = 100000, losowe dane:
	scanf("%d",&n);
	REP(i,n) scanf("%d",&t[i]);
	
	ll suma = 0;
	int x = 0;
	REP(i,n) FOR(j,i+1,n-1) if (min(t[i], t[j]) > x) {
		suma += min(t[i],t[j]);
		x = t[i] - t[j];
	}
	printf("%lld\n",suma);
	return 0;

min, min: 16.557s
min, fastMin: 5.101s
fastMin, fastMin: 5.555s
*/

/*Kod3, n = 100000, losowe dane:
	scanf("%d",&n);
	REP(i,n) scanf("%d",&t[i]);
	
	ll suma = 0;
	int x = 0;
	REP(i,n) FOR(j,i+1,n-1) if (min(t[i], t[j]) > x) {
		suma += min(t[i],t[j] - x);
		x = t[i] - t[j];
	}
	printf("%lld\n",suma);
	return 0;

min, min: 5.222s
min fastMin: 5.051s
fastMin, fastMin: 5.549s
*/

//Jak zamienic min -> max i fastMin -> fastMax wyniki sa analogiczne

/*Kod4, n = 100000, losowe dane: 
	scanf("%d",&n);
	REP(i,n) scanf("%d",&t[i]);
	
	ll suma = 0;
	int x = 0;
	REP(i,n) FOR(j,i+1,n-1) if (abs(t[i] - t[j]) > abs(t[j] - x)) {
		suma += abs(t[i] - x);
		x = t[i] - t[j];
	}
	printf("%lld\n",suma);

abs, abs, abs: 14.527s
fastAbs, fastAbs, fastAbs: 16.609s
*/

//dla fastAbs nie znalazlem kodu kiedy bedzie szybsze
//wniosek ogolnie zwykle funkcje lepsze ale jak jest grubo to mozna sprobowac

int main(int argc, char * argv[]) {
	debug = argc > 1;
	return 0;
}
