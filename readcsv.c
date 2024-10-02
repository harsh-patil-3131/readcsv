#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct date{
	int d;
	int month;
	int year;
}date;

typedef struct node{
	int amount;
	int trans_ID;
	date d;
	char comment[20];
	struct node *next;
}node;

typedef struct final{
	int t_id;
	node *start;
	node *last;
}final;
// amount,17/10/2024,comment

void init_fin(final *l){
	l->t_id=00;
	l->start=NULL;
	l->last=NULL;
	return;
}
void reverse(char str[], int len){ // reverses string(str[])
	int i=0;
	char ch;
	while(i<len/2){
		ch=str[i];
		str[i]=str[len-i-1];
		str[len-i-1]=ch;
		i++;
	}
}
char *sitoa(int a, char* buffer){ // converts string to integer(same as that of itoa)
	int i=0, rem;
	if(a==0){
		buffer[0]='0';
		buffer[1]='\0';
		return buffer;
	}
	while(a!=0){
		rem=a%10;
		buffer[i++]=rem + '0';
		a=a/10;
	}
	buffer[i]='\0';
	reverse(buffer, i);
	return buffer;
}

void add_in_node(node *nn, int amt, int t_id, char* dt, char *cmt){ //add given info. to given node
	char xyz[20];
	strcpy(xyz, dt);
	nn->amount=amt;
	nn->trans_ID=t_id;
	nn->d.d=atoi(strtok(xyz, "/"));
	nn->d.month=atoi(strtok(NULL, "/"));
	nn->d.year=atoi(strtok(NULL, "\0"));
	strcpy(nn->comment, cmt);
}

int cmp(date d, char dt[]){ // compares date d(structure) and dt(string)
	char xyz[20];
	strcpy(xyz, dt);
	if(((10000*(d.year))+(100*(d.month))+d.d) < atoi(strtok(xyz, "/"))+(100*atoi(strtok(NULL, "/")))+(10000*atoi(strtok(NULL,"\0")))){
		return 1;
	}
	return 0;
}
int cmp2(date d, char dt[]){ // compares date d(structure) and dt(string)
	char xyz[20];
	strcpy(xyz, dt);
	if(((10000*(d.year))+(100*(d.month))+d.d) <= atoi(strtok(xyz, "/"))+(100*atoi(strtok(NULL, "/")))+(10000*atoi(strtok(NULL,"\0")))){
		return 1;
	}
	return 0;
}
node* addTransaction(final *l, int amt, char dt[], char *cmt){
	node *nn=malloc(sizeof(node)); // adds transaction in linked list such that sorted w.r.t date
	if(nn==NULL){
		printf("nn==NULL\n");
		return nn;
	}
	if(l->start==NULL){
		l->start=nn;
		l->last=nn;
		nn->next=NULL;
		l->t_id++;
		add_in_node(nn, amt, l->t_id, dt, cmt);
	}
	else{
		node *p=l->start, *q;
		if(cmp(p->d, dt)==0){
			nn->next=l->start;
			l->start=nn;
			l->t_id++;
			add_in_node(nn, amt, l->t_id, dt, cmt);
			return nn;
		}
		while(p && cmp(p->d, dt)){
			q=p;
			p=p->next;
		}
		if(p==NULL){
			l->last->next=nn;
			l->last=nn;
			nn->next=NULL;
			l->t_id++;
			add_in_node(nn, amt, l->t_id, dt, cmt);
			return nn;
		}
		q->next=nn;
		nn->next=p;
		l->t_id++;
		add_in_node(nn, amt, l->t_id, dt, cmt);
	}
	return nn;
}

void import_data(final *l, char *file){ //reads file given into data
	char line[50], ch, b[12], c[20];
	int i, a;
	FILE *fp;
	fp=fopen(file, "r");
	ch=fgetc(fp);
	while(ch!=EOF && !feof(fp)){
		i=0;
		while(ch!='\n' && ch!=EOF){
			line[i]=ch;
			i++;
			ch=fgetc(fp);
		}
		line[i]='\0';
		//printf("line: %s\n", line);
		a=atoi(strtok(line, ","));
		strcpy(b, strtok(NULL, ","));
		strcpy(c, strtok(NULL, "\0"));
		addTransaction(l, a, b, c);
		ch=fgetc(fp);
	}
	fclose(fp);
}

void export_data(final *l, char *file){ // exports data present in linked list in given file
	FILE *fp;
	char buffer[20];
	node *p=l->start;
	fp=fopen(file, "w");
	while(p){
		fputs(sitoa(p->amount, buffer), fp);
		fputc(',', fp);
		fputs(sitoa(p->d.d, buffer), fp);
		fputc('/', fp);
		fputs(sitoa(p->d.month, buffer), fp);
		fputc('/', fp);
		fputs(sitoa(p->d.year, buffer), fp);
		fputc(',', fp);
		fputs(p->comment, fp);
		fputc('\n', fp);
		p=p->next;
	}
	fclose(fp);
}
void viewTransactions(final *l, char dt1[], char dt2[]){
	node *p=l->start;
	while(p && cmp(p->d, dt1)){
		p=p->next;
	}
	if(p==NULL){
		printf("no transaction present\n");
		return;
	}
	printf("\t   Date    Tran_id  amount  comment\n");
	while(p && cmp2(p->d, dt2)){
		printf("\t%2d/%2d/%4d   %3d    %6d    %s\n", p->d.d, p->d.month, p->d.year, p->trans_ID, p->amount, p->comment);
		p=p->next;
	}
}
void updateTransaction(final *l, int id, char dt[], int amount, char cmt[]){
	node *p=l->start, *q;
	if(p->trans_ID == id){
		l->start=p->next;
		p=addTransaction(l, amount, dt, cmt);
		p->trans_ID=id;
		l->t_id--;
		return;
	}
	while(p && (p->trans_ID != id)){
		q=p;
		p=p->next;
	}
	if(p==NULL){
		printf("No transaction ID found\n");
		return;
	}
	q->next=p->next;
	p=addTransaction(l, amount, dt, cmt);
	p->trans_ID=id;
	l->t_id--;
}

int main(int argc, char *argv[]){
	final l;
	node *p;
	init_fin(&l);
	addTransaction(&l, 22, "14/5/2024", "Raje0");
	addTransaction(&l, 3131, "13/05/2024", "Raje1");
	addTransaction(&l, 4141, "12/05/2024", "Raje2");
	addTransaction(&l, 5151, "12/11/2024", "Raje3");
	addTransaction(&l, 616161, "12/02/2024", "Raje4");
	import_data(&l, argv[1]);
	printf("after import_data\n");
	p=l.start;
	int i=0;
	while(p){
		printf("%d,%d,%d/%d/%d,%s\n", p->amount, p->trans_ID, p->d.d, p->d.month, p->d.year, p->comment);
		p=p->next;
		i++;
	}
	printf("Transactions: from 12/05/2024 to 14/5/2024\n");
	viewTransactions(&l, "12/5/2024", "14/5/2024");
	updateTransaction(&l, 5, "17/10/2024", 7620, "Patil mhantyat");
	printf("After updateTransaction()\n");
	p=l.start;
	while(p){
		printf("%d,%d,%d/%d/%d,%s\n", p->amount, p->trans_ID, p->d.d, p->d.month, p->d.year, p->comment);
		p=p->next;
		i++;
	}
	printf("*****\n\n");
	export_data(&l, argv[2]);
	return 0;
}


