// Either run integral(A, B) once or split the interval [A, B] into up to ~1000
// smaller intervals -- if the function f behaves oddly or the interval is long.
ld simp(ld low, ld high, const ld * old, vector<ld> & nowe) {
	const int n = 500; // n must be even!!! Try n = 2 and n = 10.
	nowe.resize(n + 1);
	ld total = 0, jump = (high - low) / n;
	for(int i = 0; i <= n; ++i) {
		int mul = i == 0 || i == n ? 1 : 2 + i % 2 * 2; // 1 2 4 2 4 ... 2 1
		nowe[i] = !old || i % 2 ? f(low + i * jump) : old[i/2];
		total += nowe[i] * mul;   // uses a global function ld f(ld x)
	}
	return total * (high - low) / n / 3;
}
ld rec(ld low, ld high, ld prv, const vector<ld> & old) {
	ld mid = (low + high) / 2;
	vector<ld> left, right;
	ld L = simp(low, mid, old.data(), left);
	ld R = simp(mid, high, old.data() + old.size() / 2, right);
	if(abs(L + R - prv) < 1e-12L) return L + R; // eps ~ required abs precision
	return rec(low, mid, L, left) + rec(mid, high, R, right);
}
ld integral(ld low, ld high) {
	vector<ld> old;
	ld prv = simp(low, high, 0, old);
	return rec(low, high, prv, old);
}
