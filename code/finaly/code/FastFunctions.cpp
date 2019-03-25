// Działa dla wszystkich par (x, y), dla których (x - y) mieści się w typie int.
int FastMax(int x, int y) { return x - y >> 31 & (x ^ y) ^ x; }
int FastMin(int x, int y) { return x - y >> 31 & (x ^ y) ^ y; }
// Działa dla wszystkich x oprócz -2**31 (bo 2**31 się nie mieści w typie int).
int FastAbs(int x)        { return (x ^ x >> 31) - (x >> 31); }
