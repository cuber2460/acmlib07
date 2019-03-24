ll trzesienie(ll a, ll b, ll c) {
	if (c<0) return 0;
	if (a>b) swap(a, b);
	ll p=c/b;
	ll k=b/a;
	ll d=(c-p*b)/a;
	return trzesienie(b-k*a, a, c-a*(k*p+d+1))+(p+1)*(d+1)+k*p*(p+1)/2;
}
