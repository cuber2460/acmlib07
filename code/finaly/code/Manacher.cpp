// @s[0..n-1]  - napis długości @n.
// @r[0..2n-2] - tablica promieni palindromów.
// s: a   b   a   a   b   a   a   c   a   a   b   b   b   b   a   a   c   a   c
// r: 0 0 1 0 0 3 0 0 2 0 0 1 0 0 3 0 0 1 0 0 0 1 1 6 1 1 0 0 0 1 0 0 1 0 1 0 0
void Manacher(const char* s, int n, int* r) {
  for (int i = 0, m = 0, k = 0, p = 0; i < 2 * n - 1; m = i++ - 1) {
    while (p < k and i / 2 + r[m] != k)
      r[i++] = min(r[m--], (k + 1 - p++) / 2);
    while (k + 1 < n and p > 0 and s[k + 1] == s[p - 1])
      k++, p--;
    r[i] = (k + 1 - p++) / 2;
  }
}
bool is_pal(int p, int k) { //Przedział [p, k] obustronnie domknięty
	return r[p + k] >= (k - p + 1) / 2;
}
