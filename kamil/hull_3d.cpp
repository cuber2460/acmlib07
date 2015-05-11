// hull_3d
struct POINT {
    POINTT x,y;
    POINT(POINTT wx, POINTT wy) : x(wx), y(wy) {}
    POINT() {}
    bool operator ==(POINT& a) {return a.x==x && a.y==y;}
};
//algorytm znajduje zasmiecona otoczke (z wszystkimi punktami na scianach)
//jesli chcemy miec tylko wierzcholki otoczki, to odkomentowujemy

//uwaga na LL (nie ma ich w iloczynie wektorowym) i ew. przepelnienie

//jak przerabiamy na double'e, to sciany grupujemy po znormalizowanym
//wektorze normalnym

//klasyfikacja punktow, patrzymy do ilu roznych (pod wzdledem wektorow normalnych)
//scian nalezy dany punkt:
//0 - nie naleze do wypuklej otoczki
//1 - punkt wewnetrzny sciany
//2 - punkt wewnatrz krawedzi
//3 i wiecej - wierzcholek

const int N=1000;
int n;
struct sciana{
    int t[3]; //numery wierzcholkow sciany
    sciana(){}
    sciana(int a,int b,int c) {t[0]=a; t[1]=b; t[2]=c;}
    bool ok(){return t[0]<n && t[1]<n && t[2]<n;} //tylko dla wspolplaszczyznowych
};

struct POINT3D{
    int x,y,z;
    POINT3D() : x(0), y(0), z(0) {}
    POINT3D(int X,int Y,int Z) : x(X), y(Y), z(Z) {}
};
bool operator <(POINT3D a,POINT3D b){
    if (a.x-b.x) return a.x<b.x;
    if (a.y-b.y) return a.y<b.y;
    return a.z<b.z;
}
inline POINT3D operator ^(POINT3D a,POINT3D b){
    return POINT3D(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
}
inline POINT3D operator -(POINT3D a,POINT3D b){
    return POINT3D(a.x-b.x,a.y-b.y,a.z-b.z);
}
inline LL operator *(POINT3D a,POINT3D b){
    return (LL)a.x*b.x+(LL)a.y*b.y+(LL)a.z*b.z;
}
inline bool operator ==(POINT3D a,POINT3D b){
    return a.x==b.x && a.y==b.y && a.z==b.z;
}
ostream& operator<<(ostream& out, POINT3D pp) {
    cout << "(" << pp.x << "," << pp.y << "," << pp.z << ")";
    return out;
}
vector<sciana> v;
POINT3D p[N];
int t[N][N]; //numer sciany do ktorej nalezy krawedz
int vis[2*N]; //scian jest nie wiecej niz 2*n-4
map<POINT3D, vector<int> > mapa; //sciany
map<int, int> klas; //klasyfikacja wierzcholkow

void reset(){ //opcjonalnie
    v.clear();
    mapa.clear();
    klas.clear();
    CLR(t, 0);
    CLR(vis, 0);
}

void step(int x){
    VPII pom;
    //znajdujemy widoczne sciany
    REP(i,SZ(v)){
        POINT3D normal=((p[v[i].t[1]]-p[v[i].t[0]])^(p[v[i].t[2]]-p[v[i].t[0]]));
        LL il=(normal*(p[x]-p[v[i].t[0]]));
        vis[i]=0;
        if (il>0) vis[i]=1;
        else if (il==0){
            if ((normal*((p[v[i].t[1]]-p[v[i].t[0]])^(p[x]-p[v[i].t[0]]))>=0
                        && normal*((p[v[i].t[2]]-p[v[i].t[1]])^(p[x]-p[v[i].t[1]]))>=0
                        && normal*((p[v[i].t[0]]-p[v[i].t[2]])^(p[x]-p[v[i].t[2]]))>=0))
                return; /*ODKOMENTUJ*/
            vis[i]=1;
        }
    } 
    int ile=SZ(v);
    REP(i,ile) if (vis[i])
        REP(j,3) if (!vis[t[v[i].t[(j+1)%3]][v[i].t[j]]])
        pom.PB(MP(v[i].t[j],v[i].t[(j+1)%3]));
    REP(i,ile) if (vis[i]){
        swap(v[i],v.back());
        v.pop_back();
        REP(j,3) t[v[i].t[j]][v[i].t[(j+1)%3]]=i;
        vis[i--]=vis[--ile]; 
    }  
    FORE(it,pom){
        t[it->X][it->Y]=t[it->Y][x]=t[x][it->X]=SZ(v);
        v.PB(sciana(it->X,it->Y,x));
    }
}
void CH3D(){ //n>=3, oblicza wektor trojkatnych scian v (mozliwe powtorzenia)
    int i=2;
    POINT3D normal;
    while (i<n && (normal=((p[1]-p[0])^(p[i]-p[0])))==POINT3D(0,0,0)) i++; 
    if (i>=n) return; //wspolliniowe
    int x=i++;
    v.PB(sciana(0,1,x));
    v.PB(sciana(1,0,x));
    t[0][1]=t[1][x]=t[x][0]=0;
    t[1][0]=t[0][x]=t[x][1]=1;
    while (i<n && normal*(p[i]-p[0])==0) i++; 
    if (i==n){ //wspolplaszczyznowe, tworzona jest sztuczna sciana dla kazdej krawedzi!!!
        v.clear();
        p[n++] = POINT3D(3123,-3123,954); //pkt spoza plaszczyzny, uwaga na zakresy!
        CH3D();
        vector<sciana> v2;
        n--;
        return;
    }
    step(i);
    FOR(j,2,n-1) if (j!=i && j!=x) step(j);
}
/****************** wszystko ponizej jest opcjonalne ****************************/
double area() {
    double res = 0.0;
    REP(i,SZ(v)) if(v[i].ok()){
        POINT3D normal=((p[v[i].t[1]]-p[v[i].t[0]])^(p[v[i].t[2]]-p[v[i].t[0]]));
        double foo=(double)normal.x*normal.x+(double)normal.y*normal.y+(double)normal.z*normal.z;
        foo=sqrt(foo);
        res+=foo;
    }
    return 0.5*res;
}

void compute_walls(){ //laczy sciany trojkatne w wielokaty i wyznacza klasyfikacje, najpierw odpal CH3D()
    REP(i,SZ(v)){
        POINT3D normal=((p[v[i].t[1]]-p[v[i].t[0]])^(p[v[i].t[2]]-p[v[i].t[0]]));
        int foo=__gcd(abs(normal.x),__gcd(abs(normal.y),abs(normal.z)));
        normal.x/=foo;
        normal.y/=foo;
        normal.z/=foo;
        REP(j,3) if(v[i].t[j] < n) mapa[normal].PB(v[i].t[j]);
    }

    FORE(i,mapa) {
        sort(ALL(i->Y));
        i->Y.erase(unique(ALL(i->Y)), i->Y.end());
        FORE(j,i->Y) klas[*j]++;
    }
}
/* sortuje wierzcholki na scianach w kolejnosci, tylko dla wersji bez smieci */
int pocz;
POINT3D norm;
bool CHcomp(int x,int y){
    if(x == pocz) return true;
    if(y == pocz) return false;
    return ((p[x]-p[pocz]) ^ (p[y]-p[pocz])) * norm < 0;
}
void sort_walls(){ //najpierw odpal compute_walls()
    FORE(i,mapa){
        vector<int>& w = i->Y;
        pocz=w[0];
        norm=(p[w[1]]-p[w[0]])^(p[w[2]]-p[w[0]]);
        sort(ALL(w), CHcomp);
    }
}
/*****************************************************************************/
void show(){
    cout << "Sciany trojkatne:" << endl;
    FORE(i,v) cout << p[i->t[0]] << " " << p[i->t[1]] << " " << p[i->t[2]] << endl;
    cout << "Klasyfikacja:" << endl;
    REP(i,n) cout << p[i] << " " << klas[i] << endl;
    cout << "Sciany:" << endl;
    FORE(it, mapa){
        cout << it->X << ": ";
        FORE(it2,(it->Y)) cout << p[*it2] << " "; cout << endl;
    }
    cout << "Pole: " << area() << endl;
}
int main() {
    cin >> n;
    REP(i,n) cin >> p[i].x >> p[i].y >> p[i].z;
    random_shuffle(p,p+n);
    CH3D();
    compute_walls();
    sort_walls();
    show();
    return 0;
}
