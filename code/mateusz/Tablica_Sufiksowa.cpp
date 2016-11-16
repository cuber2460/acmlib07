/* TABLICA SUFIKSOWA O(n*log(n)) i O(n*log^2(n)).                             */
/* Sortuje leksykograficznie wszystke sufiksy słowa.                          */
/* Dodatkowo liczy pomocnicze tablice rank i lcp.                             */
/* Wszystko indeksujemy od 1, ale podajemy wskaźnik na 0                      */

struct suffix_array {
  vector<pair<pair<int,int>, int>> wek;  // do log^2
  vector<vector<int>> radix1;  // do log
  vector<vector<int>> radix2;  // do log
  vector<int> dru;  // do log
  vector<pair<int,int>> ska;  // do log
  
  //n to limit na długość słowa
  suffix_array(int n) {
    wek.resize(n + 1, make_pair(make_pair(-1, -1), -1));
    radix1.resize(n + 1);
    radix2.resize(n + 1);
    dru.resize(n + 1, -1);
    ska.resize(n + 1, make_pair(-1, -1));
  }

  void sa_log_2(char *tab, int n, int *sa, int *ran, int *lcp) {
    int l = 0;
    for (int i = 1; i <= n; i++)
      ran[i] = tab[i];
    for (int h = 1; h <= n; h *= 2) {
      for (int i = 1; i <= n; i++)
        wek[i].first.first = ran[i];
      for (int i = 1; i + h <= n; i++)
        wek[i].first.second = ran[i + h];
      for (int i = n - h + 1; i <= n; i++)
        wek[i].first.second = 0;
      for (int i = 1; i <= n; i++)
        wek[i].second = i;
      sort(wek.begin() + 1, wek.begin() + 1 + n);
      l = 0;
      for (int i = 1; i <= n; i++) {
        if (wek[i].first != wek[i - 1].first)
          l++;
        ran[wek[i].second] = l;
      }
    }
    for (int i = 1; i <= n; i++)
      sa[ran[i]] = i;
    l = 0;
    for (int i = 1; i <= n; i++) {
      l = max(0, l - 1);
      if (ran[i] == n) {
        lcp[n] = 0;
        continue;
      }
      while (tab[i + l] == tab[sa[ran[i] + 1] + l])
        l++;
      lcp[ran[i]] = l;
    }
  }

  void sa_log(char *tab, int n, int *sa, int *ran, int *lcp) {
    int l = 0;
    for (int i = 1; i <= n; i++)
      ska[i] = make_pair(tab[i], i);
    sort(ska.begin() + 1, ska.begin() + 1 + n);
    ska[0] = ska[1];
    l = 1;
    for (int i = 1; i <= n; i++) {
      if (ska[i].first != ska[i - 1].first)
        l++;
      ran[ska[i].second] = l;
    }
    for (int h = 1; h <= n; h *= 2) {
      for (int i = 0; i <= n; i++) {
        dru[i] = 0;
        radix1[i].clear();
        radix2[i].clear();
      }
      for (int i = 1; i + h <= n; i++)
        dru[i] = ran[i + h];
      for (int i = 1; i <= n; i++)
        radix1[dru[i]].push_back(i);
      for (int i = 0; i <= n; i++)
        for (int j = 0; j < (int) radix1[i].size(); j++)
          radix2[ran[radix1[i][j]]].push_back(radix1[i][j]);
      l = 0;
      for (int i = 0; i <= n; i++) {
        for (int j = 0; j < (int) radix2[i].size(); j++) {
          if (!j || dru[radix2[i][j]] != dru[radix2[i][j - 1]])
            l++;
          ran[radix2[i][j]] = l;
        }
      }
    }
    for (int i = 1; i <= n; i++)
      sa[ran[i]] = i;
    l = 0;
    for (int i = 1; i <= n; i++) {
      l = max(0, l - 1);
      if (ran[i] == n) {
        lcp[n] = 0;
        continue;
      }
      while (tab[i + l] == tab[sa[ran[i] + 1] + l])
        l++;
      lcp[ran[i]] = l;
    }
  }
};
