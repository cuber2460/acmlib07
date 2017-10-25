/* Liczy liczbę liczb pierwszych mniejszych od n w około O(n^(2/3)).          */
/* Można modyfikować by policzyć np. sumę liczb pierwszych.                   */

struct Primes
{
	vector <long long> pie, w;
	vector <int> naj;
	vector < vector <long long> > dp;
	unordered_map <long long,int> mapa;
	long long liczdp(int n, int k)
	{
		if (!k)
			return w[n];
		if (!n)
			return 1;
		if (k>naj[n])
			return liczdp(n, naj[n])+naj[n]-k;
		if ((int)dp[n].size()>k)
			return dp[n][k];
		long long ret=liczdp(n, k-1)-liczdp(mapa[w[n]/pie[k-1]], k-1);
		dp[n].push_back(ret);
		return ret;
	}
	long long pi(long long n)
	{
		pie.clear();
		w.clear();
		dp.clear();
		naj.clear();
		mapa.clear();
		int d=1;
		while((long long)d*d<n)
			d++;
		int ok=0;
		while(!ok)
		{
			d++;
			ok=1;
			for (int i=2; i*i<=d; i++)
				if (!(d%i))
					ok=0;
		}
		int sito[d+3];
		memset(sito, 0, sizeof(sito));
		for (int i=2; i<=d; i++)
		{
			if (sito[i])
				continue;
			pie.push_back(i);
			for (int j=i; j<=d; j+=i)
				sito[j]=1;
		}
		for (long long i=1; i<=(n/i); i++)
		{
			w.push_back(i);
			if ((n/i)!=i)
				w.push_back(n/i);
		}
		sort(w.begin(), w.end());
		naj.resize(w.size(), 0);
		for (int i=0; i<naj.size(); i++)
		{
			if (i)
				naj[i]=naj[i-1];
			while(pie[naj[i]]*pie[naj[i]]<=w[i])
				naj[i]++;
		}
		dp.resize(w.size(), vector<long long>{0});
		for (int i=0; i<w.size(); i++)
			mapa[w[i]]=i;
		return liczdp((int)w.size()-1, (int)pie.size()-1)+pie.size()-2;
	}
};
