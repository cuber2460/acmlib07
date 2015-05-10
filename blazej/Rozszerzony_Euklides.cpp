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
