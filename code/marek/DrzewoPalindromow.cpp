template <typename T>
struct PalTree {
  struct PalNode {
    PalNode* suflink;
    int length;
    unordered_map<T, PalNode*> children;
    int depth;  // Długość ścieżki suflinków do korzenia. Nie jest potrzebne.

    PalNode() : depth(0) {}

    PalNode* GetChild(T t) {
      auto it = children.find(t);
      if (it == children.end()) return nullptr;
      return it->second;
    }

    void SetChild(T t, PalNode* child) {
      children[t] = child;
    }
  };

  int n;
  const T* s;
  PalNode *pin, *root;
  vector<PalNode> nodes;

  // Tablice trzymające palindromy kończące się na każdym sufiksie/prefiksie.
  // Nie są potrzebne, jak się ich nie chce.
  vector<PalNode*> longest_on_suffix;
  vector<PalNode*> longest_on_prefix;

  PalTree(const T* s_, int n_) : n(n_), s(s_), nodes(n_ + 2) {
    pin = &nodes[0];
    root = &nodes[1];
    pin->length = -1;
    root->length = 0;
    pin->suflink = pin;
    root->suflink = pin;
    PalNode* last = root;
    for (int i = 0; i < n; i++) {
      while (i - last->length - 1 < 0 or s[i - last->length - 1] != s[i]) {
        last = last->suflink;
      }
      PalNode* maybe_child = last->GetChild(s[i]);
      if (maybe_child != nullptr) {
        last = maybe_child;
      } else {
        PalNode* new_node = &nodes[i + 2];
        last->SetChild(s[i], new_node);
        new_node->length = last->length + 2;
        if (last == pin) {
          new_node->suflink = root;
        } else {
          last = last->suflink;
          while (s[i - last->length - 1] != s[i]) {
            last = last->suflink;
          }
          new_node->suflink = last->GetChild(s[i]);
        }
        last = new_node;
        // Nie trzeba przepisywać tej linijki, jak się nie chce @depth.
        new_node->depth = new_node->suflink->depth + 1;
      }
      // Nie jest potrzebne, jak się nie chce tej tablicy.
      longest_on_suffix.push_back(last);
    }
    // Dalszy kod liczy tablicę @longest_on_prefix. Można wywalić, jak się
    // nie chce tej tablicy.
    last = root;
    longest_on_prefix.resize(n);
    for (int i = n - 1; i >= 0; i--) {
      while (i + last->length + 1 >= n or s[i + last->length + 1] != s[i]) {
        last = last->suflink;
      }
      last = last->GetChild(s[i]);
      longest_on_prefix[i] = last;
    }
  }
};
