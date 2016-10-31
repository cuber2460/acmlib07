const int inf = 1e9 + 5;
const int nax = 607;

//maksymalnego skojarzenia, indeksowanie od 1, wszystko na intach
//w skojx[] bedzie skojarzenie

namespace Hung{
  int n;
  int m[nax][nax];
  int skojx[nax], skojy[nax];
  int lx[nax], ly[nax];
  bool S[nax], T[nax];
  int q[nax], qbeg, qend;
  int slack[nax], prev[nax];
  
  void init(int size) {
    n = size;
    FOR(i,1,n) FOR(j,1,n) m[i][j] = -inf; 
    FOR(i,1,n) skojx[i] = skojy[i] = 0;
  }
  
  void AddEdge(int a, int b, int c) {
    m[a][b] = max(m[a][b], c);
  }
  
  int hungarian() {
    FOR(i,1,n) {
      ly[i] = 0; lx[i] = -inf;
      FOR(j,1,n) lx[i] = max(lx[i], m[i][j]);
    }
    
    REP(k,n) {
      qbeg = qend = 0;
      FOR(i,1,n) S[i] = T[i] = false;
      FOR(i,1,n) if (!skojx[i]) {
        q[qend++] = i; break;
      }
      
      S[q[0]] = true;
      FOR(i,1,n) slack[i] = lx[q[0]] + ly[i] - m[q[0]][i], prev[i] = q[0];
      
      int y = 0;
      while (true) {
        while (qbeg < qend) {
          int x = q[qbeg++]; S[x] = true;
          FOR(i,1,n) if (!T[i]) {
            if (slack[i] > lx[x] + ly[i] - m[x][i]) {
              slack[i] = lx[x] + ly[i] - m[x][i];
              prev[i] = x;
            }
            if (lx[x] + ly[i] == m[x][i]) {
              T[i] = true;
              if (skojy[i])
                q[qend++] = skojy[i];
              else {
                y = i; goto koniec;
              }
            }
          }
        }
        
        int minslack = inf;
        FOR(i,1,n) if (!T[i]) minslack = min(minslack, slack[i]);
        FOR(i,1,n) {
          if (S[i]) lx[i] -= minslack;
          if (T[i]) 
            ly[i] += minslack;
          else
            slack[i] -= minslack;
        }
        FOR(i,1,n) if (!T[i] && !slack[i]) {
          T[i] = true;
          if (skojy[i]) {
            q[qend++] = skojy[i];
          }
          else {
            y = i; goto koniec;
          }
        }
      }
    
    koniec:
      while (y) { 
        skojy[y] = prev[y]; 
        int pom = skojx[prev[y]];
        skojx[prev[y]] = y;
        y = pom; 
      }
    }
    
    int res = 0;
    FOR(i,1,n) res += m[i][skojx[i]];
    return res;  
  }
}
