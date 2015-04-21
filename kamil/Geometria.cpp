struct P {
	ld x, y;
	ld zasieg;
	P(ld x = 0, ld y = 0) : x(x), y(y) {}
	P operator + (const P & b) const { return P(x+b.x, y+b.y); }
	P operator - (const P & b) const { return P(x-b.x, y-b.y); }
	P & operator += (const P & b) { return *this = *this + b; }
	P & operator -= (const P & b) { return *this = *this - b; }
	ll operator * (const P & b) const { return x*b.y - y*b.x; }
	ll dotProduct(const P & b) const { return x*b.x + y*b.y; }
	void wczytaj() { scanf("%Lf%Lf", &x, &y); }
	bool prawy() const { return x > 0 || (!x && y > 0); }
	bool lewy() const { return x < 0 || (!x && y < 0); }
	// bool operator < (const P & b) const { return *this * b < 0; }
	bool operator == (const P & b) const { return x == b.x && y == b.y; }
	ld norm2() { return sqrt(x * x + y * y); }
	ld angle() { return atan2(y, x); } // w radianach
	P rotate(ld angle) {
		ld s = sin(angle), c = cos(angle);
		return P(x * c - y * s, x * s + y * c);
	}
	void wypisz() { printf("(%Lf, %Lf  ...  %Lf),\n", x, y, angle()); }
};
const ld PI = 2 * acos(0);
ld na_stopnie(ld a) { return a * 180 / PI; }
ld na_radiany(ld a) { return a * PI / 180; }
vector<P> hull(vector<P> w) {
	sort(w.begin(), w.end(), [](const P & a, const P & b) {
		return a.x < b.x || (a.x == b.x && a.y < b.y);});
	w.resize(unique(w.begin(), w.end()) - w.begin());
	if(sz(w) <= 1) return w;
	vector<P> res;
	int L = 0;
	REP(_, 2) {
		for(P & C : w) {
			while((int) res.size() >= L + 2) {
				P A = res[sz(res) - 2];
				P B = res.back();
				if((C-A) * (B-A) > 0) break;
				res.pop_back();
			}
			res.pb(C);
		}
		res.pop_back();
		reverse(w.begin(), w.end());
		L = (int) res.size();
	}
	return res;
}

struct Circle {
  Point center;
  LD r;
  Circle(LD x, LD y, LD rad) {
    center = Point(x, y);
    r = rad;
  }
  Circle(const Point& a, LD rad) : center(a), r(rad) {}
  LD Area() const {
    return kPi * r * r;
  }
  LD Perimeter() const {
    return 2 * kPi * r;
  }
  LD Diameter() const {
    return 2 * r;
  }
  Point RotateRightMost(LD ang) const {
    Point right_most(r, 0);
    Point res = right_most.Rotate(ang);
    return center + res;
  }
  bool operator==(const Circle& c) const {
    return center == c.center && fabs(r - c.r) < kEps;
  }
};

struct Line {
  Point p[2];
  bool is_seg;
  Line(Point a, Point b, bool is_seg_ = false) {
    p[0] = a;
    p[1] = b;
    is_seg = is_seg_;
  }
  Line() {
  }
  Point& operator[](int a) {
    return p[a];
  }
  static bool VectorsInOneHalfPlaneCmp(Line a, Line b) {
    return (a[1] - a[0]).CrossProd(b[1] - b[0]) > 0;
  }
  Point NormalVector() {
    Point perp = p[1] - p[0];
    perp.RotateSelf(kPi / 2);
    perp.NormalizeSelf();
    return perp;
  }
  vector<LD> LineEqNormLD() { // seems ok
    LD A = p[1].y - p[0].y;
    LD B = p[0].x - p[1].x;
    LD C = -(A * p[0].x + B * p[0].y);
    assert(fabs(A * p[1].x + B * p[1].y + C) < kEps);
    LD norm = sqrt(A * A + B * B);
    vector<LD> res{A, B, C};
    for (auto& x : res) { x /= norm; }
    if (A < -kEps || (fabs(A) < kEps && B < -kEps)) {
      for (auto& x : res) { x *= -1; }
    }
    return res;
  }
  
  // assumes that coordinates are integers!
  vector<int> LineEqNormInt() { // seems ok
    int A = round(p[1].y - p[0].y);
    int B = round(p[0].x - p[1].x);
    int C = -(A * p[0].x + B * p[0].y);
    int gcd = abs(__gcd(A, __gcd(B, C)));
    vector<int> res{A, B, C};
    for (auto& x : res) { x /= gcd; }
    if (A < 0 || (A == 0 && B < 0)) {
      for (auto& x : res) { x *= -1; }
    }
    return res;
  }
};

struct Utils {
  static vector<Point> InterCircleCircle(Circle& a, Circle& b) {
    Circle* c = &a;
    Circle* d = &b;
    if (a.r + kEps < b.r) {
      swap(c, d);
    }
    if (a == b) {
      return vector<Point>{a.RotateRightMost(0), a.RotateRightMost(2 * kPi / 3),
          a.RotateRightMost(4 * kPi / 3)};
    }
    Point diff = d->center - c->center;
    LD dis = diff.Norm();
    LD ang = diff.Angle();
    LD longest = max(max(c->r, d->r), dis);
    if (2 * longest > c->r + d->r + dis + kEps) {
      return vector<Point>();
    }
    if (fabs(2 * longest - c->r - d->r - dis) < 2 * kEps) {
      return vector<Point>{c->RotateRightMost(ang)};
    }
    LD proj_diff = (c->r * c->r - d->r * d->r) / dis;
    LD r1_proj = (dis + proj_diff) / 2;
    LD ang_deviation = acos(r1_proj / c->r);
    LD ang1 = ang - ang_deviation;
    LD ang2 = ang + ang_deviation;
    return vector<Point>{c->RotateRightMost(ang1), c->RotateRightMost(ang2)};
  }
  static Point InterLineLine(Line& a, Line& b) { // working fine
    Point vec_a = a[1] - a[0];
    Point vec_b1 = b[1] - a[0];
    Point vec_b0 = b[0] - a[0]; 
    LD tr_area = vec_b1.CrossProd(vec_b0);
    LD quad_area = vec_b1.CrossProd(vec_a) + vec_a.CrossProd(vec_b0);
//     if (abs(quad_area) < kEps) {
//       throw Parallel();
//     }
    return Point(a[0] + vec_a * (tr_area / quad_area));
  }
    
  static Point ProjPointToLine(Point p, Line l) { ///Tested
    l[0] -= p;
    l[1] -= p;
    Point normal = l.NormalVector();
    normal *= normal.DotProd(l[0]);
    return normal + p;
  }
  
  static Point ReflectPtWRTLine(Point p, Line l) {
    Point proj = ProjPointToLine(p, l);
    return proj * 2 - p;
  }
  
  static vector<Point> InterCircleLine(Circle c, Line l) { // seems ok
    Point proj = ProjPointToLine(c.center, l);
    LD dis_proj = c.center.Dist(proj);
    if (dis_proj > c.r + kEps) { return vector<Point>(); }
    if (fabs(dis_proj - c.r) < kEps) { return vector<Point>{proj}; }
    LD a = sqrt(c.r * c.r - dis_proj * dis_proj);
    Point dir = l[1] - l[0];
    dir.NormalizeSelf();
    return vector<Point>{proj + dir * a, proj - dir * a};
  }
  
  static bool PtBelongToLine(Line l, Point p) {
    return fabs(l[0].CrossProd(l[1], p)) < kEps;
  }
  
  static bool PtBelongToSeg(Line l, Point p) { // seems ok
    return fabs(p.Dist(l[0]) + p.Dist(l[1]) - l[0].Dist(l[1])) < kEps;
  }
  
  static vector<Point> InterCircleSeg(Circle c, Line l) { //seems ok
    vector<Point> from_line = InterCircleLine(c, l);
    vector<Point> res;
    for (auto p : from_line) {
      if (PtBelongToSeg(l, p)) { res.PB(p); }
    }
    return res;
  }
  
  static vector<Point> TangencyPtsToCircle(Circle c, Point p) { // seems ok
    assert(c.center.Dist(p) > c.r + kEps); // or return vector<Point>() if needed
    Point mid = (c.center + p) / 2;
    Circle aux_circ = Circle(mid, c.center.Dist(p) / 2);
    return InterCircleCircle(aux_circ, c);
  }
  
  static bool AreParallel(Line l1, Line l2) { // seems ok
    return fabs(l1[0].CrossProd(l2[0], l1[1]) - l1[0].CrossProd(l2[1], l1[1])) < kEps;
  }
  
  // returns a vector of points such that their convex hull is intersection of those segments
  // SZ(0) = 0 => empty intersection, SZ(1) = 1 => intersection is a point, SZ(2) => intersection is a segment
  static vector<Point> InterSegs(Line l1, Line l2) { // seems ok
    if (!Point::LexCmp(l1[0], l1[1])) { swap(l1[0], l1[1]); }
    if (!Point::LexCmp(l2[0], l2[1])) { swap(l2[0], l2[1]); }
    if (AreParallel(l1, l2)) {
      if (!PtBelongToLine(l1, l2[0])) { return vector<Point>(); }
      vector<Point> ends(2);
      for (int tr = 0; tr < 2; tr++) {
        if (Point::LexCmp(l1[tr], l2[tr]) ^ tr) {
          ends[tr] = l2[tr];
        } else {
          ends[tr] = l1[tr];
        }
      }
      if ((ends[1] - ends[0]).IsZero()) {
        ends.pop_back();
      }
      if (SZ(ends) == 2 && Point::LexCmp(ends[1], ends[0])) { return vector<Point>(); }
      return ends;
    } else {
      Point p = InterLineLine(l1, l2);
      if (PtBelongToSeg(l1, p) && PtBelongToSeg(l2, p)) { return vector<Point>{p}; }
      return vector<Point>();
    }
  }
};
