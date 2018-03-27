// Buduje tablicę sufiksową w czasie O(n + alpha).
// s[0..n-1],   s[i] in [0..alpha-1],    sa[0..n-1],   lcp[0..n-2].
void Sufar(const int* s, int n, int alpha, int* sa, int* lcp = nullptr) {
  if (n > 0) sa[0] = 0;
  if (n <= 1) return;
  vector<int> roz(alpha + 1), wsk(alpha), typ(n + 1), ids(n, -1), news, pos;
  auto star = [&](int i) -> bool { return typ[i] == 3; };
  auto Indukuj = [&]() -> void {
    copy(roz.begin(), roz.end() - 1, wsk.begin());
    sa[wsk[s[n - 1]]++] = n - 1;
    for (int i = 0; i < n; i++)
      if (sa[i] > 0 and !typ[sa[i] - 1])
        sa[wsk[s[sa[i] - 1]]++] = sa[i] - 1;
    copy(roz.begin() + 1, roz.end(), wsk.begin());
    for (int i = n - 1; i >= 0; i--)
      if (sa[i] > 0 and typ[sa[i] - 1])
        sa[--wsk[s[sa[i] - 1]]] = sa[i] - 1;
  };
  typ[n] = 3;
  for (int i = n - 1; i >= 0; i--) {
    sa[i] = -1;
    roz[s[i] + 1]++;
    if (i != n - 1 and s[i] < s[i + 1] + !!typ[i + 1]) {
      typ[i] = 1;
    } else if (typ[i + 1]) {
      typ[i + 1] = 3;
    }
  }
  partial_sum(roz.begin(), roz.end(), roz.begin());
  copy(roz.begin() + 1, roz.end(), wsk.begin());
  for (int i = 0; i < n; i++) if (star(i)) sa[--wsk[s[i]]] = i;
  Indukuj();
  int nast_id = 0, b = -1;
  for (int i = 0; i < n; i++) {
    int a = sa[i];
    if (!star(a)) continue;
    if (b >= 0) while (a == sa[i] or !star(a) or !star(b)) {
      if (star(a) != star(b) or s[a++] != s[b++]) {
        nast_id++;
        break;
      }
    }
    ids[b = sa[i]] = nast_id;
  }
  for (int i = 0; i < n; i++) {
    if (ids[i] == -1) continue;
    news.push_back(ids[i]);
    pos.push_back(i);
  }
  vector<int> new_sa(news.size());
  Sufar(news.data(), (int) news.size(), nast_id + 1, new_sa.data());
  fill(sa, sa + n, -1);
  copy(roz.begin() + 1, roz.end(), wsk.begin());
  reverse(new_sa.begin(), new_sa.end());
  for (int j : new_sa) sa[--wsk[s[pos[j]]]] = pos[j];
  Indukuj();
  if (lcp) {
    for (int i = 0; i < n; i++) ids[sa[i]] = i;
    for (int i = 0, k = 0; i < n; i++, k = max(0, k - 1)) {
      if (ids[i] == n - 1) { k = 0; continue; }
      const int j = sa[ids[i] + 1];
      while (i + k < n and j + k < n and s[i + k] == s[j + k]) k++;
      lcp[ids[i]] = k;
    }
  }
}
