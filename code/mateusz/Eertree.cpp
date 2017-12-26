/* DRZEWO PALINDROMÓW O(n).                                                   */
/* n to aktualna liczba dodanych liter+1, last to najdłuższy palindro-sufiks  */
/* aktualnego słowa, sz to aktualny rozmiar drzewa                            */

const int nax=100*1007;
const int alfa=26;
 
int s[nax], len[nax], link[nax], graf[nax][alfa];
int n, last, sz;
 
void init()
{
    s[n++]=-1;
    link[0]=1;
    len[1]=-1;
    sz=2;
}
 
int get_link(int v)
{
    while(s[n-len[v]-2]!=s[n-1]) v=link[v];
    return v;
}
 
void add_letter(int c)
{
    s[n++]=c;
    last=get_link(last);
    if(!graf[last][c])
    {
        len[sz]=len[last]+2;
        link[sz]=graf[get_link(link[last])][c];
        graf[last][c]=sz++;
    }
    last=graf[last][c];
}
