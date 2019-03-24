ll dziel(ll a, ll b) {...}
vll inv(vll p, int n) {
	assert(p[0]);
	ll odw=dziel(1, p[0]);
	for (ll &i : p) i=(i*odw)%mod;
	vll q{1};
	for (int i=1; i<n; i<<=1) {
		vll r=multiply(vll(p.begin(), p.begin()+min(2*i, (int)p.size())), q, true);
		r.resize(2*i);
		r.erase(r.begin(), r.begin()+i);
		for (ll &j : r) j=(mod-j)%mod;
		r=multiply(r, q, true);
		q.resize(2*i);
		for (int j=0; j<min(i, (int)r.size()); j++) q[i+j]=r[j];
	}
	q.resize(n);
	while(!q.empty() && !q.back()) q.pop_back();
	for (ll &i : p) i=(i*odw)%mod;
	return q;
}
vll div(vll a, vll b) {
	int s=(int)a.size()-(int)b.size()+1;
	if (s<=0) return {};
	reverse(a.begin(), a.end()); reverse(b.begin(), b.end());
	b=inv(b, s);
	a=multiply(a, b, true);
	a.resize(s);
	reverse(a.begin(), a.end());
	return a;
}
vll rem(vll a, vll b) {
	vll d=div(a, b);
	d=multiply(d, b, true);
	for (int i=0; i<(int)d.size(); i++) a[i]=(a[i]-d[i]+mod)%mod;
	while(!a.empty() && !a.back()) a.pop_back();
	return a;
}
void precalc(int v, vector <vll> &help, vll query) {
	if ((int)help.size()<=v) help.resize(v+1);
	int s=query.size();
	if (s==1) {
		help[v]={(mod-query[0])%mod, 1};
		return;
	}
	vll a, b;
	for (int i=0; i<s/2; i++) a.push_back(query[i]);
	for (int i=s/2; i<s; i++) b.push_back(query[i]);
	precalc(v*2, help, a); precalc(v*2+1, help, b);
	help[v]=multiply(help[v*2], help[v*2+1], true);
}
void calc(int v, vll &res, vector <vll> &help, vll wek) {
	wek=rem(wek, help[v]);
	if ((int)help[v].size()==2) {
		res.push_back(wek.empty() ? 0 : wek[0]);
		return;
	}
	calc(v*2, res, help, wek);
	calc(v*2+1, res, help, wek);
}
vll multi_eva(vll wek, vll query) {
	vector <vll> help;
	precalc(1, help, query);
	vll res;
	calc(1, res, help, wek);
	return res;
}
