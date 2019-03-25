//Generuje słowo cykliczne, Działa dla alph >= 2
vector <int> de_bruijn(int len, int alph){
	vector <int> res, lyn{0};
	while (lyn[0] != alph - 1){
		int r = lyn.size();
		if (len % r == 0)
			for (int c : lyn)
				res.push_back(c);
		for (int i = r; i <= len - 1; ++i)
			lyn.push_back(lyn[i - r]);
		while (lyn.back() == alph - 1)
			lyn.pop_back();
		lyn.back()++;
	}
	res.push_back(alph - 1);
	return res;
}
