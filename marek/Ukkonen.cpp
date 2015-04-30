struct Ukkonen
{
	typedef char znak;
	static const int max_len = 1000000;

	struct node
	{
		map<znak, pair<node*, pair<int, int> > > trans;
		node * suflink;

		// wypisuje drzewo - nie trzeba przepisywac!
		string getstr(int a, int b, znak * nap)
		{
			string s;
			for(int i = a; i <= b; i++)
				s += nap[i];
			return s;
		}

		// wypisuje drzewo - nie trzeba przepisywac!
		void wypisz(znak * nap, string s = "")
		{
			printf("%s: %p, suflink -> %p\n", s.c_str(), this, suflink);
			for(map<znak, pair<node*, pair<int, int> > >::iterator i = trans.begin(); i != trans.end(); i++)
			{
				for(int j = 0; j < (int) s.length(); j++)
					printf(" ");
				printf("(%c, %p, %d, %d [%s])\n", (*i).first, (*i).second.first, (*i).second.second.first, (*i).second.second.second, getstr((*i).second.second.first, (*i).second.second.second, nap).c_str());
				if((*i).second.first != nullptr)
					(*i).second.first->wypisz(nap, s + getstr((*i).second.second.first, (*i).second.second.second, nap));
			}
		}
	};

	int n;
	znak nap[max_len + 5]; // słowo powinno zajmować indeksy [1..n]
	node * root;
	node * pin;

	Ukkonen() : n(0), root(nullptr), pin(nullptr) {}

	void kanonizuj(node * & s, int & a, int b)
	{
		if(b < a)
			return;
		pair<node*, pair<int, int> > t = s->trans[nap[a]];
		node * sp = t.first;
		int ap = t.second.first;
		int bp = t.second.second;
		while(bp - ap <= b - a)
		{
			a = a + bp - ap + 1;
			s = sp;
			if(a <= b)
			{
				t = s->trans[nap[a]];
				sp = t.first;
				ap = t.second.first;
				bp = t.second.second;
			}
		}
	}

	bool splituj(node * s, int a, int b, znak c, node * & ret)
	{
		if(a <= b)
		{
			pair<node*, pair<int, int> > & t = s->trans[nap[a]];
			node * sp = t.first;
			int ap = t.second.first;
			int bp = t.second.second;
			if(c == nap[ap + b - a + 1])
			{
				ret = s;
				return true;
			}
			ret = new node();
			t.second.second = ap + b - a;
			t.first = ret;
			ret->trans[nap[ap + b - a + 1]] = make_pair(sp, make_pair(ap + b - a + 1, bp));
			return false;
		}
		ret = s;
		return s->trans.find(c) != s->trans.end();
	}

	void update(node * & s, int & a, int & i)
	{
		node * oldr = root;
		node * r;
		bool end = splituj(s, a, i - 1, nap[i], r);
		while(!end)
		{
			r->trans[nap[i]] = make_pair(nullptr, make_pair(i, n));
			if(oldr != root)
				oldr->suflink = r;
			oldr = r;
			s = s->suflink;
			kanonizuj(s, a, i - 1);
			end = splituj(s, a, i - 1, nap[i], r);
		}
		if(oldr != root)
			oldr->suflink = s;
	}

	void konstruuj()
	{
		root = new node();
		pin = new node();
		for(int i = 1; i <= n; i++)
			pin->trans[nap[i]] = make_pair(root, make_pair(i, i));
		root->suflink = pin;
		node * s = root;
		int a = 1;
		for(int i = 1; i <= n; i++)
		{
			update(s, a, i);
			kanonizuj(s, a, i);
		}
	}

	// wypisuje slowa w kolejnosci alfabetycznej, nie trzeba przepisywac!
	void podslowa(node * v = nullptr, string s = "")
	{
		if(v == nullptr)
			v = root;
		for(auto i : v->trans)
		{
			string f = s;
			for(int j = i.second.second.first; j <= i.second.second.second; j++)
			{
				f += nap[j];
				printf("%s\n", f.c_str());
			}
			if(i.second.first != nullptr)
				podslowa(i.second.first, f);
		}
	}

	// nie trzeba przepisywac!
	void wypisz()
	{
		printf("pin = %p, root = %p\n", pin, root);
		if(pin != nullptr)
			pin->wypisz(nap);
	}
};

/* Przykład użycia */
int main()
{
	Ukkonen u;
	scanf("%s", u.nap + 1);
	u.n = strlen(u.nap + 1);
	u.konstruuj();
	u.podslowa();
	//u.wypisz();
	return 0;
}
