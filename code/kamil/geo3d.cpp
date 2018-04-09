#include "Geo2D.h"
struct Point3 {
  LD x, y, z; // ... + trivial operators
  LD& operator[](int a) {
    if (a == 0) { return x; } if (a == 1) { return y; } return z;
  }
  bool IsZero() { return abs(x) < kEps && abs(y) < kEps && abs(z) < kEps; }
  LD DotProd(Point3 a) { return x * a.x + y * a.y + z * a.z; }
  LD Norm() { return sqrt(x * x + y * y + z * z); }
  LD SqNorm() { return x * x + y * y + z * z; }
  void NormalizeSelf() { *this /= Norm(); }
  Point3 Normalize() {
    Point3 res(*this); res.NormalizeSelf(); return res;
  }
  LD Dis(Point3 a) { return (*this - a).Norm(); }
  pair<LD, LD> SphericalAngles() { return {atan2(z, sqrt(x * x + y * y)), atan2(y, x)}; }
  LD Area(Point3 p) { return Norm() * p.Norm() * sin(Angle(p)) / 2; }
  LD Angle(Point3 p) {
    LD a = Norm(); LD b = p.Norm(); LD c = Dis(p);
    return acos((a * a + b * b - c * c) / (2 * a * b));
  }
  static LD Angle(Point3 p, Point3 q) { return p.Angle(q); }
  Point3 CrossProd(Point3 p) {
    Point3 q(*this);
    return {q[1] * p[2] - q[2] * p[1],
            q[2] * p[0] - q[0] * p[2],
            q[0] * p[1] - q[1] * p[0]};
  }
  static bool LexCmp(Point3& a, const Point3& b) {
    if (abs(a.x - b.x) > kEps) { return a.x < b.x; }
    if (abs(a.y - b.y) > kEps) { return a.y < b.y; }
    return a.z < b.z;
  }
  friend ostream& operator<<(ostream& out, Point3 m);
};
struct Line3 {
  Point3 p[2]; Point3& operator[](int a) { return p[a]; }
  friend ostream& operator<<(ostream& out, Line3 m);
};
struct Plane {
  Point3 p[3];
  Point3& operator[](int a) { return p[a]; }
  Point3 GetNormal() {
    Point3 cross = (p[1] - p[0]).CrossProd(p[2] - p[0]); return cross.Normalize();
  }
  void GetPlaneEq(LD& A, LD& B, LD& C, LD& D) {
    Point3 normal = GetNormal();
    A = normal[0]; B = normal[1]; C = normal[2];
    D = normal.DotProd(p[0]);
    assert(abs(D - normal.DotProd(p[1])) < kEps);
    assert(abs(D - normal.DotProd(p[2])) < kEps);
  }
  vector<Point3> GetOrtonormalBase() {
    Point3 normal = GetNormal();
    Point3 cand = {-normal.y, normal.x, 0};
    if (abs(cand.x) < kEps && abs(cand.y) < kEps) { cand = {0, -normal.z, normal.y}; }
    cand.NormalizeSelf();
    Point3 third = Plane{Point3{0, 0, 0}, normal, cand}.GetNormal();
    assert(abs(normal.DotProd(cand)) < kEps &&
           abs(normal.DotProd(third)) < kEps && abs(cand.DotProd(third)) < kEps); 
    return {normal, cand, third};
  }
};
struct Circle3 {
  Plane pl; Point3 o; LD r;
  friend ostream& operator<<(ostream& out, Circle3 m);
};
struct Sphere {
  Point3 cent; LD r;
};
struct Utils3 {
  static bool Lines3Equal(Line3 p, Line3 l) {
    return Utils3::PtBelongToLine3(p[0], l) && Utils3::PtBelongToLine3(p[1], l);
  }
  //angle PQR
  static LD Angle(Point3 P, Point3 Q, Point3 R) { return (P - Q).Angle(R - Q); }
  static Point3 ProjPtToLine3(Point3 p, Line3 l) { // ok
    Point3 diff = l[1] - l[0]; diff.NormalizeSelf();
    return l[0] + diff * (p - l[0]).DotProd(diff);
  }
  static LD DisPtLine3(Point3 p, Line3 l) { // ok
//  LD area = Area(p, l[0], l[1]); LD dis1 = 2 * area / l[0].Dis(l[1]);
    LD dis2 = p.Dis(ProjPtToLine3(p, l)); // assert(abs(dis1 - dis2) < kEps);
    return dis2;
  }
  static LD DisPtPlane(Point3 p, Plane pl) {
    Point3 normal = pl.GetNormal(); return abs(normal.DotProd(p - pl[0]));
  }
  static Point3 ProjPtToPlane(Point3 p, Plane pl) {
    Point3 normal = pl.GetNormal(); return p - normal * normal.DotProd(p - pl[0]);
  }
  static bool PtBelongToPlane(Point3 p, Plane pl) { return DisPtPlane(p, pl) < kEps; }
  static Point PlanePtTo2D(Plane pl, Point3 p) { // ok
    assert(PtBelongToPlane(p, pl));
    vector<Point3> base = pl.GetOrtonormalBase();
    Point3 control{0, 0, 0};
    REP (tr, 3) { control += base[tr] * p.DotProd(base[tr]); }
    assert(PtBelongToPlane(pl[0] + base[1], pl));
    assert(PtBelongToPlane(pl[0] + base[2], pl));
    assert((p - control).IsZero());
    return {p.DotProd(base[1]), p.DotProd(base[2])};
  }
  static Line PlaneLineTo2D(Plane pl, Line3 l) {
    return {PlanePtTo2D(pl, l[0]), PlanePtTo2D(pl, l[1])};
  }
  static Point3 PlanePtTo3D(Plane pl, Point p) { // ok
    vector<Point3> base = pl.GetOrtonormalBase();
    return base[0] * base[0].DotProd(pl[0]) + base[1] * p.x + base[2] * p.y;
  }
  static Line3 PlaneLineTo3D(Plane pl, Line l) {
    return {PlanePtTo3D(pl, l[0]), PlanePtTo3D(pl, l[1])};
  }
  static Line3 ProjLineToPlane(Line3 l, Plane pl) { // ok
    return {ProjPtToPlane(l[0], pl), ProjPtToPlane(l[1], pl)};
  }
  static LD DisLineLine(Line3 l, Line3 k) { // ok
    Plane together {l[0], l[1], l[0] + k[1] - k[0]}; // parallel FIXME
    Line3 proj = ProjLineToPlane(k, together);
    Point3 inter = (Utils3::InterLineLine(l, proj))[0];
    Point3 on_k_inter = k[0] + inter - proj[0];
    return inter.Dis(on_k_inter);
  }
  static bool PtBelongToLine3(Point3 p, Line3 l) { return DisPtLine3(p, l) < kEps; }
  static bool Line3BelongToPlane(Line3 l, Plane pl) {
    return PtBelongToPlane(l[0], pl) && PtBelongToPlane(l[1], pl);
  }
  static LD Det(Point3 a, Point3 b, Point3 d) { // ok
    Point3 pts[3] = {a, b, d};
    LD res = 0;
    for (int sign : {-1, 1}) {
      REP (st_col, 3) {
        int c = st_col;
        LD prod = 1;
        REP (r, 3) {
          prod *= pts[r][c];
          c = (c + sign + 3) % 3;
        }
        res += sign * prod;
      }
    }
    return res;
  }
  static LD Area(Point3 p, Point3 q, Point3 r) { q -= p; r -= p; return q.Area(r); }
  static vector<Point3> InterLineLine(Line3 k, Line3 l) {
    if (Lines3Equal(k, l)) { return {k[0], k[1]}; }
    if (PtBelongToLine3(l[0], k)) { return {l[0]}; }
    Plane pl{l[0], k[0], k[1]};
    if (!PtBelongToPlane(l[1], pl)) { return {}; }
    Line k2 = PlaneLineTo2D(pl, k); Line l2 = PlaneLineTo2D(pl, l);
    vector<Point> inter = Utils::InterLineLine(k2, l2);
    vector<Point3> res;
    for (auto P : inter) { res.PB(PlanePtTo3D(pl, P)); }
    return res;
  }
  static Plane ParallelPlane(Plane pl, Point3 A) { // plane parallel to pl going through A
    Point3 diff = A - ProjPtToPlane(A, pl);
    return {pl[0] + diff, pl[1] + diff, pl[2] + diff};
  }
  // image of B in rotation wrt line passing through origin s.t. A1->A2 
  // implemented in more general case with similarity instead of rotation
  static Point3 RotateAccordingly(Point3 A1, Point3 A2, Point3 B1) { // ok
    Plane pl{A1, A2, {0, 0, 0}};
    Point A12 = PlanePtTo2D(pl, A1); Point A22 = PlanePtTo2D(pl, A2);
    complex<LD> rat = complex<LD>(A22.x, A22.y) / complex<LD>(A12.x, A12.y);
    Plane plb = ParallelPlane(pl, B1); Point B2 = PlanePtTo2D(plb, B1);
    complex<LD> Brot = rat * complex<LD>(B2.x, B2.y);
    return PlanePtTo3D(plb, {Brot.real(), Brot.imag()});
  }
  
  static vector<Circle3> InterSpherePlane(Sphere s, Plane pl) { // ok
    Point3 proj = ProjPtToPlane(s.o, pl);
    LD dis = s.o.Dis(proj);
    if (dis > s.r + kEps) { return {}; }
    if (dis > s.r - kEps) { return {{pl, proj, 0}}; } // is it best choice? 
    return {{pl, proj, sqrt(s.r * s.r - dis * dis)}};
  }
  static bool PtBelongToSphere(Sphere s, Point3 p) {
    return abs(s.r - s.o.Dis(p)) < kEps;
  }
};
struct PointS { // just for conversion purposes, probably toEucl suffices
  LD lat, lon;
  Point3 toEucl() {
    return Point3{cos(lat) * cos(lon), cos(lat) * sin(lon), sin(lat)};
  }
  PointS(Point3 p) {
    p.NormalizeSelf(); lat = asin(p.z); lon = acos(p.y / cos(lat));
  }
};
LD DistS(Point3 a, Point3 b) {
  return atan2l(b.CrossProd(a).Norm(), a.DotProd(b));
}
struct CircleS {
  Point3 o; // center of circle on sphere
  LD r; // arc len
  LD area() const { return 2 * kPi * (1 - cos(r)); }
};

CircleS From3(Point3 a, Point3 b, Point3 c) { // any three different points
  int tmp = 1;
  if ((a - b).Norm() > (c - b).Norm()) { swap(a, c); tmp = -tmp; }
  if ((b - c).Norm() > (a - c).Norm()) { swap(a, b); tmp = -tmp; }
  Point3 v = (c - b).CrossProd(b - a); v = v * (tmp / v.Norm());
  return CircleS{v, DistS(a,v)};
}

CircleS From2(Point3 a, Point3 b) { // neither the same nor the opposite
  Point3 mid = (a + b) / 2; mid = mid / mid.Norm(); return From3(a, mid, b);
}

LD SphAngle(Point3 A, Point3 B, Point3 C) { //angle at A, no two points opposite
  LD a = B.DotProd(C); LD b = C.DotProd(A); LD c = A.DotProd(A);
  return acos((b - a * c) / sqrt((1 - Sq(a)) * (1 - Sq(c))));
}

LD TriangleArea(Point3 A, Point3 B, Point3 C) { // no two poins opposite
  LD a = SphAngle(C, A, B); LD b = SphAngle(A, B, C); LD c = SphAngle(B, C, A);
  return a + b + c - kPi;
}
vector<Point3> IntersectionS(CircleS c1, CircleS c2) {
  Point3 n = c2.o.CrossProd(c1.o), w = c2.o * cos(c1.r) - c1.o * cos(c2.r);
  LD d = n.SqNorm();
  if (d < kEps) { return {}; } // parallel circles (can fully overlap)
  LD a = w.SqNorm() / d;
  vector<Point3> res;
  if (a >= 1 + kEps) { return res; }
  Point3 u = n.CrossProd(w) / d;
  if (a > 1 - kEps) { res.PB(u); return res; }
  LD h = sqrt((1 - a) / d);
  res.PB(u + n * h); res.PB(u - n * h);
  return res;
}
bool Eq(LD a, LD b) { return abs(a - b) < kEps; }
vector<Point3> intersect(Sphere a, Sphere b, Sphere c) { // Does not work for 3 colinear centers
    vector<Point3> res;
    Point3 ex, ey, ez;
    LD r1 = a.r, r2 = b.r, r3 = c.r, d, cnd_x = 0, i, j;
    ex = (b.o - a.o).Normalize();
    i = ex.DotProd(c.o - a.o);
    ey = ((c.o - a.o) - ex * i).Normalize();
    ez = ex.CrossProd(ey);
    d = (b.o - a.o).Norm();
    j = ey.DotProd(c.o - a.o);

    bool cnd = 0;
    if (Eq(r2, d - r1)) { cnd_x = +r1; cnd = 1; }
    if (Eq(r2, d + r1)) { cnd_x = -r1; cnd = 1; }

    if (!cnd && (r2 < d - r1 || r2 > d + r1)) return res;
    if (cnd) {
        if (Eq(Sq(r3), (Sq(cnd_x - i) + Sq(j)))) res.PB(Point3(cnd_x, 0.0, 0.0));
    } else {
      LD x = (Sq(r1) - Sq(r2) + Sq(d)) / (2 * d);
      LD y = (Sq(r1) - Sq(r3) + Sq(i) + Sq(j)) / (2 * j) - (i / j) * x;
      LD u = Sq(r1) - Sq(x) - Sq(y);
      if (u >= -kEps) {
        LD z = sqrtl(max(LD(0), u)); res.PB(Pt(x, y, z));
        if (!isZero(z)) res.PB(Pt(x, y, -z));
      }
    }
    for (auto& it : res) { it = a.o + ex * it[0] + ey * it[1] + ez * it[2]; }
    return res;
}
