#pragma GCC optimize("Ofast")

using v4si = unsigned __attribute__ ((vector_size (16)));

unsigned Sumuj(unsigned* a_, int len) {
  v4si sum = {0, 0, 0, 0};
  while (len % 4) {
    sum[0] += a_[0];
    len--;
    a_++;
  }
  const v4si* a = (const v4si*) a_;
  len /= 4;
  for (int i = 0; i < len; i++) {
    sum += a[i];
  }
  return sum[0] + sum[1] + sum[2] + sum[3];
}
