const int N = 510;
struct NegCycle{
    typedef LD T; //type of costs
    const T INF=1e18;
    const T EPS = 1e-18;

    struct edge{
        int from, to,id; 
        T cost;
        edge(int _from, int _to, int _id, T _cost): from(_from), to(_to),id(_id), cost(_cost){}
    };

    int n;
    vector<edge> E;
    T dist[N];
    int from[N], vis[N];

    NegCycle(int _n): n(_n), E() {}

    // Krawedz od f do t o koszcie cost i identyfikatorze id
    void addEdge(int f,int t,int id, T cost){
        E.PB(edge(f, t, id, cost));
    }

    // Sprawdza czy w grafie istnieje ujemny cykl osiagalny z wierzcholka start.
    // Jesli takowy cykl istnieje, to funkcja zwraca go w postaci wektora identyfikatorów krawędzi
    // Pusty wektor oznacza brak ujemnego cyklu. Zlozonosc O(nm). 
    // Jesli chcemy sprawdzic czy istnieje cykl o wadze <= 0, to zmieniamy wage
    // w na (n+1)*w-1 i sprawdzamy czy w grafie istnieje cykl o wadze ujemnej 
    VI find(int start){
        REP(i, n) from[i] = vis[i] = -1;
        int who = -1;
        REP(i,n) dist[i] = INF;
        dist[start]=0;

        bool change = false;
        REP(_,n){
            change = false;
            FORE(it, E) {
                if (dist[it->from] != INF && dist[it->to] > dist[it->from] + it->cost + EPS) {
                    dist[it->to] = dist[it->from] + it->cost;
                    from[it->to] = it-E.begin();
                    who = it->to;
                    change = true;
                }
            }
        }
        if (!change) return VI();
        VI cycle;
        int t = 0;
        while(vis[who] == -1) {
            vis[who] = t++;
            cycle.PB(E[from[who]].id);
            who = E[from[who]].from;
        };
        cycle = VI(cycle.begin()+vis[who], cycle.end());
        reverse(ALL(cycle));
        return cycle;
    }
};
