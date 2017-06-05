struct Node {
  int len, suf;
  map<char, int> tab;
};

#define PIN 0
#define ROOT 1
#define NODE(node) (nodes[node])
#define LINK(par, chl, c) NODE(par).tab[c] = (chl)
#define TGET(node, c) (                 \
    [](Node& n, char ch) -> int {       \
      auto it = n.tab.find(ch);         \
      if (it == n.tab.end()) return 0;  \
      return it->second;                \
    }(NODE(node), c))
#define TITER(par, chl, c)               \
    for (const auto& t : NODE(par).tab)  \
      if (int chl = t.second)            \
        if (char c = t.first)
#define NEW_NODE(a...) (nodes.emplace_back(a), (int) nodes.size() - 1)

struct Automaton {
  vector<Node> nodes;

  Automaton() {
    if (NEW_NODE() != PIN)  assert(false);
    if (NEW_NODE() != ROOT) assert(false);
    NODE(PIN).len = -1;
    NODE(ROOT).len = 0;
    NODE(ROOT).suf = PIN;
  }

  int MaybeSplit(int v, char c, int q) {
    if (NODE(v).len + 1 == NODE(q).len) return q;
    const int cpy = NEW_NODE(NODE(q));
    NODE(cpy).len = NODE(v).len + 1;
    while (TGET(v, c) == q) {
      LINK(v, cpy, c);
      v = NODE(v).suf;
    }
    return NODE(q).suf = cpy;
  }

  int AddLetter(int last, char c) {
    if (const int q = TGET(last, c)) {
      return MaybeSplit(last, c, q);
    } else if (!TGET(PIN, c)) {
      LINK(PIN, ROOT, c);
    }
    const int cur = NEW_NODE();
    NODE(cur).len = NODE(last).len + 1;
    while (true) {
      if (const int q = TGET(last, c)) {
        const int result = MaybeSplit(last, c, q);  // MaybeSplit can resize
        NODE(cur).suf = result;                     // the vector!!!
        return cur;
      }
      LINK(last, cur, c);
      last = NODE(last).suf;
    }
  }
};

int main() {
  Automaton a;
  int last = ROOT;
  for (char c : "napis") last = a.AddLetter(last, c);
  return 0;
}
