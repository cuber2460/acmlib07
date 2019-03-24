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
struct C {
	P o;
	ld r;	
};
P rot(P a) {return {a.y, -a.x};}
muu & operator<<(muu &o, L x) {
	return o << "[" << x.a << "x + " << x.b << "y = " << x.c << "]";
}
muu & operator<<(muu &o, C x) {
	return o << "[O = " << x.o << ", r = " << x.r << "]";
}
L line_by_vec(P vec, P o) {return L(vec.x, vec.y, sca(vec, o));}
L make_line(P a, P b) {return L(a.y - b.y, b.x - a.x, vec(b, a));}
P project(L l, P p) {//TESTED
	P v = l.v();
	return p - v * (l.a * p.x + l.b * p.y - l.c) / norm2(v);
}
ld dist(L l, P p) {//TESTED
	return abs(l.a * p.x + l.b * p.y - l.c) / sqrt(l.a * l.a + l.b * l.b);
}
//symetria(środek/oś, obiekt)
P symmetry(L l, P p) {//TESTED
	P v = l.v();
	return p - 2 * v * (l.a * p.x + l.b * p.y - l.c) / norm2(v);
}
L symmetry(L l, L l1) {//TESTED
	ld Q = 2 * (l1.a * l.a + l1.b * l.b) / (K(l.a) + K(l.b));
	return L(l1.a - l.a * Q, l1.b - l.b * Q, l1.c - l.c * Q);
}
C symmetry(L l, C c){return C{symmetry(l, c.o), c.r};} //TESTED
P symmetry(P o, P x){return 2 * o - x;} //TESTED
L symmetry(P o, L l){return L{l.a, l.b, 2 * o.x * l.a + 2 * o.y * l.b - l.c};} //TESTED
C symmetry(P l, C c){return C{symmetry(l, c.o), c.r};} //TESTED
//Przecięcia
P inter(L k, L l) {//TESTED
	#define Q(i, j) (k.i * l.j - k.j * l.i) //Koniecznie w nawiasach!!!!!!!
	return P{Q(c, b) / Q(a, b), Q(a, c) / Q(a, b)};
	#undef Q
}
vector <P> inter(C c, L l) {//TESTED
	ld h = dist(l, c.o);
	if (iszero(h - c.r)) return {project(l, c.o)};
	if (h > c.r) return {};
	ld k = sqrt(K(c.r) - K(h));
	P o = project(l, c.o);
	P d = rot(l.v());
	d *= k / norm(d);
	return {o - d, o + d};
}
vector <P> inter(C a, C b) {//TESTED
	ld l = dist(a.o, b.o);
	if (l > a.r + b.r + eps || l < abs(a.r - b.r) - eps || l < eps) return {};
	return inter(a, L(2 * (a.o.x - b.o.x), 2 * (a.o.y - b.o.y),
				            K(b.r) - K(a.r) - norm2(b.o) + norm2(a.o)));
}
//rotate(środek, kąt, obiekt)
P rotate(P o, ld alpha, P x) {//TESTED
	return o + (x - o) * P{cos(alpha), sin(alpha)};
}
L rotate(P o, ld alpha, L l) {//TESTED
	ld si = sin(alpha), co = cos(alpha);
	return L(l.a * co - l.b * si, l.a * si + l.b * co,
		l.c - l.a * (o.x * (1 - co) - o.y * si) - l.b * (o.y * (1 - co) + o.x * si));
}
C rotate(P o, ld alpha, C c) {//TESTED
	return C{rotate(o, alpha, c.o), c.r};
}
P scale(P o, ld sc, P x) {//TESTED
	return x * sc + o * (1 - sc);
}
C scale(P o, ld sc, C c) {//TESTED
	return C{scale(o, sc, c.o), c.r * abs(sc)};
}
L scale(P o, ld sc, L l) {//TESTED
	return L(l.a / sc, l.b / sc, l.c - (l.a * o.x + l.b * o.y) * (1 - 1 / sc));
}
//styczne do okręgu
vector <L> tangent(C c, P p) {//TESTED
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
//TESTED
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
//prosta prostopadła/równoległa przechodząca przez punkt
L parallel(L l, P x) {
	return L(l.a, l.b, l.a * x.x + l.b * x.y);
}
L perpendicular(L l, P x) {
	return L(l.b, -l.a, l.b * x.x - l.a * x.y);
}
//TESTED
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
	ld a = dist(A, O);//TESTED
	ld b = dist(O, B);
	return make_line(O, (A * b + B * a) / (a + b));
}
L segment_bisector(P a, P b) {
	return L(-2 * (b.x - a.x), -2 * (b.y - a.y) , norm2(a) - norm2(b));
}
//Sprawdzanie własności
ld det(ld a[3][3]) {
	ld r = 0;
	int s = 0;
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
bool parallel(L k, L l) {
	return iszero(k.a * l.b - k.b * l.a);
}
bool perpendicular(L k, L l) {//TESTED
	return iszero(k.a * l.a + k.b * l.b);
}
bool on_line(L l, P p) {//TESTED
	return iszero(l.a * p.x + l.b * p.y - l.c);
}
bool on_circle(C c, P p) {//TESTED
	return iszero(c.r - dist(p, c.o));
}
P on(L x, ld t) {
	P r = P{x.a, x.b} / norm2(x.v()) * x.c + t * P{-x.b, x.a};
	return r;
}
