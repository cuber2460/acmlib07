int phi[nax]; // 0.4s for nax = 1e7+5
for(int i = 0; i < nax; ++i) phi[i] = i;
for(int i = 2; i < nax; ++i) if(phi[i] == i)
	for(int j = i; j < nax; j += i)
		phi[j] = phi[j] / i * (i - 1);
// phi = {0 1 1 2 2 4 2 6 4 6 4 10 ...}
