struct Simplex {        // Maximize c*x subject to Ax <= b.
  using T = double;     // Initialize the structure, set A, b, c and then run
  vector<vector<T>> A;  // solve(). Max objective is stored in res. To recover
  vector<T> b, c;       // the best result, use getVars().
  int V, E;
  vector<int> eqIds, varIds, cols;
  T res;
  static constexpr T kEps = 1e-9;

  Simplex(int vars, int eqs) : A(eqs, vector<T>(vars)), b(eqs), c(vars),
      V(vars), E(eqs), eqIds(eqs), varIds(vars), res(0) {
    iota(varIds.begin(), varIds.end(), 0);
    iota(eqIds.begin(), eqIds.end(), vars);
  }

  void pivot(int eq, int var) {
    T coef = 1 / A[eq][var];
    cols.clear();
    for (int i = 0; i < V; i++) {
      if (abs(A[eq][i]) > kEps) { cols.push_back(i); A[eq][i] *= coef; }
    }
    A[eq][var] *= coef; b[eq] *= coef;
    for (int row = 0; row < E; row++) {
      if (row == eq || abs(A[row][var]) < kEps) { continue; }
      T k = -A[row][var];
      A[row][var] = 0;
      for (int i : cols) { A[row][i] += k * A[eq][i]; }
      b[row] += k * b[eq];
    }
    T q = c[var]; c[var] = 0;
    for (int i : cols) { c[i] -= q * A[eq][i]; }
    res += q * b[eq];
    swap(varIds[var], eqIds[eq]);
  }

  bool solve() {
    while (true) {
      int eq = -1, var = -1;
      for (int i = 0; i < E; i++) { if (b[i] < -kEps) { eq = i; break; } }
      if (eq == -1) { break; }
      for (int i = 0; i < V; i++) { if (A[eq][i] < -kEps) { var = i; break; } }
      if (var == -1) { res = -1e9; return false; /* No solution */ }
      pivot(eq, var);
    }
    while (true) {
      int var = -1, eq = -1;
      for (int i = 0; i < V; i++) { if (c[i] > kEps) { var = i; break; } }
      if (var == -1) { break; }
      for (int i = 0; i < E; i++) {
        if (A[i][var] < kEps) { continue; }
        if (eq >= 0 && b[i] / A[i][var] >= b[eq] / A[eq][var]) { continue; }
        eq = i;
      }
      if (eq == -1) { res = 1e9; return false; /* Unbounded */ }
      pivot(eq, var);
    }
    return true;
  }

  vector<T> getVars() { // Optimal assignment of variables.
    vector<T> result(V);
    for (int i = 0; i < E; i++) if (eqIds[i] < V) result[eqIds[i]] = b[i];
    return result;
  }
};
