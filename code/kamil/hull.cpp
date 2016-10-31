// hull online
inline TYP operator^(const P& a,const P& b){return real(a)*imag(b)-real(b)*imag(a);}
inline TYP skret(const P& a,const P& b,const P& c){return ((c-a)^(b-a));}

namespace Hull {
P sr;
int cw(const P& l) {
    TYP a = real(l), b = imag(l);
    if (b >= 0) return a >= 0 ? 1: 2;
    else return a <= 0 ? 3 : 4;
}

struct compare {
    bool operator() (const P& l, const P& r) const {
        int a = cw(l - sr), b = cw(r - sr);
        if (a != b) return a < b;
        TYP d = skret(l, sr, r);
        return d != 0 ? d > 0 : abs(l - sr) < abs(r - sr);
    }
};

struct comp {
    bool operator() (const P& a, const P& b){
        if(real(a) != real(b)) return real(a)<real(b);
        if(imag(a) != imag(b)) return imag(a)<imag(b);
        return false;
    }
};

set<P, compare> hull;
set<P, comp> ph;
typedef set<P, compare>::iterator HI;

void init_hull(vector<P> t) {
    sr = t[0] + t[1] + t[2];
    sr = P(real(sr) / 3, imag(sr) / 3);
    FORE(it, t) hull.insert(*it);
}

HI prev(const HI& i) {
    HI j = i;
    if (i == hull.begin()) j = hull.end();
    return --j;
}

HI next(const HI& i) {
    HI j = i; ++j;
    return j == hull.end() ? hull.begin() : j;
}

void hull_add(P p) { // Multiply by 3 to get good average inside point
    p *= 3;
    if (siz(hull) == 0) {
        if (siz(ph) >= 2 && skret(*ph.begin(), *ph.rbegin(), p) != 0) {
            vector<P> t;
            t.PB(*ph.begin());
            t.PB(*ph.rbegin());
            t.PB(p);
            init_hull(t);
            ph.clear();
        } else ph.insert(p);
        return ;
    }
    HI i = hull.lower_bound(p), j, k;
    if (i == hull.end()) i = hull.begin();
    j = prev(i);
    if (skret(*i, *j, p) >= 0) return;
    while (siz(hull) >= 3) {
        j = prev(i); k = prev(j); 
        if (skret(*k, *j, p) >= 0 && skret(*i, *j, p) <= 0) {
            hull.erase(j); continue;
        }
        k = next(i);
        if (skret(*j, *i, p) >= 0 && skret(*k, *i, p) <= 0) {
            hull.erase(i); i = k; continue;
        }
        break;
    }
    hull.insert(p);
}
vector<P> get_hull() { return vector<P>(ALL(hull)); }
}
