const int inf = 1e9 + 5;
const int nax = 1e6 + 5;

inline int fastMax(int x, int y) { return (((y-x)>>(32-1))&(x^y))^y; }
inline int fastMin(int x, int y) { return (((y-x)>>(32-1))&(x^y))^x; }
inline int fastAbs(int n) { return (n ^ (n >> (32-1))) - (n >> (32-1)); }
