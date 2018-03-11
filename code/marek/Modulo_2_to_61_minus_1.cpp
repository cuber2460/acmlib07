// Zwraca (a * b) % (2**p - 1).
ull Mnoz(ull a, ull b) {
  constexpr int p = 61;
  constexpr ull mod = (1llu << p) - 1;
  const auto A = (__uint128_t) a * b;
  ull result = (ull) (A & mod) + (ull) (A >> p);
  if (mod <= result) {
    result -= mod;
  }
  return result;
}
