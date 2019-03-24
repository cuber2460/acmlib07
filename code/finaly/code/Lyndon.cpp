// 1) Przyjmuje słowo s (wypełnione na pozycjach 0, 1, ..., n-1).
//    Dzieli słowo s na pewną liczbę słów Lyndona p_1, ... p_k tak, że:
//    p_1 >= p_2 >= ... >= p_k (leksykograficznie)
//    Podział jest zapisywany w tablicy b - na i-tej pozycji jest true,
//    jeśli nastąpiło cięcie przed i-tą literką.
// 2) Znajduje minimalne leksykograficznie przesunięcie cykliczne słowa.
// 3) Znajduje minimalny leksykograficznie sufiks słowa.
void lyndon(char * s,   // Słowo zaczynające się na pozycji 0:
                        //   2) s powinno być sklejone: xx.
            int n,      // Długość słowa s (licząc ew. podwojenie).
            int& suf,   // 3) pozycja minimalnego leksykograficznie sufiksu.
            int& cyk,   // 2) pozycja minimalnego leksykograficznie przes. cykl.
            bool* b) {  // Tablica cięcia b.
  for (int i = 0; i < n; i++) b[i] = false;  // wykomentuj, jeśli nie 1)
  int p = 0, k = 0, m = 1;
  while (p < n) {
    if (m == n or s[m] < s[k]) {
      if (p < n / 2) cyk = p;  // wykomentuj, jeśli nie 2)
      while (p <= k) {
        p += m - k;
        if (p < n) {
          suf = p;  // wykomentuj, jeśli nie 3)
          b[p] = true;  // wykomentuj, jeśli nie 1)
        }
      }
      m = (k = p) + 1;
    } else if (s[m++] != s[k++]) k = p;
  }
}
