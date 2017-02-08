template <typename T>
int PrefMatching(const T* s, int n, int j, int k) {
  k = max(k, j);
  int i = k - j;
  while (k < n and s[i] == s[k]) {
    i++;
    k++;
  }
  return k - j;
}

// Liczy funkcję pref.
// s - słowo na pozycjach [0, n-1].
// n - długość słowa.
// p - tablica pref, która zostanie wypełniona na pozycjach [0, n - 1].
template <typename T>
void Pref(const T* s, int n, int* p) {
  int i = 0;
  p[0] = -1;
  for (int j = 1; j < n; j++) {
    const int l = j - i;
    if (l < p[i] and j + p[l] < i + p[i]) {
      p[j] = p[l];
    } else {
      p[j] = PrefMatching(s, n, j, i + p[i]);
      i = j;
    }
  }
  p[0] = n;
}
