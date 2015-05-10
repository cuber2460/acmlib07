#include <bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld kPi = 2 * acos(0);
const ld kEps = 1e-9;
struct P {
  ld x, y, z;
  P(ld a = 0, ld b = 0, ld c = 0) : x(a), y(b), z(c) {}
  P(const P& a) : x(a.x), y(a.y), z(a.z) {}
  void operator=(const P& a) { x = a.x; y = a.y; z = a.z; }
  P operator+(const P& a) const { P p(x + a.x, y + a.y, z + a.z); return p; }
  P operator-(const P& a) const { P p(x - a.x, y - a.y, z - a.z); return p; }
  P operator*(ld a) const { P p(x * a, y * a, z * a); return p; }
  P operator/(ld a) const { assert(a > kEps); P p(x / a, y / a, z / a); return p; }
  P& operator+=(const P& a) { x += a.x; y += a.y; z += a.z; return *this; }
  P& operator-=(const P& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
  P& operator*=(ld a) { x *= a; y *= a; z *= a; return *this;}
  P& operator/=(ld a) { assert(a > kEps); x /= a; y /= a; z /= a; return *this; }
  
  ld& operator[](int a) {
    if(a == 0) return x;
    if(a == 1) return y;
    if(a == 2) return z;
    assert(false);
  }
  bool IsZero() const {
    return fabs(x) < kEps && fabs(y) < kEps && fabs(z) < kEps;
  }
  bool operator==(const P& a) const {
    return (*this - a).IsZero();
  }
  ld DotProd(const P& a) const {
    return x * a.x + y * a.y + z * a.z;
  }
  ld Norm() const { return sqrt(x*x+y*y+z*z); }
  void NormalizeSelf() { *this /= Norm(); }
  P Normalize() {
    P res(*this);
    res.NormalizeSelf();
    return res;
  }
  ld Dis(const P& a) const { return (*this - a).Norm(); }
  pair<ld, ld> SphericalAngles() const {
    return {atan2(z, sqrt(x * x + y * y)), atan2(y, x)};
  }
  ld Area(const P& p) const {
    return Norm() * p.Norm() * sin(Angle(p)) / 2;
  }
  ld Angle(const P& p) const {
    ld a = Norm();
    ld b = p.Norm();
    ld c = Dis(p);
    return acos((a * a + b * b - c * c) / (2 * a * b));
  }
  static ld Angle(P& p, P& q) { return p.Angle(q); }
  P CrossProd(P p) {
    P q(*this);
    return {q[1] * p[2] - q[2] * p[1], q[2] * p[0] - q[0] * p[2], q[0] * p[1] - q[1] * p[0]};
  }
  static bool LexCmp(const P& a, const P& b) {
    if (fabs(a.x - b.x) > kEps) return a.x < b.x;
    if (fabs(a.y - b.y) > kEps) return a.y < b.y;
    return a.z < b.z;
  }
};

struct Line {
  P p[2]; bool is_seg;
  Line(P a, P b, bool is_seg_ = false) {
    p[0] = a;
    p[1] = b;
    is_seg = is_seg_;
  }
  Line() {}
  P& operator[](int a) { return p[a]; }
};

struct Plane {
  P p[3];
  Plane(P a, P b, P c) {
    p[0] = a;
    p[1] = b;
    p[2] = c;
  }
  P& operator[](int a) { return p[a]; }
  P GetNormal() {
    P cross = (p[1] - p[0]).CrossProd(p[2] - p[0]);
    return cross.Normalize();
  }
  void GetPlaneEq(ld& A, ld& B, ld& C, ld& D) {
    P normal = GetNormal();
    A = normal[0] ;
    B = normal[1];
    C = normal[2];
    
    D = normal.DotProd(p[0]);
    assert(abs(D - normal.DotProd(p[1])) < kEps);
    assert(abs(D - normal.DotProd(p[2])) < kEps);
  }
};

struct Utils {
  static P ProjPtToLine(P p, Line l) { // ok
    P diff = l[1] - l[0];
    diff.NormalizeSelf();
    return l[0] + diff * (p - l[0]).DotProd(diff);
  }
  static ld DisPtLine(P p, Line l) { // ok
//    ld area = Area(p, l[0], l[1]);
//     ld dis1 = 2 * area / l[0].Dis(l[1]);
    ld dis2 = p.Dis(ProjPtToLine(p, l));
//     assert(abs(dis1 - dis2) < kEps);
    return dis2;
  }
  static ld DisPtPlane(P p, Plane pl) {
    P normal = pl.GetNormal();
    return abs(normal.DotProd(p - pl[0]));
  }
  static P ProjPtToPlane(P p, Plane pl) {
    P normal = pl.GetNormal();
    return p - normal * normal.DotProd(p - pl[0]);
  }
  static bool PtBelongToPlane(P p, Plane pl) {
    return DisPtPlane(p, pl) < kEps;
  }
  static bool PtBelongToLine(P& p, Line& l) {
    return DisPtLine(p, l) < kEps;
  }
  static ld Det(P a, P b, P d) { // ok
    P pts[3] = {a, b, d};
    ld res = 0;
    for (int sign : {-1, 1}) {
	  for(int st_col=0;st_col<3;++st_col) {
        int c = st_col;
        ld prod = 1;
        for(int r=0;r<3;++r) {
          prod *= pts[r][c];
          c = (c + sign + 3) % 3;
        }
        res += sign * prod;
      }
    }
    return res;
  }
  static ld Area(P p, P q, P r) { // ok
    q -= p;
    r -= p;
    return q.Area(r);
  }  
};
int main() { return 0; }
/*  double x = sin(lng/180*PI)*cos(lat/180*PI)*alt;
    double y = cos(lng/180*PI)*cos(lat/180*PI)*alt;
    double z = sin(lat/180*PI)*alt; */
