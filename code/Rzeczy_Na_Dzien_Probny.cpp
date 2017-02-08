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
