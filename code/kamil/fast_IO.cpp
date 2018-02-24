// uncomment all lines if you need negative numbers
ll fast_read() {
	#define CU c = getchar_unlocked()
	ll x = 0;
	char c;
	while(isspace(CU));;;
	//~ bool is_neg = c == '-';
	//~ if(is_neg) CU;
	while(isdigit(c)) {
		x = 10 * x + c - '0';
		CU;
	}
	//~ if(is_neg) x *= -1;
	return x;
	#undef CU
}
void fast_print(ll x, char after = '\n') {
	static char buf[53];
	int i = 50;
	buf[i+1] = after;
	//~ bool is_neg = x < 0;
	//~ x = abs(x);
	while(x || i == 50) {
		buf[i--] = '0' + x % 10;
		x /= 10;
	}
	//~ if(is_neg) buf[i--] = '-';
	fputs_unlocked(buf + i + 1, stdout);
}
