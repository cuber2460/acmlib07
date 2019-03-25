struct Compress {//7 na 8 bitów używa
	static unsigned char data[];
	int pos = -1, left = 1;
	uint64_t Data(int bits, uint64_t x = 0) {
		assert(1 <= bits and bits <= 64);
		for (int i = 0; i < bits; i++) {
			if (left-- == 1) { pos++; left = 7; }
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
		out.write(reinterpret_cast<const char*>(data), pos + 1);
		out << ")" << code << "\";\n";
	}
};
unsigned char Compress::data[2 * 128 * 1024 /* More than will be written. */];
