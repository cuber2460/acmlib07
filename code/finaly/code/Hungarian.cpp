/* HUNGARIAN O(n^3) - Maksymalne najdroższe skojarzenie w pełnym grafie
 dwudzielnym o równolicznych zbiorach wierzchołków. Dostaje macierz z wagami.
 Zwraca wektor 'one', gdzie wierzcholek 'i' jest sparowany z 'one[i]' po prawej.
 Jak chcemy najtańsze, to bierzemy wszystko z minusem. Jak nie ma krawędzi,
 to dajemy -INF przy założeniu: n * |waga| < INF. Indeksujemy od 0. */
#define REP(i, n) for(int i = 0; i < int(n); ++i)
vector<int> hungarian2(const vector<vector<int>> & w) {
	const int n = w.size();
	vector<int> one(n, -1), two(n, -1), L(n), R(n), par(n);
	REP(i, n) L[i] = *max_element(w[i].begin(), w[i].end());
	REP(rep, n) {
		vector<bool> left(n), right(n);
		vector<int> slack(n, INT_MAX), q;
		int x = -1;
		REP(i, n) if(one[i] == -1) q.push_back(i);
		while(x == -1) {
			REP(z, q.size()) {
				int a = q[z];
				left[a] = true;
				REP(b, n) {
					int tmp = L[a] + R[b] - w[a][b];
					if(!right[b] && tmp < slack[b]) {
						par[b] = a;
						slack[b] = tmp;
						if(tmp == 0) {
							right[b] = true;
							if(two[b] != -1) q.push_back(two[b]);
							else { x = b; goto koniec; }
						}
					}
				}
			}
			int val = INT_MAX;
			REP(i, n) if(!right[i]) val = min(val, slack[i]);
			REP(i, n) {
				if(left[i]) L[i] -= val;
				if(right[i]) R[i] += val;
				else if((slack[i] -= val) == 0) {
					right[i] = true;
					if(two[i] != -1) q.push_back(two[i]);
					else x = i;
				}
			}
		}
		koniec:
		while(x != -1) {
			int tmp = one[par[x]];
			one[par[x]] = x;
			two[x] = par[x];
			x = tmp;
		}
	}
	return one;
}
