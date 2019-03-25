void Pref(const char* s, int n, int* p) {
  p[0] = n;
  int i = 1, m = 0;
  while (i < n) {
    while (m + i < n and s[m + i] == s[m]) m++;
    p[i++] = m;
    m = max(m - 1, 0);
    for (int k = 1; p[k] < m; m--) p[i++] = p[k++];
  }
}
