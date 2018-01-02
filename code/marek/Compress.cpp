struct Compress {
  static unsigned char data[];
  int pos = -1, left = 0;

  uint64_t Data(int bits, uint64_t x = 0) {
    assert(1 <= bits and bits <= 64);
    for (int i = 0; i < bits; i++) {
      if (left-- == 0) {
        pos++;
        left = 7;                             // Jeśli nie UTF-8.
        left = 5 * !!(pos % 6);               // Jeśli UTF-8.
        data[pos] |= 252 - 124 * (left / 5);  // Jeśli UTF-8.
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
    while (left or (pos + 1) % 6) Data(1);  // Jeśli UTF-8.
    out.write(reinterpret_cast<const char*>(data), pos + 1);
    out << ")" << code << "\";\n";
  }
};

unsigned char Compress::data[2 * 128 * 1024 /* More than will be written. */];
