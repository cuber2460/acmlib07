// Wypełnia tablicę r1 promieniami nieparzystych palindromów. Przykład:
// nap: a b a c a b a
//  r1: 0 1 0 3 0 1 0
void Manacher1(char* nap, int* r1, int n) {
  if (n == 0) return;
  int i = 0, k = 1;
  r1[0] = 0;
  while (i < n) {
    while (k <= r1[i] and i + k + 1 < n and r1[i - k] != r1[i] - k) {
      r1[i + k] = min(r1[i - k], r1[i] - k);
      k++;
    }
    if (i + k >= n) break;
    r1[i + k] = r1[i] - k;
    i += k;
    k = 1;
    while ((i + r1[i] + 1 < n) and (i - r1[i] - 1 >= 0) and
           nap[i + r1[i] + 1] == nap[i - r1[i] - 1]) {
      r1[i]++;
    }
  }
}

// Wypełnia tablicę r2 promieniami parzystych palindromów. Na pozycji i-tej
// znajduje się promień palindromów, które mają środek pomiędzy literkami (i-1)
// oraz i. To na przyklad oznacza, ze zawsze r2[0] = 0 (o ile slowo nie jest
// puste). Przyklad:
// nap: a b b a a b b
//  r2: 0 0 2 0 3 0 1
void Manacher2(char* nap, int* r2, int n) {
  if (n == 0) return;
  int i = 0, k = 1;
  r2[0] = 0;
  while (i < n) {
    while (k < r2[i] and i + k < n and r2[i - k] != r2[i] - k) {
      r2[i + k] = min(r2[i - k], r2[i] - k);
      k++;
    }
    if (i + k >= n) break;
    r2[i + k] = max(0, r2[i] - k);
    i += k;
    k = 1;
    while ((i + r2[i] < n) and (i - r2[i] - 1 >= 0) and
           nap[i + r2[i]] == nap[i - r2[i] - 1]) {
      r2[i]++;
    }
  }
}
