// uncomment all lines if you need negative numbers
ll fast_read() { // assumes no white spaces before a number
	ll x = 0; // so you must use scanf("%d ", &n) with this
	//~ bool is_neg = false;
	while(true) {
		char c = getchar_unlocked();
		if(isdigit(c)) x = 10 * x + c - '0';
		//~ else if(c == '-') is_neg = true;
		else break;
	}
	//~ if(is_neg) x *= -1;
	return x;
}
void fast_print(ll x) { // or (ll x, char AFTER)
	static char buf[53];
	int i = 50;
	buf[i+1] = '\n'; // or ' ' or AFTER
	//~ bool is_neg = x < 0;
	//~ x = abs(x);
	while(x || i == 50) {
		buf[i--] = '0' + x % 10;
		x /= 10;
	}
	//~ if(is_neg) buf[i--] = '-';
	fputs_unlocked(buf + i + 1, stdout);
}
