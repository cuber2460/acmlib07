/* TREAP O(q*log(q))                                                          */
/* drzewo amortyzowane randomizowanie, merge() łączy dwa drzewa,              */
/* split() rozbija jedno drzewo na dwa, dwie podstawowe operacje              */
/* cut_leq() i cut_v() to funkcje podawane do split(), gl_help to ich zmienna */
/* pomocnicza                                                                 */

struct node
{
	int id, roz, prior;
	node * lew;
	node * pra;
	node * oj;
	node(int idid)
	{
		prior=rand();
		lew = pra = oj = NULL;
		roz=1;
		id=idid;
	}
	node()
	{
		prior=rand();
		lew = pra = oj = NULL;
		roz=1;
	}
};
inline void update(node * v)
{
	if (v==NULL) return;
	v->roz=1;
	if (v->lew!=NULL)
		v->roz+=v->lew->roz;
	if (v->pra!=NULL)
		v->roz+=v->pra->roz;
	//tutaj dodajemy wszystkie operacje związane z funkcjami m.in. drzewa przedziałowego, tzn. lazy propagation itp.
}
node * merge(node * v, node * u)
{
	if (v==NULL) return u;
	if (u==NULL) return v;
	if ((v->prior)>=(u->prior))
	{
		update(v);//czasem można usunąć
		v->pra=merge(v->pra, u);
		if (v->pra!=NULL)
			v->pra->oj=v;
		update(v);
		return v;
	}
	else
	{
		update(u);//czasem można usunąć
		u->lew=merge(v, u->lew);
		if (u->lew!=NULL)
			u->lew->oj=u;
		update(u);
		return u;
	}
}
pair <node *, node *> split(node * v, function <bool(node*)> is_left)//is_left ma stwierdzać, czy dany wierzchołek powinien być w lewej części
{
	if (v==NULL)
		return make_pair(v, v);
	pair <node *, node *> ret;
	update(v);//czasem można usunąć
	v->oj=NULL;
	if (is_left(v))
	{
		ret=split(v->pra, is_left);
		v->pra=ret.first;
		if (v->pra!=NULL)
			v->pra->oj=v;
		ret.first=v;
	}
	else
	{
		ret=split(v->lew, is_left);
		v->lew=ret.second;
		if (v->lew!=NULL)
			v->lew->oj=v;
		ret.second=v;
	}
	update(v);
	return ret;
}
int gl_help;
function <bool(node*)> cut_leq(int v)//ucnina elementy z id<=v(jeśli posortowane po id)
{
	gl_help=v;
	return [](node* u)->bool{return u->id<=gl_help;};
}
function <bool(node*)> cut_v(int v)//ucnina v pierwszych elementow
{
	gl_help=v;
	return [](node* u)->bool
	{
		int pom=1;
		if (u->lew!=NULL)
			pom+=u->lew->roz;
		if (pom>gl_help)
			return false;
		gl_help-=pom;
		return true;
	};
}
int position(node * v)//mówi który w kolejności (w swoim drzewie) jest dany wierzchołek
{
	int ret=1;
	if (v->lew!=NULL)
		ret+=v->lew->roz;
	if (v->oj==NULL)
		return ret;
	return position(v->oj)+ret-(v->oj->lew==v)*(v->roz+1);
}
