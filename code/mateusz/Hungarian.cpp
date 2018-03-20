/* HUNGARIAN O(n^3).                                                          */
/* Maksymalne najdroższe skojarzenie w pełnym grafie dwudzielnym o            */
/* równolicznych zbiorach wierzchołków.  Argumenty to macierz z wagami        */
/* i liczba wierzchołków.  Zwracany wynik to wektor, który mówi co jest       */
/* z czym skojarzone.                                                         */
/* Jak chcemy najtańsze to bierzemy wszystko z minusem, jak gdzieś nie ma     */
/* krawędzi to dajemy -INF, przy zalozeniu, ze: n * |waga| < INF.             */
/* Wszystko w wektorze indeksujemy od 0.                                      */

const int INF = INT_MAX;
#define REP(i, n) for(int i = 0; i < int(n); ++i)
vector<int> hungarian(const vector<vector<int>> & w) {
	const int n = w.size();
	vector<int> one(n, -1), two(n, -1), L(n), R(n), par(n);
	REP(i, n) L[i] = *max_element(w[i].begin(), w[i].end());
	REP(k, n) {
		vector<bool> left(n), right(n);
		vector<int> slack(n, INF), q;
		int a = -1;
		REP(i, n) if(one[i] == -1) q.push_back(i);
		while(a == -1) {
			REP(z, q.size()) {
				int i = q[z];
				left[i] = true;
				REP(j, n) {
					int tmp = L[i] + R[j] - w[i][j];
					if(!right[j] && tmp < slack[j]) {
						par[j] = i;
						slack[j] = tmp;
						if(tmp == 0) {
							right[j] = true;
							if(two[j] != -1) q.push_back(two[j]);
							else { a = j; goto koniec; }
						}
					}
				}
			}
			int x = INF;
			REP(i, n) if(!right[i]) x = min(x, slack[i]);
			REP(i, n) {
				if(left[i]) L[i] -= x;
				if(right[i]) R[i] += x;
				else if((slack[i] -= x) == 0) {
					right[i] = true;
					if(two[i] != -1) q.push_back(two[i]);
					else a = i;
				}
			}
		}
		koniec:
		while(a != -1) {
			int tmp = one[par[a]];
			one[par[a]] = a;
			two[a] = par[a];
			a = tmp;
		}
	}
	return one;
}
