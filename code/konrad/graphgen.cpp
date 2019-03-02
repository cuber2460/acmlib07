//Generuje wszystkie nieizomorficzne grafy o n wierzchołkach
//Dla n = 9 liczy się około minuty
//W kolejności od najmniejszej liczby krawędzi
const int nax = 12;
int n;
using ull = unsigned long long;
using graph = array <array <bool, nax>, nax>;
using arrh = array<ull, nax>;
ostream & operator<<(ostream &d, graph g) {
	int q = 0;
	d << "{";
	for (int i = 0; i < nax; ++i)
		for (int j = i + 1; j < nax; ++j)
			if (g[i][j])
				d << ", " + 2 * !q++ << i << "-" << j;
	return d << "}";
}
arrh hashes(const graph &g) {
	arrh ans;
	for (int i = 0; i < n; ++i) ans[i] = 135792468;
	for (int _ = 0; _ < n / 2; ++_) {
		arrh curr = array<ull, nax>();
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				if (g[i][j])
					curr[i] ^= ans[j] * ans[j];
		for (int i = 0; i < n; ++i)
			curr[i] ^= (ans[i] + curr[i]) % 123456789;
		swap(ans, curr);
	}
	return ans;
}
ull make_hash(arrh c) {
	ull ans = 0;
	sort(c.begin(), c.begin() + n);
	for (int i = 0; i < n; ++i)
		ans += (i + 1) * c[i];
	return ans;
}
bool used[nax];
graph g1, g2;
arrh h1, h2;
int per[nax];
bool bt(int i) {
	if (i == n) return true;
	for (int j = 0; j < n; ++j) if (!used[j] && h1[i] == h2[j]) {
		bool good = true;
		for (int k = 0; k < i; ++k)
			if (g1[i][k] != g2[j][per[k]]) {
				good = false;
				break;
			}
		if (good) {
			per[i] = j;
			used[j] = true;
			if (bt(i + 1)) return true;
			used[j] = false;
		}
	}
	return false;
}
bool isom(graph _g1, arrh _h1, graph _g2, arrh _h2) {
	g1 = _g1, g2 = _g2, h1 = _h1, h2 = _h2;
	for (int i = 0; i < n; ++i) used[i] = false;
	return bt(0);
}
vector <graph> graphs[nax * nax];//graphs[i] - grafy mające i krawędzi
int ile = 1;
void go() {
	graphs[0].resize(1);
	for (int r = 0; r < n * (n - 1) / 2; ++r) {
		map <ull, vector <pair <graph, array <ull, nax> > > > all;
		for (graph g : graphs[r]) {
			for (int i = 0; i < n; ++i)
				for (int j = i + 1; j < n; ++j) 
					if (!g[i][j]) {
						graph gn = g;
						gn[i][j] = gn[j][i] = true;
						//if (gn ani żadne jego nadgrafy nas nie interesują) continue;
						auto H = hashes(gn);
						auto &v = all[make_hash(H)];
						bool good = true;
						for (auto c : v)
							if (isom(c.first, c.second, gn, H)) {
								good = false;
								break;
							}
						if (good) {
							static int show = 5;
							ile++;
							if (ile == show) {
								cerr << imie(ile) " time: " << (clock() / (ld) CLOCKS_PER_SEC) << "s" << endl;
								show *= 1.4;
							}
							v.emplace_back(gn, H);
							debug << imie(gn);
						}
					}
		}
		for (auto &a : all)
			for (auto &b : a.second)
				graphs[r + 1].push_back(b.first);
		cerr << r << " edges done; time: " << (clock() / (ld) CLOCKS_PER_SEC) << "s" << endl;
	}
}
int main() {
	scanf("%d", &n);
	go();
	cerr << imie(ile) << endl;
}
