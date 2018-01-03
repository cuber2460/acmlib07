struct Compress {
  static unsigned char data[];  // Wersja bez UTF-8 używa 8/8 bitów.
  int pos = -1, left = 0;       // Wersja z UTF-8 używa 16/24 = 2/3 bitów.

  uint64_t Data(int bits, uint64_t x = 0) {
    assert(1 <= bits and bits <= 64);
    for (int i = 0; i < bits; i++) {
      if (left-- == 0) {
        pos++;
        left = 7;                             // Jeśli nie UTF-8.
        left = 5 - 2 * !(pos % 3);            // Jeśli UTF-8.
        data[pos] |= 368 - 48 * left;         // Jeśli UTF-8.
      }
      if (((x >> i) ^ (data[pos] >> left)) & 1) {
        data[pos] ^= 1 << left;
        x ^= 1llu << i;
      }
    }
    return x;
  }

  void Print() {
    constexpr char code[] = "KaMylMaDoWnA";  // Length must be <= 16.
    std::ofstream out("compress.cpp");
    out << "unsigned char Compress::data[] = R\"" << code << "(";
    while (left or (pos + 1) % 3) Data(1);  // Jeśli UTF-8.
    out.write(reinterpret_cast<const char*>(data), pos + 1);
    out << ")" << code << "\";\n";
  }
};

unsigned char Compress::data[2 * 128 * 1024 /* More than will be written. */];
