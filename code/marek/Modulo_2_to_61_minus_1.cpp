// Zwraca (a * b) % (2**p - 1).
// Int - typ, który jest w stanie pomieścić a * b.
template <typename T = unsigned long long, typename Int = __uint128_t>
T Mnoz(T a, T b) {
  constexpr int p = 61;
  constexpr T mod = (T(1) << p) - 1;
  const auto A = (Int) a * b;
  T result = (T) (A & mod) + (T) (A >> p);
  if (mod <= result) {
    result -= mod;
  }
  return result;
}
