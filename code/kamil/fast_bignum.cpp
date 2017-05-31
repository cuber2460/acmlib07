#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;

// base and base_digits must be consistent
const int base = 1000000000;
const int base_digits = 9;

typedef long long ll;

struct Big {
    vector<int> w;
    int sign; // 1 or -1

	int & operator [](int i) { return w[i]; }
	const int & operator [](int i) const { return w[i]; }
	
	int size() const { return w.size(); }
	void push_back(int x) { w.push_back(x); }
	
    Big() : sign(1) {}
    Big(ll v) { *this = v; }
    Big(const string &s) { read(s); }
    //~ void operator=(const Big &he) { sign=he.sign; w=he.w; }
    
    bool iszero() const { return w.empty(); }

    void operator=(ll v) {
        sign = v >= 0 ? 1 : -1;
        w.clear();
        for(v = std::abs(v); v; v /= base)
			push_back(v % base);
    }
    
    private : Big add_sub(const Big & he, int type) {
		for (int i = 0, carry = 0; i < he.size() || carry; ++i) {
			if (i == size()) push_back(0);
			w[i] += type * (carry + (i < he.size() ? he[i] : 0));
			if((carry = w[i] >= base)) w[i] -= base; // TODO: sth wrong with compilator? warning without extra ()
			else if((carry = w[i] < 0)) w[i] += base;
		}
		trim();
		return *this;
	} public :
    Big operator+(const Big &he) const {
		if(he.iszero()) return *this;
		if(sign != he.sign) return *this - (-he);
		if(abs() < he.abs()) return he + *this;
		Big res = *this;
		return res.add_sub(he, 1);
	}
    Big operator-(const Big &he) const {
		if(sign != he.sign) return *this + (-he);
		if (abs() < he.abs()) return -(he - *this);
		Big res = *this;
		return res.add_sub(he, -1);
	}

    void operator*=(int v) {
        if (v < 0) sign *= -1, v *= -1;
        for (int i = 0, carry = 0; i < size() || carry; ++i) {
            if (i == size()) push_back(0);
            ll cur = (ll) w[i] * v + carry;
            carry = cur / base;
            w[i] = cur % base;
        }
        trim();
    }

	void operator/=(int v) {
        if (v < 0) sign *= -1, v *= -1;
        for (int i = size() - 1, rem = 0; i >= 0; --i) {
            ll cur = w[i] + (ll) rem * base;
            w[i] = cur / v;
            rem = cur % v;
        }
        trim();
    }

    Big operator*(int v) const {
        Big res = *this;
        res *= v;
        return res;
    }

    Big operator/(int v) const {
        Big res = *this;
        res /= v;
        return res;
    }

    int operator%(int v) const {
        if (v < 0) v *= -1; // consider assert(v > 0)
        int m = 0;
        for (int i = size() - 1; i >= 0; --i)
            m = (w[i] + (ll) m * base) % v;
        return m * sign;
    }
    

    friend pair<Big, Big> divmod(const Big &a1, const Big &b1) {
        int norm = base / (b1.w.back() + 1);
        Big a = a1.abs() * norm;
        Big b = b1.abs() * norm;
        Big q, r;
        q.w.resize(a.size());

        for (int i = a.size() - 1; i >= 0; i--) {
            r *= base;
            r += a[i];
            int s1 = b.size() < r.size() ? r[b.size()] : 0;
            int s2 = b.size() - 1 < r.size() ? r[b.size() - 1] : 0;
            int d = ((long long) s1 * base + s2) / b.w.back();
            r -= b * d;
            while (r < 0)
                r += b, --d;
            q[i] = d;
        }

        q.sign = a1.sign * b1.sign;
        r.sign = a1.sign;
        q.trim();
        r.trim();
        return make_pair(q, r / norm);
    }

    friend Big sqrt(const Big &a1) {
        Big a = a1;
        while (a.w.empty() || a.size() % 2 == 1)
            a.push_back(0);

        int n = a.size();
        
        int firstDigit = (int) sqrt((double) a[n - 1] * base + a[n - 2]);
        int norm = base / (firstDigit + 1);
        a *= norm;
        a *= norm;
		while (a.w.empty() || a.size() % 2 == 1)
			a.push_back(0);
        
        Big r = (long long) a[n - 1] * base + a[n - 2];
        firstDigit = (int) sqrt((double) a[n - 1] * base + a[n - 2]);
        int q = firstDigit;
		Big res;
 
        for(int j = n / 2 - 1; j >= 0; j--) {
            for(; ; --q) {
                Big r1 = (r - (res * 2 * base + q) * q) * base * base + (j > 0 ? (long long) a[2 * j - 1] * base + a[2 * j - 2] : 0);
                if (r1 >= 0) {
                    r = r1;
                    break;
                }
            }
            res *= base;
            res += q;

            if (j > 0) {
				int d1 = res.size() + 2 < r.size() ? r[res.size() + 2] : 0;
				int d2 = res.size() + 1 < r.size() ? r[res.size() + 1] : 0;
                int d3 = res.size() < r.size() ? r[res.size()] : 0;
                q = ((long long) d1 * base * base + (long long) d2 * base + d3) / (firstDigit * 2);
            }           
        }
        
        res.trim();
        return res / norm;
    }

    Big operator/(const Big &v) const {
        return divmod(*this, v).first;
    }

    Big operator%(const Big &v) const {
        return divmod(*this, v).second;
    }

    void operator+=(const Big &v) {
        *this = *this + v;
    }
    void operator-=(const Big &v) {
        *this = *this - v;
    }
    void operator*=(const Big &v) {
        *this = *this * v;
    }
    void operator/=(const Big &v) {
        *this = *this / v;
    }

    // comparators
    int comp(const Big & he) const {
		if(sign != he.sign) return sign;
		if(size() != he.size())
			return sign * (size() < he.size() ? -1 : 1);
		for (int i = size() - 1; i >= 0; i--)
            if (w[i] != he[i])
                return sign * (w[i] < he[i] ? -1 : 1);
        return 0;
    }
    bool operator<(const Big &he) const { return comp(he) < 0; }
    bool operator<=(const Big &he) const { return comp(he) <= 0; }
    bool operator>(const Big &he) const { return comp(he) > 0; }
    bool operator>=(const Big &he) const { return comp(he) >= 0; }
    bool operator==(const Big &he) const { return comp(he) == 0; }
    bool operator!=(const Big &he) const { return comp(he) != 0; }


    void trim() {
        while (!w.empty() && w.back() == 0) w.pop_back();
        if (w.empty()) sign = 1;
    }

    Big operator-() const {
        Big res = *this;
        res.sign = -sign;
        return res;
    }

    Big abs() const {
        Big res = *this;
        res.sign = 1;
        return res;
    }

    ll longValue() const {
        ll res = 0;
        for (int i = size() - 1; i >= 0; i--)
            res = res * base + w[i];
        return res * sign;
    }

    void read(const string &s) {
        sign = 1;
        w.clear();
        int pos = 0;
        while (pos < (int) s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-')
                sign = -sign;
            ++pos;
        }
        for (int i = s.size() - 1; i >= pos; i -= base_digits) {
            int x = 0;
            for (int j = max(pos, i - base_digits + 1); j <= i; j++)
                x = x * 10 + s[j] - '0';
            push_back(x);
        }
        trim();
    }

    friend istream& operator>>(istream &stream, Big &v) {
        string s;
        stream >> s;
        v.read(s);
        return stream;
    }

    friend ostream& operator<<(ostream &stream, const Big &v) {
        if (v.sign == -1)
            stream << '-';
        stream << (v.w.empty() ? 0 : v.w.back());
        for (int i = (int) v.size() - 2; i >= 0; --i)
            stream << setw(base_digits) << setfill('0') << v[i];
        return stream;
    }

    static vector<int> convert_base(const vector<int> &a, int old_digits, int new_digits) {
        vector<long long> p(max(old_digits, new_digits) + 1);
        p[0] = 1;
        for (int i = 1; i < (int) p.size(); i++)
            p[i] = p[i - 1] * 10;
        vector<int> res;
        long long cur = 0;
        int cur_digits = 0;
        for (int i = 0; i < (int) a.size(); i++) {
            cur += a[i] * p[cur_digits];
            cur_digits += old_digits;
            while (cur_digits >= new_digits) {
                res.push_back(int(cur % p[new_digits]));
                cur /= p[new_digits];
                cur_digits -= new_digits;
            }
        }
        res.push_back((int) cur);
        while (!res.empty() && res.back() == 0)
            res.pop_back();
        return res;
    }

    typedef vector<long long> vll;
	// TODO: use O((n*m)^c) karatsuba without extending smaller vector
    static vll karatsubaMultiply(const vll &a, const vll &b) {
        int n = a.size();
        vll res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    res[i + j] += a[i] * b[j];
            return res;
        }

        int k = n >> 1;
        vll a1(a.begin(), a.begin() + k);
        vll a2(a.begin() + k, a.end());
        vll b1(b.begin(), b.begin() + k);
        vll b2(b.begin() + k, b.end());

        vll a1b1 = karatsubaMultiply(a1, b1);
        vll a2b2 = karatsubaMultiply(a2, b2);

        for (int i = 0; i < k; i++)
            a2[i] += a1[i];
        for (int i = 0; i < k; i++)
            b2[i] += b1[i];

        vll r = karatsubaMultiply(a2, b2);
        for (int i = 0; i < (int) a1b1.size(); i++)
            r[i] -= a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            r[i] -= a2b2[i];

        for (int i = 0; i < (int) r.size(); i++)
            res[i + k] += r[i];
        for (int i = 0; i < (int) a1b1.size(); i++)
            res[i] += a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++)
            res[i + n] += a2b2[i];
        return res;
    }

    Big operator*(const Big &v) const {
        vector<int> a6 = convert_base(this->w, base_digits, 6);
        vector<int> b6 = convert_base(v.w, base_digits, 6);
        vll a(a6.begin(), a6.end());
        vll b(b6.begin(), b6.end());
        while (a.size() < b.size())
            a.push_back(0);
        while (b.size() < a.size())
            b.push_back(0);
        while (a.size() & (a.size() - 1))
            a.push_back(0), b.push_back(0);
        vll c = karatsubaMultiply(a, b);
        Big res;
        res.sign = sign * v.sign;
        for (int i = 0, carry = 0; i < (int) c.size(); i++) {
            long long cur = c[i] + carry;
            res.push_back((int) (cur % 1000000));
            carry = (int) (cur / 1000000);
        }
        res.w = convert_base(res.w, 6, base_digits);
        res.trim();
        return res;
    }
};

Big random_Big(int n) {
	string s;
	for (int i = 0; i < n; i++) {
		s += rand() % 10 + '0';
	}
	return Big(s);
}

// random tests
int main() {
	for(int i = 0; i < 1000; i++) {
		cout << i << endl;
		int n = rand() % 100 + 1;
		Big a = random_Big(n);
		Big res = sqrt(a);
		Big xx = res * res;
		Big yy = (res + 1) * (res + 1);
 
		if (xx > a || yy <= a) {
			cout << a << " " << res << endl;
			break;
		}

		int m = rand() % n + 1;
		Big b = random_Big(m) + 1;
		res = a / b;
		xx = res * b;
		yy = b * (res + 1);

		if (xx > a || yy <= a) {
			cout << a << " " << b << " " << res << endl;
			break;
		}
	}

    Big a = random_Big(10000);
	Big b = random_Big(2000);
    clock_t start = clock();
    Big c = a / b;
    fprintf(stdout, "time=%.3lfsec\n", 0.001 * (clock() - start));

    Big x = 5;
    x = 6;
    cout << x << endl;
}
