struct node {
	int roz=1;
	node *lew, *oj, *pra;
	node() { lew=pra=oj=NULL; }
};
void update(node *v) {
	if (v==NULL) return;
	v->roz=1;
	for (int h=0; h<2; h++) {
		if (v->lew!=NULL) {
			v->roz+=v->lew->roz;
		}
		swap(v->lew, v->pra);
	}
}
node *merge(node *v, node *u) {
	if (v==NULL) return u;
	if (u==NULL) return v;	
	if (rand()%(v->roz+u->roz)<(v->roz)) {
		update(v);//czasem można usunąć
		v->pra=merge(v->pra, u);
		v->pra->oj=v;
		update(v);
		return v;
	} else {
		update(u);//czasem można usunąć
		u->lew=merge(v, u->lew);
		u->lew->oj=u;
		update(u);
		return u;
	}
}
pair <node*, node*> split(node *v, const function <bool(node*)> &is_left) {
	if (v==NULL)
		return make_pair(v, v);
	pair <node*, node*> ret;
	update(v);//czasem można usunąć
	v->oj=NULL;
	if (is_left(v)) {
		ret=split(v->pra, is_left);
		v->pra=ret.first;
		if (v->pra!=NULL)
			v->pra->oj=v;
		ret.first=v;
	} else {
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
function <bool(node*)> cut_v(int v) {
	gl_help=v;
	return [](node *u)->bool {
		int pom=1;
		if (u->lew!=NULL) pom+=u->lew->roz;
		if (pom>gl_help) return false;
		gl_help-=pom;
		return true;
	};
}
int position(node *v) {
	int ret=1;
	if (v->lew!=NULL) ret+=v->lew->roz;
	if (v->oj==NULL) return ret;
	return position(v->oj)+ret-(v->oj->lew==v)*(v->roz+1);
}
