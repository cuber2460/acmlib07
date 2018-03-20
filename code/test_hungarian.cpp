/* HUNGARIAN O(n^3).                                                          */
/* Maksymalne najdroższe skojarzenie w pełnym grafie dwudzielnym o            */
/* równolicznych zbiorach wierzchołków.  Argumenty to macierz z wagami        */
/* i liczba wierzchołków.  Zwracany wynik to wektor, który mówi co jest       */
/* z czym skojarzone.                                                         */
/* Jak chcemy najtańsze to bierzemy wszystko z minusem, jak gdzieś nie ma     */
/* krawędzi to dajemy -INF, przy zalozeniu, ze: n * |waga| < INF.             */
/* Wszystko w wektorze indeksujemy od 0.                                      */

#include <bits/stdc++.h>
using namespace std;

vector<int> hungarian(const vector<vector<int>>& w, int n) {
  int lx[n], ly[n], skojx[n], skojy[n];
  int markx[n], marky[n], slack[n], par[n], q[n];
  for (int i = 0; i < n; i++) {
    skojx[i] = skojy[i] = -1;
    ly[i] = 0;
    lx[i] = *max_element(w[i].begin(), w[i].end());
  }
  for (int k = 0; k < n; k++) {
    int v = -1, qb = 0, qe = 0;
    for (int i = 0; i < n; i++) {
      marky[i] = markx[i] = 0;
      slack[i] = -1;
      if (skojx[i] == -1)
        q[qe++] = i;
    }
    while (v == -1) {
      while (qb < qe) {
        int i = q[qb++];
        markx[i] = 1;
        for (int j = 0; j < n; j++) {
          if (!marky[j] && (slack[j] == -1 ||
              slack[j] > lx[i] + ly[j] - w[i][j])) {
            if ((slack[j] = lx[par[j] = i] + ly[j] - w[i][j]) == 0) {
              marky[j] = 1;
              if (skojy[j] != -1) {
                q[qe++] = skojy[j];
              } else {
                v=j;
                goto koniec;
              }
            }
          }
        }
      }
      int x = -1;
      for (int i = 0; i < n; i++)
        if (!marky[i] && (x == -1 || slack[i] < x))
          x = slack[i];
      for (int i = 0; i < n; i++) {
        if (markx[i]) lx[i] -= x;
        if (marky[i]) ly[i] += x;
        else if ((slack[i] -= x) == 0) {
          marky[i] = 1;
          if (skojy[i] != -1) q[qe++] = skojy[i];
          else v = i;
        }
      }
    }
    koniec:
    while (v != -1) {
      int y = skojx[par[v]];
      skojx[par[v]] = v;
      skojy[v] = par[v];
      v = y;
    }
  }
  return vector<int>(skojx, skojx + n);
}

const int INF = INT_MAX;
#define REP(i, n) for(int i = 0; i < int(n); ++i)
vector<int> hungarian2(const vector<vector<int>> & w) {
	const int n = w.size();
	vector<int> m1(n, -1), m2(n, -1), L(n), R(n), par(n);
	REP(i, n) L[i] = *max_element(w[i].begin(), w[i].end());
	REP(k, n) {
		vector<bool> left(n), right(n);
		vector<int> slack(n, INF), q;
		int a = -1;
		REP(i, n) if(m1[i] == -1) q.push_back(i);
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
							if(m2[j] != -1) q.push_back(m2[j]);
							else { a = j; goto koniec; }
						}
					}
				}
			}
			int x = INF;
			REP(i, n) if(!right[i]) x = min(x, slack[i]);
			assert(x != INF);
			REP(i, n) {
				if(left[i]) L[i] -= x;
				if(right[i]) R[i] += x;
				else if((slack[i] -= x) == 0) {
					right[i] = true;
					if(m2[i] != -1) q.push_back(m2[i]);
					else a = i;
				}
			}
		}
		koniec:
		while(a != -1) {
			int tmp = m1[par[a]];
			m1[par[a]] = a;
			m2[a] = par[a];
			a = tmp;
		}
	}
	return m1;
}

int main() {
	//~ auto xx = hungarian({{-5,-16},{-10,-20}}, 2);
	//~ for(int a : xx) printf("%d ", a); puts(""); return 0;
	for(int rep = 0; rep < 100000; ++rep) {
		if(rep % 50 == 0) printf("%d\n", rep);
		int n = 1 + rand() % 20;
		vector<vector<int>> grid(n, vector<int>(n));
		for(int i = 0; i < n; ++i)
			for(int j = 0; j < n; ++j)
				grid[i][j] = -10 + rand() % 20;
		assert(hungarian(grid, n) == hungarian2(grid));
	}
	puts("OK");
}
