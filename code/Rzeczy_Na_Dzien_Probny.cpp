// 1. Sprawdzić int128.
// Oczekiwany output: 61231558446921906466935685523974676212.
int Main() {
  __int128_t x = (1llu << 62);
  x *= x;
  while (x) {
    printf("%d", (int) (x % 10));
    x /= 10;
  }
  printf("\n");
  return 0;
}

// 2. Czy rand() działa tak samo na sprawdzaczce.
int Main() {
  srand(0x12345);
  const int a = rand();
  printf("a = %d\n", a);
  printf("RAND_MAX w zbiorze: = [0, %d]\n", (int) RAND_MAX);
  assert(a == 1206605802 /* Tu wkleić wartość po uruchomieniu u siebie. */);
  return 0;
}

// 3. Sprawdzanie czasu.
int Main() {
  while (clock() <= 0.690 * CLOCKS_PER_SEC);
  // Sprawdzić czy uruchomienie zajęło dokładnie 690ms.  Jeśli nie da się
  // sprawdzić dokładnego czasu wykonania, to można zbinsearchować time limit.
  return 0;
}

// 4. Sprawdzić czy time() zwraca różne wartości pomiędzy uruchomieniami.
int Main() {
  switch (time(NULL) % 3) {
    case 0:
      while (true);  // Daj TLE.
    case 1:
      assert(false);  // Daj RE.
    case 2:
      printf("Daj WA.\n");
  }
  return 0;
}

// 5. Przetestować czy kompresja działa.
// (...) <- Przepisać kompresję.
int Main() {
  // Ustawić false do wygenerowania kodu, a true do zweryfikowania kompresji.
  bool czy_uruchomienie = false;
  Compress compress;
  for (int i = 1; i <= 100; i++) {
    const uint64_t result = compress.Data(62, i * i);
    if (czy_uruchomienie) {
      assert(result == i * i);
    }
  }
}

// 6. Sprawdzić float128.
// Oczekiwany output: 47.610000000.
int Main() {
  __float128 x = 6.9;
  printf("%.9lf\n", (double)(x*x));
  return 0;
}

// 7. Sprawdzić ordered_seta.
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;

template <typename T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

ordered_set<int> s;

int Main() {
  s.insert(1);
  s.insert(2);
  cout << s.order_of_key(1) << endl;    // Out: 0.
  cout << *s.find_by_order(1) << endl;  // Out: 2.
}
