struct Compress {
  static unsigned char data[];
  int position = -1;
  int left = 0;

  uint64_t Data(int bits, uint64_t x = 0) {
    uint64_t result = 0;
    assert(1 <= bits and bits <= 64);
    assert(bits == 64 or x < (1llu << bits));
    if (left > 0) {
      const int b = min(left, bits);
      bits -= b;
      left -= b;
      #define MASKA(x, y) (x & ((1 << (y)) - 1))
      result = (uint64_t) (MASKA(data[position] >> left, b)) << bits;
      data[position] |= MASKA(x >> bits, b) << left;
      #undef MASKA
    }
    while (bits >= 8) {
      bits -= 8;
      position++;
      result |= (uint64_t) data[position] << bits;
      data[position] = x >> bits;
    }
    if (bits > 0) {
      left = 8 - bits;
      position++;
      result |= data[position] >> left;
      data[position] = x << left;
    }
    return result;
  }

  void Print() {
    constexpr char code[] = "KaMylMaDoWnA";  // Length must be <= 16.
    std::ofstream out("compress.cpp");
    out << "unsigned char Compress::data[] = R\"" << code << "(";
    out.write(reinterpret_cast<const char*>(data), position + 1);
    out << ")" << code << "\";\n";
  }
};

unsigned char Compress::data[2 * 128 * 1024 /* More than will be written. */];
