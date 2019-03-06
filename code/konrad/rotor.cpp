using pii = pair <int, int>;
const int nax = 2e3 + 3;
pii in[nax];
int wh[nax];
pair <pii, pii> dir[nax * nax / 2];
pii operator-(pii a, pii b) {return {a.first - b.first, a.second - b.second;}}
ll pro(pii a, pii b) {return a.first * 1ll * b.second - a.second * 1ll * b.first;}
bool cmp(pair <pii, pii> a, pair <pii, pii> b) {
	ll p = pro(a.first, b.first);
	if (p > 0) return 1;
	if (p < 0) return 0;
	return a.second < b.second;
	//Jak nie może być trzech współliniowych to po prostu: return pro(a.first, b.first) > 0;
}
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		scanf("%d%d", &in[i].first, &in[i].second);
	sort(in, in + n);
	for (int i = 0; i < n; ++i)
		wh[i] = i;
	int cou = 0;
	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j)
			dir[cou++] = {max(in[i] - in[j], in[j] - in[i]), {i, j}};
	sort(dir, dir + cou, cmp);
	for (int i = 0; i < cou; ++i) {
		debug << imie(i) imie(dir[i]);
		auto c = dir[i].second;
		pii x = in[wh[c.first]], y = in[wh[c.second]];
		swap(wh[c.first], wh[c.second]);
		swap(in[wh[c.first]], in[wh[c.second]]);
		//Policz wynik dla posortowanych prostopadle do prostej xy
	}
}
