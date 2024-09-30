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
void reverse(char str[], int len){
	int i=0;
	char ch;
	while(i<len/2){
		ch=str[i];
		str[i]=str[len-i-1];
		str[len-i-1]=ch;
		i++;
	}
}
char *sitoa(int a, char* buffer){
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
void add_in_node(node *nn, int amt, int t_id, char* dt, char *cmt){
	char xyz[20];
	strcpy(xyz, dt);
	nn->amount=amt;
	nn->trans_ID=t_id;
	nn->d.d=atoi(strtok(xyz, "/"));
	nn->d.month=atoi(strtok(NULL, "/"));
	nn->d.year=atoi(strtok(NULL, "\0"));
	strcpy(nn->comment, cmt);
}
int cmp(date d, char dt[]){
	char xyz[20];
	strcpy(xyz, dt);
	if(((10000*(d.year))+(100*(d.month))+d.d) < atoi(strtok(xyz, "/"))+(100*atoi(strtok(NULL, "/")))+(10000*atoi(strtok(NULL,"\0")))){
		return 1;
	}
	return 0;
}
void dup_addTransaction(final *l, int amt, char dt[], char *cmt){
	//printf("amt:%d dt:%s cmt:%s\n", amt, dt, cmt);
	node *nn=malloc(sizeof(node));
	if(nn==NULL){
		printf("nn==NULL\n");
		return;
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
			return;
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
			return;
		}
		q->next=nn;
		nn->next=p;
		l->t_id++;
		add_in_node(nn, amt, l->t_id, dt, cmt);
	}
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
		dup_addTransaction(l, a, b, c);
		ch=fgetc(fp);
	}
	fclose(fp);
}
void export_data(final *l, char *file){
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
int main(int argc, char *argv[]){
	final l;
	node *p;
	init_fin(&l);
	dup_addTransaction(&l, 22, "12/11/1930", "Raje0");
	dup_addTransaction(&l, 3131, "19/02/1640", "Raje1");
	dup_addTransaction(&l, 4141, "12/05/1522", "Raje2");
	dup_addTransaction(&l, 5151, "31/11/1955", "Raje3");
	dup_addTransaction(&l, 6161, "3/1/2010", "Raje4");
	import_data(&l, argv[1]);
	printf("after import_data\n");
	p=l.start;
	while(p){
		printf("%d,%d,%d/%d/%d,%s\n", p->amount, p->trans_ID, p->d.d, p->d.month, p->d.year, p->comment);
		p=p->next;
	}
	printf("*****\n\n");
	export_data(&l, argv[2]);
	return 0;
}
