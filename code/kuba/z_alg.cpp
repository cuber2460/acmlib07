#include <bits/stdc++.h>
using namespace std;

void z_alg(int n, char *s, int *pref) {
	pref[1] = 0; int t = 0, k = 0;
	for (int i = 2; i <= n; ++i) {
		while (s[i + t] == s[1 + t]) t++;
		pref[i] = t;
		for (k = 1; t > 0 && pref[1 + k] != --t; ++k, ++i) {
			pref[i + 1] = min(pref[1 + k], t);
		}
	}
}


