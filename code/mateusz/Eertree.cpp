/* DRZEWO PALINDROMÓW O(n).                                                   */
/* n to aktualna liczba dodanych liter+1, last to najdłuższy palindro-sufiks  */
/* aktualnego słowa, sz to aktualny rozmiar drzewa                            */
/*                                                                            */
/* Poniżej kod liczący podział na parzyste palindromy minimalizujący liczbę   */
/* palindromów dłuższych niż 2.                                               */
const int maxn = 1000*1000+7, sigma = 26;
int len[maxn], link[maxn], to[maxn][sigma];
int slink[maxn], diff[maxn];
pair <int,int> series_ans[maxn];
int ans[maxn], z[maxn];
int sz, last, n;
char s[maxn];

void init() {
    s[n++] = -1;
    link[0] = 1;
    len[1] = -1;
    sz = 2;
}
int get_link(int v) {
    while(s[n - len[v] - 2] != s[n - 1]) v = link[v];
    return v;
}
void add_letter(char c) {
    s[n++] = c -= 'a';
    last = get_link(last);
    if(!to[last][c])
    {
        len[sz] = len[last] + 2;
        link[sz] = to[get_link(link[last])][c];
        diff[sz] = len[sz] - len[link[sz]];
        if(diff[sz] == diff[link[sz]])
            slink[sz] = slink[link[sz]];
        else
            slink[sz] = link[sz];
        to[last][c] = sz++;
    }
    last = to[last][c];
}
int main() {
    init();
    for(int i = 1; i <= nn; i++) {
        add_letter(tek[i]);
        for(int v = last; len[v] > 0; v = slink[v]) {
            series_ans[v] = {ans[i - (len[slink[v]] + diff[v])], i - (len[slink[v]] + diff[v])};
            if(diff[v] == diff[link[v]])
                series_ans[v] = min(series_ans[v], series_ans[link[v]]);
            if (!(i&1)) {
                if (series_ans[v].first+1<ans[i]) {
                    ans[i] = series_ans[v].first + 1;
                    z[i] = series_ans[v].second;
                }
            }
        }
        if (!(i&1) && tek[i]==tek[i-1] && ans[i-2]<ans[i]) {
            ans[i]=min(ans[i], ans[i-2]);
            z[i]=i-2;
        }
    }
}
