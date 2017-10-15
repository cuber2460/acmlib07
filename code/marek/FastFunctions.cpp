int FastMax(int x, int y) { return x - y >> 31 & (x ^ y) ^ x; }
int FastMin(int x, int y) { return x - y >> 31 & (x ^ y) ^ y; }
int FastAbs(int x)        { return (x ^ x >> 31) - (x >> 31); }
