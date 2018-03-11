constexpr int n = (1 << 20);
// Xor(a, b, c):
//   Trans(a) XOR; Trans(b) XOR; Trans(c) XOR
//   d[i] = a[i] * b[i] * c[i]
//   Trans(d) XOR ODW
//   return d
void Trans(ll a[n]) {
  for (int len = 1; len < n; len *= 2) {
    for (int i = 0; i < n; i += 2 * len) {
      for (int j = 0; j < len; j++) {
        const ll u = a[i + j];
        const ll v = a[i + j + len];
        a[i + j      ] = DodajLiczby(u, v);              // XOR
        a[i + j + len] = OdejmijLiczby(u, v);            // XOR
        a[i + j      ] = Podziel2(DodajLiczby(u, v));    // XOR ODW
        a[i + j + len] = Podziel2(OdejmijLiczby(u, v));  // XOR ODW
        a[i + j      ] = v;                              // AND
        a[i + j + len] = DodajLiczby(u, v);              // AND
        a[i + j      ] = OdejmijLiczby(v, u);            // AND ODW
        a[i + j + len] = u;                              // AND ODW
        a[i + j      ] = DodajLiczby(u, v);              // OR
        a[i + j + len] = u;                              // OR
        a[i + j      ] = v;                              // OR ODW
        a[i + j + len] = OdejmijLiczby(u, v);            // OR ODW
      }
    }
  }
}
