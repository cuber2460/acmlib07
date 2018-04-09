  friend pair<Big, Big> divmod(const Big& a1, const Big& b1) {
    D norm = base / (b1.w.back() + 1);
    Big a = a1.abs() * norm;
    Big b = b1.abs() * norm;
    Big q, r;
    q.w.resize(a.size());
    for (int i = a.size() - 1; i >= 0; i--) {
      r *= base;
      r += a[i];
      int s1 = b.size() < r.size() ? r[b.size()] : 0;
      int s2 = b.size() - 1 < r.size() ? r[b.size() - 1] : 0;
      D d = ((D2) s1 * base + s2) / b.w.back();
      r -= b * d;
      while (r < 0)
        r += b, --d;
      q[i] = d;
    }
    q.sign = a1.sign * b1.sign;
    r.sign = a1.sign;
    q.trim();
    r.trim();
    return make_pair(q, r /= norm);
  }
  friend Big sqrt(const Big& a1) {
    Big a = a1;
    while (a.w.empty() || a.size() % 2 == 1)
      a.push_back(0);
    int n = a.size();
    D firstDigit = my_sqrt((long long) a[n - 1] * base + a[n - 2]);
    D norm = base / (firstDigit + 1);
    a *= norm;
    a *= norm;
    while (a.w.empty() || a.size() % 2 == 1)
      a.push_back(0);
    Big r = (D2) a[n - 1] * base + a[n - 2];
    firstDigit = sqrt((double) a[n - 1] * base + a[n - 2]);
    D q = firstDigit;
    Big res;
    for (int j = n / 2 - 1; j >= 0; j--) {
      for (; ; --q) {
        #warning "res * 2 * base -> res * (2 * base)"
        Big r1 = (r - (res * 2 * base + q) * q) * base * base +
            (j > 0 ? (D2) a[2 * j - 1] * base + a[2 * j - 2] : 0);
        if (r1 >= 0) {
          r = r1;
          break;
        }
      }
      res *= base;
      res += q;
      if (j > 0) {
        int d1 = res.size() + 2 < r.size() ? r[res.size() + 2] : 0;
        int d2 = res.size() + 1 < r.size() ? r[res.size() + 1] : 0;
        int d3 = res.size() < r.size() ? r[res.size()] : 0;
        q = ((D2) d1 * base * base + (D2) d2 * base + d3) / (firstDigit * 2);
      }
    }
    return res.trim() /= norm;
  }
