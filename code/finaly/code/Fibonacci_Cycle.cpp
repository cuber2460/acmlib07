// Zwraca rozmiar cyklu ciągu Fibonacciego modulo @mod. Znaleziony cykl
// niekoniecznie jest najmniejszym cyklem, wiadomo jednak, że cykl(m) <= 6m.
// Dla m, które nie są postaci 2 * 5^r, zachodzi ograniczenie: cykl(m) <= 4m.
// Jeśli nie zrobi się Nww, to wiadomo, że cykl(m) <= m * 2**(1 + #dz.pierw.m).
int FibonacciCycle(int mod) {
  auto PrimeCycle = [](int p) -> int {
    switch (p % 10) {
      case 2: return 3;   // Tylko 2 spełnia ten case.
      case 5: return 20;  // Tylko 5 spełnia ten case.
      case 1: case 9: return p - 1;
      case 3: case 7: return 2 * (p + 1);
      default: __builtin_unreachable();
    }
  };
  int cycle = 1;
  for (const pair<int, int>& pk : Factor(mod)) {
    const int p = pk.first;
    const int k = pk.second;
    cycle = Nww(cycle, PrimeCycle(p) * Power(p, k - 1));
  }
  return cycle;
}
