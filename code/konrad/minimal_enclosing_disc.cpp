using P = pair <ld, ld>;
using circle = pair <P, ld>; //Środek, kwadrat promienia
const ld eps = 1e-10;
pii in[nax];
//operatory: P + P, P - P, P * ld, P / ld
ld sq_dist(P a, P b) {
	a = a - b;
	return a.first * a.first + a.second * a.second;
}
circle diam(P a, P b) {return {(a + b) / 2, sq_dist(a, b) / 4};}
circle opisany(P A, P B, P C) {
	ld a = sq_dist(B, C);
	ld b = sq_dist(C, A);
	ld c = sq_dist(A, B);
	if (a < eps) return diam(A, B); //Te ify są potrzebne
	if (b < eps) return diam(B, C); //tylko jak dwa punkty mogą
	if (c < eps) return diam(C, A); //się (prawie) pokrywać
	ld u = a * (b + c - a);
	ld v = b * (c + a - b);
	ld w = c * (a + b - c);
	if (u < 0)
		return diam(B, C);
	if (v < 0)
		return diam(A, C);
	if (w < 0)
		return diam(A, B);
	P O = (A * u + B * v + C * w) / (u + v + w);
	return {O, sq_dist(O, A)};
}
bool inside(circle a, P b) {
	return sq_dist(a.first, b) < a.second + eps;
}
circle go3(int i, int j, int n) {
	circle ans = opisany(in[0], in[i], in[j]);
	for (int x = 0; x < n; ++x)
		if (!inside(ans, in[x]))
			ans = opisany(in[x], in[i], in[j]);
	return ans;
}
circle go2(int i, int n) {
	circle ans = diam(in[i], in[0]);
	for (int x = 0; x < n; ++x)
		if (!inside(ans, in[x]))
			ans = go3(i, x, x);
	return ans;
}
circle go(int n) {
	circle ans = {in[0], 0};
	for (int i = 1; i < n; ++i)
		if (!inside(ans, in[i]))
			ans = go2(i, i);
	return ans;
}
int main() {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; ++i)
		scanf("%d%d", &in[i].first, &in[i].second);
	random_shuffle(in, in + n);
	circle ans = go(n);
	printf("%.10lf %.10lf\n%.10lf\n", (double)ans.first.first, (double) ans.first.second, (double)sqrt(ans.second));
}
