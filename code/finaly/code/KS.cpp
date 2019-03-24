using triple = tuple<int,int,int>;
using vi = vector<int>;

void ksRadixPass(const vi&src, vi&dest, int size, const vi&keys, int K){
  vi cnt(K+1, 0);
  for(int i = 0; i < size; i++) { cnt[keys[src[i]]]++; }
  int prefSum = 0;
  for(int i = 0; i <= K; i++){
    int tmp = cnt[i];
    cnt[i] = prefSum;
    prefSum += tmp;
  }
  for(int i = 0; i < size; i++){
    int& pos = cnt[keys[src[i]]];
    dest[pos] = src[i];
    pos++;
  }
}
void ksSuffixArray(const vi& src, vi& dest, int N, int K){
  int n0 = (N + 2) / 3, n2 = N / 3, n02 = n0 + n2;
  vi src12(n02 + 3, 0), dest12(n02 + 3, 0), src0(n0, 0), dest0(n0, 0);
  int ptr = 0;
  for(int i = 0; i < N + (N % 3 == 1); i++) {
    if(i % 3 != 0) src12[ptr++] = i;
  }
  ksRadixPass( src12, dest12, n02, vi(src.begin()+2, src.end()), K);
  ksRadixPass(dest12,  src12, n02, vi(src.begin()+1, src.end()), K);
  ksRadixPass( src12, dest12, n02, src, K);
  int num = 0;
  triple bigt = triple(-1, -1, -1);
  for (int i = 0; i < n02; i++) {
    int pos = dest12[i];
    triple newt = triple(src[pos], src[pos+1], src[pos+2]);
    if (newt != bigt) {
      num++;
      bigt = newt;
    }
    if (pos % 3 == 1) src12[pos / 3] = num;
    else src12[pos / 3 + n0] = num;
  }
  if (num < n02) {
    ksSuffixArray(src12, dest12, n02, num);
    for (int i = 0; i < n02; i++) { src12[dest12[i]] = i + 1; }
  } 
  else for (int i = 0; i < n02; i++) { dest12[src12[i] - 1] = i; }
  ptr = 0;
  for (int i = 0; i < n02; i++) if (dest12[i] < n0) src0[ptr++] = 3 * dest12[i];
  ksRadixPass(src0, dest0, n0, src, K);
  for (int p = 0, t = (N % 3 == 1), k=0; k < N; k++) {
    int i = dest12[t] < n0 ? dest12[t] * 3 + 1 : (dest12[t] - n0) * 3 + 2, j = dest0[p];
    bool take0;
    if (dest12[t] < n0) {
      take0 = make_pair(src[i], src12[dest12[t] + n0]) < make_pair(src[j], src12[j / 3]);
    } else {
      take0 = triple(src[i], src[i + 1], src12[dest12[t] - n0 + 1]) <
              triple(src[j], src[j + 1], src12[j / 3 + n0]);
    }
    if (take0) {
      dest[k] = i; t++;
      if (t == n02) {
        k++;
        while (p < n0) {
          dest[k] = dest0[p]; k++; p++;
        }
      } /* end if */
    } else {
      dest[k] = j; p++;
      if (p == n0) {
        k++;
        while (t < n02) {
          if (dest12[t] < n0) dest[k] = dest12[t] * 3 + 1;
          else dest[k] = (dest12[t] - n0) * 3 + 2;
          k++; t++;
        }
      }
    }
  }
}
vi suffixArray(vi source) {
  int N = source.size(), K = *max_element(source.begin(), source.end());
  if (N == 1) { return {0}; }
  for (int& v : source) { v++; }
  for (int i = 0; i < 3; i++) { source.push_back(0); }
  vi result(N + 3);
  ksSuffixArray(source, result, N, K + 2);
  result.resize(N);
  return result;
} /* {1, 2, 1, 1, 2} -> {2, 3, 0, 4, 1} */

vi lcp(vi source, const vi &suf) {
  int N = source.size(), k = 0;
  source.push_back(-1);
  vi rank(N), ans(N);
  for (int i = 0; i < N; ++i) rank[suf[i]] = i;
  for (int i = 0; i < N; ++i) {
    int ak = rank[i];
    if (ak) while (source[suf[ak] + k] == source[suf[ak - 1] + k]) k++;
    ans[ak] = k;
    if (k) k--;
  }
  return ans;
} /* dla przykladu powyzej: {0, 1, 2, 0, 1} */
