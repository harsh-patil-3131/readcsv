void add_in_node(node *nn, int amt, int t_id, char dt[], char *cmt){
	nn->amount=amt;
	nn->trans_ID=t_id;
	nn->d.d=atoi(strtok(dt, "/"));
	nn->d.month=atoi(strtok(NULL, "/"));
	nn->d.year=atoi(strtok(NULL, "\0"));
	strcpy(nn->comment, cmt);
}
//cmp(p->d<dt)
int cmp(date d, char dt[]){
	if(((10000*(d.year))+(100*(d.month))+d.d) < atoi(strtok(dt, "/"))+(100*atoi(strtok(NULL, "/")))+(10000*atoi(strtok(NULL,"\0")))){
		return 1;
	}
	return 0;
}
void dup_addTransaction(final *l, int amt, char dt[], char *cmt){
	node *nn=malloc(sizeof(node));
	if(l->start==NULL){
		l->start=nn;
		l->last=nn;
		nn->next=NULL;
		l->t_id++;
		add_in_node(nn, amt, l->t_id, dt, cmt);
	}
	else{
		node *p=l->start, *q;
		while(p && cmp(p->d, dt)){
			q=p;
			p=p->next;
		}
		if(p==NULL){
			l->last->next=nn;
			nn->next=NULL;
			l->t_id++;
			add_in_node(nn, amt, l->t_id, dt, cmt);
			return;
		}
		q->next=nn;
		nn->next=p;
		l->t_id++;
		add_in_node(nn, amt, l->t_id, dt, cmt);
	}
	
}
