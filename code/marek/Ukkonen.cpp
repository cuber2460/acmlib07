template <typename Char>
struct Ukkonen {
  // Musi być ściśle większe niż jakakolwiek długość słowa.
  static constexpr int kInfinity = numeric_limits<int>::max();

  struct Node {
    map<Char, pair<Node*, pair<int, int>>> transition;
    Node* suflink;
  };

  // Ta metoda jest wywoływana zawsze gdy tworzona jest krawędź {node}[a, +oo).
  void CreateLeafCallback(Node* node, int a) {
    // printf("CreateLeafCallback({%p}[%d, +oo))\n", node, a);
  }

  // Ta metoda jest wywoływana zawsze gdy krawędź {node}[a, b] zamienia się
  // w dwie krawędzie: {node}[a, c-1], {middle}[c, b].
  void SplitEdgeCallback(Node* node, int a, int b, Node* middle, int c) {
    // printf("SpltEdgCllbck({%p}[%d, %d] -> {%p}[%d, %d] + {%p}[%d, %d])\n",
    //        node, a, b, node, a, c - 1, middle, c, b);
  }

  // vector<unique_ptr<Node>> nodes_to_delete;  // Odkomentować w celu usuwania.
  Node* NewNode() {
    Node* node = new Node();
    // nodes_to_delete.insert(node);            // Odkomentować w celu usuwania.
    return node;
  }

  vector<Char> text;  // Słowo powinno zajmować indeksy [0..n-1].
  Node* root;
  Node* pin;
  Node* last_explicit_node;  // Ostatni wierzchołek ,,explicit''.
  int last_length;    // Liczba literek do ostatniego wierzchołka ,,implicit''.

  // "reserve" warto ustawić na maksymalną dlugość słowa, ale wcale nie trzeba.
  Ukkonen(const int reserve = 0) : root(nullptr), pin(nullptr) {
    text.reserve(reserve);
    root = NewNode();
    pin = NewNode();
    root->suflink = pin;
    last_explicit_node = root;
    last_length = 0;
  }

  void Canonize(Node** s, int* a, int b) {
    if (b < *a) return;
    pair<Node*, pair<int, int>> t = (*s)->transition[text[*a]];
    Node* sp = t.first;
    int ap = t.second.first;
    int bp = t.second.second;
    while (bp - ap <= b - *a) {
      *a = *a + bp - ap + 1;
      *s = sp;
      if (*a <= b) {
        t = (*s)->transition[text[*a]];
        sp = t.first;
        ap = t.second.first;
        bp = t.second.second;
      }
    }
  }

  bool TestAndSplit(Node* s, int a, int b, Char c, Node** ret) {
    if (a <= b) {
      pair<Node*, pair<int, int>>& t = s->transition[text[a]];
      Node* sp = t.first;
      int ap = t.second.first;
      int bp = t.second.second;
      if (c == text[ap + b - a + 1]) {
        *ret = s;
        return true;
      }
      *ret = NewNode();
      t.second.second = ap + b - a;
      t.first = *ret;
      (*ret)->transition[text[ap + b - a + 1]] =
          make_pair(sp, make_pair(ap + b - a + 1, bp));
      SplitEdgeCallback(s, ap, bp, *ret, ap + b - a + 1);
      return false;
    }
    *ret = s;
    return s->transition.find(c) != s->transition.end();
  }

  void Update(Node** s, int* a, int i) {
    Node* oldr = root;
    Node* r;
    bool end = TestAndSplit(*s, *a, i - 1, text[i], &r);
    while (!end) {
      CreateLeafCallback(r, i);
      r->transition[text[i]] = make_pair(nullptr, make_pair(i, kInfinity));
      if (oldr != root) oldr->suflink = r;
      oldr = r;
      *s = (*s)->suflink;
      Canonize(s, a, i - 1);
      end = TestAndSplit(*s, *a, i - 1, text[i], &r);
    }
    if (oldr != root) oldr->suflink = *s;
  }

  // Dodaje kolejną literę do drzewa.
  void AddLetter(Char z) {
    const int i = static_cast<int>(text.size());
    text.push_back(z);
    auto it = pin->transition.find(z);
    if (it == pin->transition.end())
      pin->transition[z] = make_pair(root, make_pair(i, i));
    Update(&last_explicit_node, &last_length, i);
    Canonize(&last_explicit_node, &last_length, i);
  }

  // Zamienia wszystkie krawędzie: [x, +oo) -> [x, text.size()-1].
  void ClearInfinities(Node* node = nullptr) {
    if (node == nullptr) node = root;
    for (auto& it : node->transition) {
      if (it.second.second.second == kInfinity)
        it.second.second.second = static_cast<int>(text.size()) - 1;
      else ClearInfinities(it.second.first);
    }
  }
};

template <typename Char>
constexpr int Ukkonen<Char>::kInfinity;

int main() {  // Przykład użycia.
  string s = "abcdefgh#";
  Ukkonen<char> u(s.size() /* reserve */);
  for (char c : s) u.AddLetter(c);
  u.ClearInfinities();
  return 0;
}
