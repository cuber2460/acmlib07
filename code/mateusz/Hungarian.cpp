/* HUNGARIAN O(n^3).                                                          */
/* Maksymalne najdroższe skojarzenie w pełnym grafie dwudzielnym o            */
/* równolicznych zbiorach wierzchołków.  Argumenty to macierz z wagami        */
/* i liczba wierzchołków.  Zwracany wynik to wektor, który mówi co jest       */
/* z czym skojarzone.                                                         */
/* Jak chcemy najtańsze to bierzemy wszystko z minusem, jak gdzieś nie ma     */
/* krawędzi to dajemy -INF, przy zalozeniu, ze: n * |waga| < INF.             */

// TODO(Mateusz): Opisać dokładnie wejście i wyjśce: Rozmiary tablic, indeksy
// od których się rzeczy zaczynają.
// TODO(Mateusz): Zrobić template po typie wag oraz po znaku (oznaczającym,
// czy chce się maksimum czy minimum).
vector<int> hungarian(const vector<vector<int>>& w, int n) {
  // TODO(Mateusz): Nie rób takich tablic na stosie!  Potem się okaże, że na
  // jakimś konkursie jest limit na stos i to się wywali.
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
                // TODO(Mateusz): Ten kod jest zbyt skomplikowany,
                // żeby robić goto!
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
