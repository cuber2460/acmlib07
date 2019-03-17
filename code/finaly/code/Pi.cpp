struct Primes {
	vector <ll> w, dp;
	int gdz(ll v) {
		if (v<=w.back()/v)
			return v-1;
		return w.size()-w.back()/v;
	}
	ll pi(ll n) {
		for (ll i=1; i*i<=n; i++) {
			w.push_back(i);
			if ((n/i)!=i)
				w.push_back(n/i);
		}
		sort(w.begin(), w.end());
		for (ll i : w)
			dp.push_back(i-1);
		for (ll i=1; (i+1)*(i+1)<=n; i++) {
			if (dp[i]==dp[i-1]) continue;
			for (int j=(int)w.size()-1; w[j]>=(i+1)*(i+1); j--)
				dp[j]-=dp[gdz(w[j]/(i+1))]-dp[i-1];
		}
		return dp.back();
	}
	ll ask(ll v) { return dp[gdz(v)]; }//v==n/u for some u
};
