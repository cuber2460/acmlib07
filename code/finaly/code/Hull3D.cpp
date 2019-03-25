#define REP(i, n) for (int i = 0; i < int(n); ++i)
// algorytm znajduje zasmiecona otoczke (z wszystkimi punktami na scianach)
// jesli chcemy miec tylko wierzcholki otoczki, to odkomentowujemy
// uwaga na ll (nie ma ich w iloczynie wektorowym) i ew. przepelnienie
// jak przerabiamy na double'e, to sciany grupujemy po znormalizowanym
// wektorze normalnym
// klasyfikacja punktow, patrzymy do ilu roznych (pod wzgledem wektorow
// normalnych)
// scian nalezy dany punkt:
// 0 - nie naleze do wypuklej otoczki
// 1 - punkt wewnetrzny sciany
// 2 - punkt wewnatrz krawedzi
// 3 i wiecej - wierzcholek
const int N = 1000;
int n;
struct sciana {
  int t[3]; // numery wierzcholkow sciany
  sciana() {}
  sciana(int a, int b, int c) {
    t[0] = a; t[1] = b; t[2] = c;
  }
};
using ll = long long;
#define K(x) x *x
struct P3 {
  ll x, y, z;
  void read() { cin >> x >> y >> z; }
  bool operator<(const P3 &he) const { // only for map
    return vector<ll>{x, y, z} < vector<ll>{he.x, he.y, he.z};
  }
  ll dot(const P3 &he) { return (ll)x * he.x + (ll)y * he.y + (ll)z * he.z; }
  P3 operator-(const P3 &he) { return P3{x - he.x, y - he.y, z - he.z}; }
  bool operator==(const P3 &he) { return x == he.x && y == he.y && z == he.z; }
  P3 operator^(const P3 &he) {
    return P3{y * he.z - z * he.y, z * he.x - x * he.z, x * he.y - y * he.x};
  }
  ll mno(const P3 &b, const P3 &c) const {
    return x * b.y * c.z + y * b.z * c.x + z * b.x * c.y - x * b.z * c.y -
           y * b.x * c.z - z * b.y * c.x;
  }
  double norm() { return sqrt(double(K(x) + K(y) + K(z))); }
};
vector<sciana> v;
vector<P3> p;
vector<vector<int>> t; // numer sciany do ktorej nalezy krawedz
void step(int x) {
  // znajdujemy widoczne sciany
  vector<bool> vis(v.size());
  REP(i, v.size()) {
    P3 normal = ((p[v[i].t[1]] - p[v[i].t[0]]) ^ (p[v[i].t[2]] - p[v[i].t[0]]));
    ll il = (normal.dot(p[x] - p[v[i].t[0]]));
    vis[i] = false;
    if (il > 0) vis[i] = true; /* uwaga na double */
    else if (il == 0) {
      if ((normal.dot((p[v[i].t[1]] - p[v[i].t[0]]) ^ (p[x] - p[v[i].t[0]])) >= 0 &&
           normal.dot((p[v[i].t[2]] - p[v[i].t[1]]) ^ (p[x] - p[v[i].t[1]])) >= 0 &&
           normal.dot((p[v[i].t[0]] - p[v[i].t[2]]) ^ (p[x] - p[v[i].t[2]])) >= 0))
        //~ return; /*ODKOMENTUJ*/
        vis[i] = true;
    }
  }
  int ile = v.size();
  vector<pair<int, int>> pom;
  REP(i, ile)
  if (vis[i]) REP(j, 3) if (!vis[t[v[i].t[(j + 1) % 3]][v[i].t[j]]])
      pom.push_back(make_pair(v[i].t[j], v[i].t[(j + 1) % 3]));
  REP(i, ile) if (vis[i]) {
    swap(v[i], v.back());
    REP(j, 3) t[v[i].t[j]][v[i].t[(j + 1) % 3]] = i;
    vis[i--] = vis[--ile];
    v.pop_back();
  }
  REP(i, v.size()) assert(!vis[i]);
  for (pair<int, int> pa : pom) {
    t[pa.first][pa.second] = t[pa.second][x] = t[x][pa.first] = v.size();
    v.push_back(sciana(pa.first, pa.second, x));
  }
}
void CH3D() { // n>=3, oblicza wektor trojkatnych scian v (mozliwe powtorzenia)
  int i = 2;
  P3 normal;
  while (i < n && (normal = ((p[1]-p[0]) ^ (p[i]-p[0]))) == P3{0, 0, 0} /* uwaga na double */) i++;
  if (i >= n) return; // wspolliniowe
  int x = i++;
  v.push_back(sciana(0, 1, x));
  v.push_back(sciana(1, 0, x));
  t[0][1] = t[1][x] = t[x][0] = 0;
  t[1][0] = t[0][x] = t[x][1] = 1;
  while (i < n && normal.dot(p[i] - p[0]) == 0 /* uwaga na double */) i++;
  // wspolplaszczyznowe, tworzona jest sztuczna sciana dla kazdej
  if (i == n) { // krawedzi!!!
    cerr << "deeeebug  wspolplaszczyznowe\n";
    v.clear();
    p.push_back(P3{3123, -3123, 954}); // pkt spoza plaszczyzny, uwaga na zakresy!
    n++; t.clear();
    t = vector<vector<int>>(n, vector<int>(n));
    CH3D();
    n--; t.pop_back();
    return;
  }
  step(i);
  for (int j = 2; j <= n - 1; ++j) if (j != i && j != x) step(j);
}
/****************** wszystko ponizej jest opcjonalne ******************************/
double area() {
  double res = 0.0;
  debug() << imie(v.size());
  REP(i, v.size()) {
    P3 normal = ((p[v[i].t[1]] - p[v[i].t[0]]) ^ (p[v[i].t[2]] - p[v[i].t[0]]));
    res += normal.norm();
  }
  return 0.5 * res;
}
double volume() {
  double res = 0.0; 
  REP(i, v.size()) {
    res += p[v[i].t[0]].mno(p[v[i].t[1]], p[v[i].t[2]]);
    debug() << imie((double)p[v[i].t[0]].mno(p[v[i].t[1]], p[v[i].t[2]]));
    //~ P3 normal=((p[v[i].t[1]]-p[v[i].t[0]])^(p[v[i].t[2]]-p[v[i].t[0]]));
    //~ double
    //foo=(double)normal.x*normal.x+(double)normal.y*normal.y+(double)normal.z*normal.z;
    //~ foo=sqrt(foo);
    //~ res+=foo;
  }
  return abs(res) / 6;
}
map<P3, vector<int>> mapa; // sciany (uwaga na double)
vector<int> klas;
void compute_walls() { // laczy sciany trojkatne w wielokaty i wyznacza
  REP(i, v.size()) {   // klasyfikacje, najpierw odpal CH3D()
    P3 normal = ((p[v[i].t[1]] - p[v[i].t[0]]) ^ (p[v[i].t[2]] - p[v[i].t[0]]));
    ll foo = __gcd(abs(normal.x), __gcd(abs(normal.y), abs(normal.z))); // uwaga na double
    normal.x /= foo; normal.y /= foo; normal.z /= foo;
    REP(j, 3) if (v[i].t[j] < n) mapa[normal].push_back(v[i].t[j]);
  }
  for (auto &pa : mapa) {
    sort(pa.second.begin(), pa.second.end());
    pa.second.erase(unique(pa.second.begin(), pa.second.end()), pa.second.end());
    for (int j : pa.second) assert(j < n), klas[j]++;
  }
}
/* sortuje wierzcholki na scianach w kolejnosci, tylko dla wersji bez smieci */
int pocz;
#define norm norm_compile
P3 norm;
bool CHcomp(int x, int y) {
  if (x == pocz) return true;
  if (y == pocz) return false;
  return ((p[x] - p[pocz]) ^ (p[y] - p[pocz])).dot(norm) < 0;
}
void sort_walls() { // najpierw odpal compute_walls()
  for (auto &pa : mapa) {
    vector<int> &w = pa.second;
    if ((int)w.size() == 2) continue;
    pocz = w[0];
    norm = (p[w[1]] - p[w[0]]) ^ (p[w[2]] - p[w[0]]);
    sort(w.begin(), w.end(), CHcomp);
  }
}
void show() {
  cout << "Sciany trojkatne:" << endl;
  for (sciana i : v) cout << p[i.t[0]] << ' ' << p[i.t[1]] << ' ' << p[i.t[2]] << endl;
  REP(i, n) cout << p[i] << " " << klas[i] << endl; // klasyfikacja
  for (auto pa : mapa) { cout << pa.first << ": "; // sciany
    for (int x : pa.second) cout << p[x] << " "; cout << endl;
  }
  printf("%6lf %6lf\n", area(), volume());
}
void test_case() {
  cin >> n;
  klas = vector<int>(n);
  p.clear(); p.resize(n);
  t = vector<vector<int>>(n, vector<int>(n));
  REP(i, n) p[i].read();
  // random_shuffle(p.begin(),p.end());
  CH3D();
  compute_walls(); sort_walls(); show();
  pocz = 0;
  mapa.clear(); v.clear();
}
