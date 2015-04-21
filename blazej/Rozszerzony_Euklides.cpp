//sprawdzone recznie wiec swag
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

void eukl(int &x, int &y, int a, int b) {
	if (!a) {
		x = 0;
		y = 1;
		return;
	}
	
	eukl(y,x,b%a,a);
	x -= y*(b/a);
}

int main(int argc, char * argv[]) {
	debug = argc > 1;
	int a,b;
	scanf("%d%d",&a,&b);
	int x,y;
	eukl(x,y,a,b);
	printf("%d %d nwd to %d\n",x,y,x*a + y*b); 
	return 0;
}
