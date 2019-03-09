namespace Matching {
    const int N = 30007;
    int n, m, nxt[N], fa[N], S[N], Q[N], *Top = Q, pre[N], Vis[N], Tim = 0;
    struct edge {int to, next;}e[200007]; int head[N], cnt = 1;
    inline void addedge(int x, int y) {e[++cnt] = (edge){y, head[x]}; head[x] = cnt;}
    int getfa(int x) {return (x == fa[x]) ? x : (fa[x] = getfa(fa[x]));}
    inline int lca(int x, int y) {
        for(++Tim, x = getfa(x), y = getfa(y); ; x^= y^= x^= y)
            if(x) {
                if(Vis[x] == Tim) return x;
                Vis[x] = Tim;
                x = getfa(pre[nxt[x]]);
            }
    }
    inline void blossom(int x, int y, int l) {
        while(getfa(x) != l) {
            pre[x] = y;
            if(S[nxt[x]] == 1) S[*Top = nxt[x]] = 0, *Top++;
            if(fa[x] == x) fa[x] = l;
            if(fa[nxt[x]] == nxt[x]) fa[nxt[x]] = l;
            y = nxt[x];
            x = pre[y];
        }
    }
    inline int Match(int x) {
        for(int i = 1; i <= n; ++i) fa[i] = i;
        memset(S, 0xff, sizeof(S));
        S[*(Top = Q) = x] = 0; ++Top;
        for(int *i = Q; i != Top; *i++)
            for(int T = head[*i]; T; T = e[T].next) {
                int y = e[T].to;
                if(S[y] == -1) {
                    pre[y] = *i; S[y] = 1;
                    if(!nxt[y]) {
                        for(int u = y, v = *i, lst; v; u = lst, v = pre[u])
                            lst = nxt[v], nxt[v] = u, nxt[u] = v;
                        return 1;
                    }
                    S[*Top = nxt[y]] = 0; *Top++;
                }
                else if(!S[y] && getfa(y) != getfa(*i)) {
                    int l = lca(y, *i);
                    blossom(y, *i, l);
                    blossom(*i, y, l);
                }
            }
        return 0;
    }
}
