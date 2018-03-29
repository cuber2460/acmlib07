// Either run integral(A, B) once or split the interval [A, B] into 100-1000
// smaller intervals -- if the function behaves oddly or the interval is long.
ld _simp(ld low, ld high) {
	const int n = 500;
	ld total = 0, jump = (high - low) / n;
	for(int i = 0; i <= n; ++i) {
		int mno = 2;
		if(i & 1) mno = 4;
		else if(i == 0 || i == n) mno = 1;
		total += f(low + i * jump) * mno; // uses a global function ld f(ld x)
	}
	return total * (high - low) / n / 3;
}
ld integral(ld low, ld high, ld prv = 0, bool known = false) {
	if(!known) prv = _simp(low, high);
	ld mid = (low + high) / 2;
	ld L = _simp(low, mid), R = _simp(mid, high);
	if(abs(L + R - prv) < 1e-12L) return L + R;
	return integral(low, mid, L, true) + integral(mid, high, R, true);
}
