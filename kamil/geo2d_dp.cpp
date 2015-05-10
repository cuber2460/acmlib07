#include <bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld kEps = 1e-9;
const ld kPi = 2 * acos(0);
struct P {
  ld x, y;
  P(ld a = 0, ld b = 0) : x(a), y(b) {}
  P(const P& a) : x(a.x), y(a.y) {}
  void operator=(const P& a) { x = a.x; y = a.y; }
  P operator+(const P& a) const { return P(x+a.x, y+a.y); }
  P operator-(const P& a) const { return P(x-a.x, y-a.y); }
  P operator*(ld a) const { P p(x * a, y * a); return p; }
  P operator/(ld a) const { P p(x / a, y / a); return p; }
  P& operator+=(const P& a) { x+=a.x; y+=a.y; return *this; }
  P& operator-=(const P& a) { x-=a.x; y-=a.y; return *this; }
  P& operator*=(ld a) { x *= a; y *= a; return *this;}
  P& operator/=(ld a) { x /= a; y /= a; return *this; }
  
  bool IsZero() const { return fabs(x) < kEps && fabs(y) < kEps; }
  bool operator==(const P& a) const { return (*this - a).IsZero(); }
  ld CrossProd(const P& a) const { return x * a.y - y * a.x; }
  ld CrossProd(P a, P b) const {
    a -= *this;
    b -= *this;
    return a.CrossProd(b);
  }
  ld DotProd(const P& a) const { return x * a.x + y * a.y; } // ABcos
  ld Norm() const { return sqrt(x * x + y * y); }
  void NormalizeSelf() { *this /= Norm(); }
  P Normalize() {
    P res(*this);
    res.NormalizeSelf();
    return res;
  }
  ld Dist(const P& a) const { return (*this - a).Norm(); }
  ld Angle() const { return atan2(y, x); }
  void RotateSelf(ld angle) {
    ld c = cos(angle);
    ld s = sin(angle);
    ld nx = x * c - y * s;
    ld ny = y * c + x * s;
    y = ny;
    x = nx;
  }
  P Rotate(ld angle) const {
    P res(*this);
    res.RotateSelf(angle);
    return res;
  }
  static bool LexCmp(const P& a, const P& b) {
    if (fabs(a.x - b.x) > kEps) return a.x < b.x;
    return a.y < b.y;
  }
};

struct Circle {
  P center; ld r;
  Circle(ld x, ld y, ld rad) {
    center = P(x, y);
    r = rad;
  }
  Circle(const P& a, ld rad) : center(a), r(rad) {}
  ld Area() const { return kPi * r * r; }
  ld Perimeter() const { return 2 * kPi * r; }
  ld Diameter() const { return 2 * r; }
  P RotateRightMost(ld ang) const {
    P right_most(r, 0);
    P res = right_most.Rotate(ang);
    return center + res;
  }
  bool operator==(const Circle& c) const {
    return center == c.center && fabs(r - c.r) < kEps;
  }
};

struct Line {
  P p[2]; bool is_seg;
  Line(P a, P b, bool is_seg_ = false) {
    p[0] = a;
    p[1] = b;
    is_seg = is_seg_;
  }
  Line() { }
  P& operator[](int a) { return p[a]; }
  static bool VectorsInOneHalfPlaneCmp(Line a, Line b) {
    return (a[1] - a[0]).CrossProd(b[1] - b[0]) > 0;
  }
  P NormalVector() {
    P perp = p[1] - p[0];
    perp.RotateSelf(kPi / 2);
    perp.NormalizeSelf();
    return perp;
  }
  vector<ld> LineEqNormld() { // seems ok
    ld A = p[1].y - p[0].y;
    ld B = p[0].x - p[1].x;
    ld C = -(A * p[0].x + B * p[0].y);
    assert(fabs(A * p[1].x + B * p[1].y + C) < kEps);
    ld norm = sqrt(A * A + B * B);
    vector<ld> res{A, B, C};
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
  /*ld distPoint(P C) {
     ld dist = ((B-A)*(C-A)) / sqrt((B-A)*(B-A));
     if(is_seg){
		if(dot(C-B, B-A) > 0) return dist(B,C);
		if(dot(C-A, A-B) > 0) return dist(A,C);
	}
	return abs(dist);
  }*/
};

struct Utils {
  static vector<P> InterCircleCircle(Circle& a, Circle& b) {
    Circle* c = &a;
    Circle* d = &b;
    if (a.r + kEps < b.r) swap(c, d);
    if (a == b) {
      return vector<P>{a.RotateRightMost(0), a.RotateRightMost(2 * kPi / 3),
          a.RotateRightMost(4 * kPi / 3)};
    }
    P diff = d->center - c->center;
    ld dis = diff.Norm();
    ld ang = diff.Angle();
    ld longest = max(max(c->r, d->r), dis);
    if (2 * longest > c->r + d->r + dis + kEps) {
      return vector<P>();
    }
    if (fabs(2 * longest - c->r - d->r - dis) < 2 * kEps) {
      return vector<P>{c->RotateRightMost(ang)};
    }
    ld proj_diff = (c->r * c->r - d->r * d->r) / dis;
    ld r1_proj = (dis + proj_diff) / 2;
    ld ang_deviation = acos(r1_proj / c->r);
    ld ang1 = ang - ang_deviation;
    ld ang2 = ang + ang_deviation;
    return vector<P>{c->RotateRightMost(ang1), c->RotateRightMost(ang2)};
  }
  static P InterLineLine(Line& a, Line& b) { // working fine
    P vec_a = a[1] - a[0];
    P vec_b1 = b[1] - a[0];
    P vec_b0 = b[0] - a[0]; 
    ld tr_area = vec_b1.CrossProd(vec_b0);
    ld quad_area = vec_b1.CrossProd(vec_a) + vec_a.CrossProd(vec_b0);
//     if (abs(quad_area) < kEps) {
//       throw Parallel();
//     }
    return P(a[0] + vec_a * (tr_area / quad_area));
  }
    
  static P ProjPToLine(P p, Line l) { ///Tested
    l[0] -= p;
    l[1] -= p;
    P normal = l.NormalVector();
    normal *= normal.DotProd(l[0]);
    return normal + p;
  }
  
  static P ReflectPtWRTLine(P p, Line l) {
    P proj = ProjPToLine(p, l);
    return proj * 2 - p;
  }
  
  static vector<P> InterCircleLine(Circle c, Line l) {
    P proj = ProjPToLine(c.center, l);
    ld dis_proj = c.center.Dist(proj);
    if (dis_proj > c.r + kEps) { return vector<P>(); }
    ld a = sqrt(c.r * c.r - dis_proj * dis_proj);
    P dir = l[1] - l[0];
    dir.NormalizeSelf();
    vector<P> cands{proj + dir * a, proj - dir * a};
    if (cands[0].Dist(cands[1]) < kEps) { return vector<P>{proj}; }
    return cands;
  }
  
  static bool PtBelongToLine(Line l, P p) {
    return fabs(l[0].CrossProd(l[1], p)) < kEps;
  }
  
  static bool PtBelongToSeg(Line l, P p) { // NIEDOKLADNOSC!
    return fabs(p.Dist(l[0]) + p.Dist(l[1]) - l[0].Dist(l[1])) < kEps;
  }
  
  static vector<P> InterCircleSeg(Circle c, Line l) { //seems ok
    vector<P> from_line = InterCircleLine(c, l);
    vector<P> res;
    for (auto p : from_line) {
      if (PtBelongToSeg(l, p)) { res.push_back(p); }
    }
    return res;
  }
  
  static vector<P> TangencyPtsToCircle(Circle c, P p) { // seems ok
    assert(c.center.Dist(p) > c.r + kEps); // or return vector<P>() if needed
    P mid = (c.center + p) / 2;
    Circle aux_circ = Circle(mid, c.center.Dist(p) / 2);
    return InterCircleCircle(aux_circ, c);
  }
  
  static bool AreParallel(Line l1, Line l2) { // seems ok
    return fabs(l1[0].CrossProd(l2[0], l1[1]) - l1[0].CrossProd(l2[1], l1[1])) < kEps;
  }
  
  static vector<P> InterSegs(Line l1, Line l2) { // seems ok
    if (!P::LexCmp(l1[0], l1[1])) { swap(l1[0], l1[1]); }
    if (!P::LexCmp(l2[0], l2[1])) { swap(l2[0], l2[1]); }
    if (AreParallel(l1, l2)) {
      if (!PtBelongToLine(l1, l2[0])) { return vector<P>(); }
      vector<P> ends(2);
      for (int tr = 0; tr < 2; tr++) {
        if (P::LexCmp(l1[tr], l2[tr]) ^ tr) {
          ends[tr] = l2[tr];
        } else {
          ends[tr] = l1[tr];
        }
      }
      if ((ends[1] - ends[0]).IsZero()) {
        ends.pop_back();
      }
      if ((int)ends.size() == 2 && P::LexCmp(ends[1], ends[0])) { return vector<P>(); }
      return ends;
    } else {
      P p = InterLineLine(l1, l2);
      if (PtBelongToSeg(l1, p) && PtBelongToSeg(l2, p)) { return vector<P>{p}; }
      return vector<P>();
    }
  }
  static ld Angle(P A, P Q, P R) { // angle PQR
    ld ang2 = (A - Q).Angle();
    ld ang1 = (R - Q).Angle();
    ld ans = ang1 - ang2;
    if (ans < kEps) ans += 2 * kPi;
    return ans;
  }
  static ld DiskInterArea(Circle c1, Circle c2) {
    if (c1.r < c2.r) swap(c1,c2);
    ld dist = c1.center.Dist(c2.center);
    if (c1.r + c2.r < dist + kEps) return 0;
    if (c1.r - c2.r > dist - kEps)
      return kPi * c2.r * c2.r;
    vector<P> inters = Utils::InterCircleCircle(c1, c2);
    ld cross = (inters[0] - c1.center).CrossProd(inters[1] - c1.center);
    if (cross < kEps)
      swap(inters[0], inters[1]);
    ld res = 0;
    ld ang1 = Angle(inters[0], c1.center, inters[1]);
    res += ang1 * c1.r * c1.r;
    ld ang2 = Angle(inters[1], c2.center, inters[0]);
    res += ang2 * c2.r * c2.r;
    res -= (inters[0] - c1.center).CrossProd(inters[1] - c1.center) + (inters[1] - c2.center).CrossProd(inters[0] - c2.center);
    return res / 2;
  }
};
int main() { return 0; }
/* sin(a+b) = sin(a)*cos(b) + cos(a)*sin(b)
 * cos(a+b) = c*c-s*s
 * sin(a)+sin(b) = 2sin((a+b)/2)cos((a-b)/2) */

/* 1) dp[i][j] = min{dp[i-1][k] + C[k][j]}
 * 2) min_k dla (i,j) <= min_k dla (i,j+1)
 * rec(i, low, high, min_kLOW, min_kHIGH){ med_j = (low+high)/2; brut;}*/

// Knuth: F[a][b]=min{F[a][c-1]+F[c+1][b]+Cost} for c in opt[a][b-1]-opt[a+1][b]
// enough: C[a][c]+C[b][d]<=C[a][d]+C[b][c] && C[b][c] <= C[a][d]
