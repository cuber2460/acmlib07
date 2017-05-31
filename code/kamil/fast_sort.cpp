void fast_sort(vector<unsigned> & t) {
	int n = t.size(), k = 1 << 16; // t[i] < k * k
	auto tmp = t; // if array: static int/unsigned tmp[nax];
	REP(turn, 2) {
		#define val(x) (turn ? x / k : x % k)
		vector<int> cnt(k+1);
		REP(i, n) cnt[val(t[i]) + 1]++;
		REP(i, k) cnt[i+1] += cnt[i];
		REP(i, n) tmp[ cnt[val(t[i])]++ ] = t[i];
		REP(i, n) t[i] = tmp[i];
		#undef val
	}
}
