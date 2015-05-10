#include<stdint.h>
struct bigNum{
    typedef uint32_t mult_t;
    typedef uint16_t base_t;
    static const base_t BASE = 10000;
    static const int B_DIGS = 4;
    static const char prFirst[];
    static const char prNext[];
    int len, cap;
    base_t* digs;
    private:
    bigNum(): len(0), cap(0), digs(NULL) {}
    public:
    explicit bigNum(base_t n) {
        len = cap = 2;
        digs = new base_t[cap];
        digs[0] = n % BASE;
        digs[1] = n / BASE;
        clen();
    }
    void init(int l, base_t* const d, int c = 0) {
        len = l;
        cap = c;
        if (!c) cap = max(len,1);
        digs = new base_t[cap];
        memcpy(digs, d, len * sizeof(base_t));
        memset(digs + len, 0, (cap - len) * sizeof(base_t));
        clen();
    }
    void extend(int n_cap) {
        if (cap >= n_cap) return;
        base_t* n_digs = new base_t[n_cap];
        memcpy(n_digs, digs, len * sizeof(base_t));
        memset(n_digs + len, 0, (n_cap - len) * sizeof(base_t));
        if(digs) delete[] digs;
        swap(digs, n_digs);
        cap = n_cap;
    }
    void clen() {
        while (len > 0 && !digs[len - 1]) --len;
    }
    bigNum(int l, base_t* const d, int c = 0) {
        init(l, d, c);
    }
    explicit bigNum(const char* s) {
        int n = strlen(s);
        cap = len = (n + B_DIGS - 1) / B_DIGS;
        digs = new base_t[len];
        int pos = 0;
        FORD(i, len-1, 0) {
            digs[i] = 0;
            for (; pos < n - (i) * B_DIGS && pos < n; ++pos) {
                digs[i] = 10 * digs[i] + s[pos] - '0';
            }
        }
        clen();
    }
    bigNum(bigNum const & b) {
        init(b.len, b.digs);
    }
    ~bigNum() {
        if (digs) delete[] digs;
    }
    bigNum& operator= (const bigNum & b) {
        delete[] digs;
        init(b.len, b.digs);
        return *this;
    }
    bool operator == (bigNum const& b) const {
        if (len != b.len) return false;
        REP(i, len) {
            if (digs[i] != b.digs[i]) return false;
        }
        return true;
    }
    bool operator != (bigNum const& b) const {
        return ! (operator==(b));
    }
    short compare(bigNum const& b) const {
        if (len < b.len) return -1;
        if (len > b.len) return 1;
        FORD(i, len - 1, 0) {
            if (digs[i] < b.digs[i]) return -1;
            if (digs[i] > b.digs[i]) return 1;
        }
        return 0;
    }
    bool operator < (bigNum const &b) const {
        return compare(b) < 0;
    }
    bool operator > (bigNum const &b) const {
        return compare(b) > 0;
    }
    bool operator <= (bigNum const &b) const {
        return compare(b) <= 0;
    }
    bool operator >= (bigNum const &b) const {
        return compare(b) >= 0;
    }
    bigNum const& operator += (base_t n) {
        return operator+=(bigNum(n));
    }
    bigNum const & operator+= (bigNum const& b) {
        extend(max(len, b.len)+1);
        len = max(len, b.len)+1;
        digs[0] += b.digs[0];
        int i;
        for (i = 1; i < len; ++i) {
            if (digs[i-1] >= BASE) {
                digs[i-1] -= BASE;
                ++digs[i];
            } else if (i >= b.len) break;
            if (i<b.len) digs[i] += b.digs[i];
        }
        clen();
        return *this;
    }
    bigNum const operator+(bigNum const& b) const {
        bigNum res(len, digs, max(len, b.len) + 1);
        res += b;
        return res;
    } 
    bigNum const& operator -= (bigNum const& b) {
        base_t rem = 0;
        REP(i,len) {
            if (i < b.len) rem += b.digs[i];
            if (rem > digs[i]) {
                digs[i] -= (rem-BASE);
                rem = 1;
            }
            else {
                digs[i] -= rem;
                rem = 0;
            }
            if (rem == 0 && i >= b.len - 1) break;
        }
        assert(rem == 0);
        clen();
        return *this;
    }
    bigNum const& operator-= (base_t n) {
        return operator-=(bigNum(n));
    }
    bigNum const operator- (bigNum const& b) const {
        bigNum res(len, digs);
        res -= b;
        return res;
    } 
    bigNum const& operator *= (base_t  n) {
        if (n >= BASE) return operator *= (bigNum(n));
        extend(len+1);
        ++len;
        base_t p = 0;
        REP(i, len) {
            mult_t m = (mult_t) digs[i]*n + p;
            digs[i] = m % BASE;
            p = m / BASE;
        }
        clen();
        return *this;
    }
    bigNum const operator << (int sh) const {
        bigNum res;
        res.extend(len + sh);
        res.len = len + sh;
        memcpy(res.digs + sh, digs, len * sizeof(base_t));
        return res;
    }
    bigNum const multSh(base_t n, int sh) const {
        assert(n < BASE);
        bigNum res;
        res.extend(len + sh + 1);
        res.len =  len + 1 + sh;
        base_t p = 0;
        FOR(i, sh, sh + len - 1) {
            mult_t m = (mult_t) digs[i-sh]*n + p;
            res.digs[i] = m % BASE;
            p = m / BASE;
        }
        res.digs[len+sh] = p;
        res.clen();
        return res;
    }
    bigNum const operator * (bigNum const & b) const {
        bigNum res;
        res.extend(len + b.len);
        REP(i, len) {
            base_t p = 0;
            REP(j, b.len) {
                mult_t m = (mult_t)digs[i] * b.digs[j] + p + res.digs[i+j];
                res.digs[i+j] = m%BASE;
                p = m/BASE;
            }
            int s = i + b.len;
            while (p>0) {
                res.digs[s] += p;
                if (res.digs[s] >= BASE) {
                    res.digs[s] -= BASE;
                    p = 1;
                } else p = 0;
            }
        }
        res.len = len + b.len;
        res.clen();
        return res;
    }
    bigNum const & operator *= (bigNum const & b) {
        return *this = *this * b;
    } 
    base_t operator % (base_t m) const {
        base_t res = 0;
        FORD(i, len-1, 0) {
            res = ((mult_t)BASE*res + digs[i]) % m;
        }
        return res;
    }
    pair<bigNum, bigNum> const div(bigNum const & b) const {
        bigNum d;
        int dlen = max(len - b.len + 1, 0);
        d.extend(dlen);
        bigNum rem(*this);
        FORD(i, dlen-1, 0) {
            base_t l = 0, r = BASE-1;
            while (l < r) {
                base_t m = (l + r + 1) / 2;
                if (rem < b.multSh(m, i)) {
                    r = m - 1;
                } else {
                    l = m;
                }
            }
            rem -= b.multSh(l,i);
            if (l > 0) d.digs[i] = l;
        }
        d.len = dlen;
        d.clen();
        return MP(d, rem);
    }
    bigNum const sqrt() const {
        bigNum res;
        int n = (len + 1) / 2;
        res.extend(n);
        bigNum rem(*this);
        FORD(i, n, 0) {
            base_t l = 0, r = BASE - 1;
            while ( l <r) {
                base_t m = (l+r+1)/2;
                bigNum b(res);
                b *= 2;
                b += (bigNum(m) << i);
                if (rem < b.multSh(m,i)) {
                    r = m - 1;
                } else l = m;
            }
            bigNum ls = bigNum(l).multSh(1,i);
            bigNum b(res);
            b *= 2;
            b += ls;
            rem -= b.multSh(l,i);
            res += ls;
            if (l != 0 && !res.len) res.len = i + 1;
        }
        return res;
    }

    bigNum const operator/ (bigNum const & b) const {
        return div(b).X;
    }
    bigNum const operator% (bigNum const & b) const {
        return div(b).Y;
    }
    bigNum const & operator /= (bigNum const &b) {
        return *this = *this / b;
    }
    bigNum const & operator %= (bigNum const &b) {
        return *this = *this % b;
    }
    bigNum const operator /= (base_t n) {
        if (n >= BASE) return *this/bigNum(n);
        mult_t p = 0;
        FORD(i, len-1, 0) {
            p = BASE*p + digs[i];
            digs[i] = p / n;
            p %= n;
        }
        clen();
        return *this;
    }
    friend std::ostream& operator<< (std::ostream& str, bigNum const &n) {
        if (n.len == 0) str << 0;
        else {
            cout << n.digs[n.len-1];
            FORD(i, n.len-2, 0) str << setw(4) << setfill('0') << n.digs[i];
        }
        return str;
    }
    friend std::istream& operator>> (std::istream& str, bigNum &n) {
        string s;
        str >> s;
        n = bigNum(s.c_str());
        return str;
        
    }
    void write() const {
        if (len == 0) printf("0");
        else {
            printf(prFirst,digs[len-1]);
            FORD(i, len-2, 0) printf(prNext,digs[i]);
        }
    }

    void writeln() const {
        write(); putchar('\n');
    }
};

const char bigNum::prFirst[] = "%u";
const char bigNum::prNext[] = "%.4u";
char buf[11000000];
int main() {
    scanf("%s", buf);
    bigNum n(buf);
    n.sqrt().writeln();
    return 0;
}
