#define x first
#define y second
using ld = long double;
const ld eps = 1e-8;
ld K(ld a) {return a * a;}
using P = pair <ld, ld>;
P operator+(P a, P b) {return {a.x + b.x, a.y + b.y};}
P operator-(P a, P b) {return {a.x - b.x, a.y - b.y};}
P operator*(P a, ld b) {return {a.x * b, a.y * b};}
P operator*(ld b, P a) {return {a.x * b, a.y * b};}
void operator*=(P &a, ld b) {a.x *= b; a.y *= b;}
P operator/(P a, ld b) {return {a.x / b, a.y / b};}
P operator*(P a, P b) {return {a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x};}
ld vec(P a, P b) {return a.x * b.y - a.y * b.x;}
ld sca(P a, P b) {return a.x * b.x + a.y * b.y;}
ld sq_dist(P a, P b) {
	a = a - b;
	return K(a.first) + K(a.second);
}
ld dist(P a, P b) {return sqrt(sq_dist(a, b));}
ld roundpow2(ld x) {
	ld y = 1;
	while (y > x) y /= 2;
	while (y < x) y *= 2;
	return y;
}
bool iszero(ld x) {return abs(x) < eps;}
struct L {
	ld a, b, c; //ax + by = c
	L(ld a_ = 0, ld b_ = 0, ld c_ = 0) : a(a_), b(b_), c(c_) {
		//potrzebne tylko jak "intensywnie używamy" przecięć i wyznaczania prostej przez punkty
		//bez tego można dać pusty konstruktor
		ld s = sqrt(K(a) + K(b));
		s = roundpow2(s); //Tylko dlatego, że mnożenie/dzielenie przez potęgę dwójki nie gubi dokładności. 
		a /= s; b /= s; c /= s;
	}
	P v() {return {a, b};}
};
ld norm2(P a) {return K(a.first) + K(a.second);}
ld norm(P a) {return sqrt(norm2(a));}
struct C {P o; ld r;};
P rot(P a) {return {a.y, -a.x};}
muu & operator<<(muu &o, L x) {
	return o << "[" << x.a << "x + " << x.b << "y = " << x.c << "]";
}
muu & operator<<(muu &o, C x) {
	return o << "[O = " << x.o << ", r = " << x.r << "]";
}
L line_by_vec(P vec, P o) {return L(vec.x, vec.y, sca(vec, o));}
L make_line(P a, P b) {return L(a.y - b.y, b.x - a.x, vec(b, a));}
P project(L l, P p) {
	P v = l.v();
	return p - v * (l.a * p.x + l.b * p.y - l.c) / norm2(v);
}
ld dist(L l, P p) {
	return abs(l.a * p.x + l.b * p.y - l.c) / sqrt(l.a * l.a + l.b * l.b);
}
P symmetry(L l, P p) {//Wszystkie symetrie: Symetria(środek/oś, obiekt)
	P v = l.v();
	return p - 2 * v * (l.a * p.x + l.b * p.y - l.c) / norm2(v);
}
L symmetry(L l, L l1) {
	ld Q = 2 * (l1.a * l.a + l1.b * l.b) / (K(l.a) + K(l.b));
	return L(l1.a - l.a * Q, l1.b - l.b * Q, l1.c - l.c * Q);
}
C symmetry(L l, C c){return C{symmetry(l, c.o), c.r};} 
P symmetry(P o, P x){return 2 * o - x;} 
L symmetry(P o, L l){return L{l.a, l.b, 2 * o.x * l.a + 2 * o.y * l.b - l.c};} 
C symmetry(P l, C c){return C{symmetry(l, c.o), c.r};} 
P inter(L k, L l) {
	#define Q(i, j) (k.i * l.j - k.j * l.i) //Koniecznie w nawiasach!!!!!!!
	return P{Q(c, b) / Q(a, b), Q(a, c) / Q(a, b)};
	#undef Q
}
vector <P> inter(C c, L l) {
	ld h = dist(l, c.o);
	if (iszero(h - c.r)) return {project(l, c.o)};
	if (h > c.r) return {};
	ld k = sqrt(K(c.r) - K(h));
	P o = project(l, c.o);
	P d = rot(l.v());
	d *= k / norm(d);
	return {o - d, o + d};
}
vector <P> inter(C a, C b) {
	ld l = dist(a.o, b.o);
	if (l > a.r + b.r + eps || l < abs(a.r - b.r) - eps || l < eps) return {};
	return inter(a, L(2 * (a.o.x - b.o.x), 2 * (a.o.y - b.o.y),
				            K(b.r) - K(a.r) - norm2(b.o) + norm2(a.o)));
}
//rotate(środek, kąt, obiekt)
P rotate(P o, ld alpha, P x) {return o + (x - o) * P{cos(alpha), sin(alpha)};}
L rotate(P o, ld alpha, L l) {
	ld si = sin(alpha), co = cos(alpha);
	return L(l.a * co - l.b * si, l.a * si + l.b * co,
		l.c - l.a * (o.x * (1 - co) - o.y * si) - l.b * (o.y * (1 - co) + o.x * si));
}
C rotate(P o, ld alpha, C c) {return C{rotate(o, alpha, c.o), c.r};}
P scale(P o, ld sc, P x) {return x * sc + o * (1 - sc);}
C scale(P o, ld sc, C c) {return C{scale(o, sc, c.o), c.r * abs(sc)};}
L scale(P o, ld sc, L l) {
	return L(l.a / sc, l.b / sc, l.c - (l.a * o.x + l.b * o.y) * (1 - 1 / sc));
}
vector <L> tangent(C c, P p) {
	ld l = dist(c.o, p);
	if (l < c.r - eps) return {};
	if (l < c.r + eps) return {line_by_vec(p - c.o, p)};
	P to = p - c.o;
	to *= c.r / norm(to);
	ld co = c.r / l;
	ld si = sqrt(1 - K(co));
	P x = to * P{co, si};
	P y = to * P{co, -si};
	return {line_by_vec(x, c.o + x), line_by_vec(y, c.o + y)};
}
vector <L> tangent(C a, C b) { //Jeśli trzeba, to doifować case'y, kiedy któryś okrag ma promień 0
	vector <P> which;
	ld l = dist(a.o, b.o);
	if (l < eps) return {};
	P to = b.o - a.o;
	to *= a.r / norm(to);
	for (int d = -1; d <= 1; d += 2) {//d=-1 zewnętrzna styczna, d=1 wewnętrzna
		ld rr = a.r + b.r * d;
		if (abs(rr) > l + eps) continue;
		if (abs(rr) > l - eps) {
			if (rr > 0) which.emplace_back(1, 0);
			else which.emplace_back(-1, 0);
		}
		else {
			ld co = rr / l;
			ld si = sqrt(1 - K(co));
			which.emplace_back(co, si);
			which.emplace_back(co, -si);
		}
	}
	vector <L> ans;
	for (P x : which) {
		P y = x * to;
		ans.push_back(line_by_vec(y, a.o + y));
	}
	return ans;
}
L parallel(L l, P x) {return L(l.a, l.b, l.a * x.x + l.b * x.y);}
L perpendicular(L l, P x) {return L(l.b, -l.a, l.b * x.x - l.a * x.y);}
ld angle(P A, P O, P B) {//miara kąta AOB
	ld a = dist(A, O), b = dist(B, O), c = dist(A, B);
	ld co = (K(a) + K(b) - K(c)) / (2 * a * b);
	if (co > 1) co = 1;
	if (co < -1) co = -1;
	ld alpha = acos(co);
	if (vec(B - O, A - O) < 0)
		alpha = -alpha;
	return alpha;
}
L angle_bisector(P A, P O, P B) {//Dwusieczna kąta wewnętrznego
	ld a = dist(A, O);
	ld b = dist(O, B);
	return make_line(O, (A * b + B * a) / (a + b));
}
L segment_bisector(P a, P b) {
	return L(-2 * (b.x - a.x), -2 * (b.y - a.y) , norm2(a) - norm2(b));
}
ld det(ld a[3][3]) {
	ld r = 0; int s = 0;
	for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) if (i - j)
		r += (1 - (++s & 2)) * a[i][0] * a[j][1] * a[i ^ j ^ 3][2];
	return r;
}
bool collinear(P a, P b, P c) {
	ld x[3][3] = {a.x, a.y, 1, b.x, b.y, 1, c.x, c.y, 1};
	return iszero(det(x));
}
bool concurrent(L k, L l, L m) {
	ld x[3][3] = {k.a, k.b, k.c, l.a, l.b, l.c, m.a, m.b, m.c};
	return iszero(det(x));
}
bool parallel(L k, L l) {return iszero(k.a * l.b - k.b * l.a);}
bool perpendicular(L k, L l) {return iszero(k.a * l.a + k.b * l.b);}
bool on_line(L l, P p) {return iszero(l.a * p.x + l.b * p.y - l.c);}
bool on_circle(C c, P p) {return iszero(c.r - dist(p, c.o));}
P on(L x, ld t) {
	P r = P{x.a, x.b} / norm2(x.v()) * x.c + t * P{-x.b, x.a};
	return r;
}
ld vec(P a, P b, P c) {
	return vec(a, b) + vec(b, c) + vec(c, a);
}
//Zwraca i takie, że prosta o-poly[i] jest "styczna"
//Jeśli o jest wewnątrz wielokąta, to zwróci nieokreślony punkt
//Jeśli dir = true, to cały wielokąt leży na lewo od prostej o-poly[i]
//jeśli false, to na lewo niezależnie, czy poly jest clockwise, czy nie
int tangent(P o, const vector <P> &poly, bool dir) {
	int mul = dir ? 1 : -1;
	assert(poly.size() >= 3u);
	int n = poly.size();
	int ans = 0;
	int low = 0, high = n - 1;
	if (mul * vec(o, poly[0], poly[1]) > 0) {
		while (low <= high) {
			int med = (low + high) / 2;
			if (mul * vec(o, poly[med], poly[(med + 1) % n]) < 0 || mul * vec(o, poly[0], poly[med]) < 0) {
				ans = med;
				high = med - 1;
			}
			else
				low = med + 1;
		}
	}
	else {
		while (low <= high) {
			int med = (low + high) / 2;
			if (mul * vec(o, poly[med], poly[(med + 1) % n]) < 0 && mul * vec(o, poly[0], poly[med]) >= 0) {
				ans = med;
				high = med - 1;
			}
			else
				low = med + 1;
		}
	}
	return ans;
}
//Zwraca punkt X minimalizujący |XA| + |XB| + |XC| (punkt Fermata)
P fermat(P A, P B, P C) {
	static const ld dwa3 = 2 * sqrt(3);
	ld a = sq_dist(B, C);
	ld b = sq_dist(C, A);
	ld c = sq_dist(A, B);
	ld S = abs(vec(A - B, B - C)) * dwa3;
	ld u = K(a) - 2 * K(b - c) + a * (b + c + S);
	ld v = K(b) - 2 * K(c - a) + b * (c + a + S);
	ld w = K(c) - 2 * K(a - b) + c * (a + b + S);
	if (v < 0 && w < 0) return A;
	if (w < 0 && u < 0) return B;
	if (u < 0 && v < 0) return C;
	return (A * u + B * v + C * w) / (u + v + w);
}
//Zwraca środek okręgu opisanego
P opisany(P A, P B, P C) {
	ld a = sq_dist(B, C);
	ld b = sq_dist(C, A);
	ld c = sq_dist(A, B);
	ld u = a * (b + c - a);
	ld v = b * (c + a - b);
	ld w = c * (a + b - c);
	return (A * u + B * v + C * w) / (u + v + w);
}
