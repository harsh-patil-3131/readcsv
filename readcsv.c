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
void import_csv(final *l, char *file){
	char line[50], ch;
	int i;
	FILE *fp;
	fp=fopen(file, "r");
	ch=fgetc(fp);
	while(ch!=EOF && !feof(fp)){
		i=0;
		node *nn=malloc(sizeof(node));
		if(l->start==NULL){
			l->start=nn;
			l->last=nn;
			nn->next=NULL;
		}
		else{
			l->last->next=nn;
			l->last=nn;
			nn->next=NULL;
		}
		while(ch!='\n' && ch!=EOF){
			line[i]=ch;\
			i++;
			ch=fgetc(fp);
		}
		line[i]='\0';
		printf("line: %s\n", line);
		nn->amount=atoi(strtok(line, ","));
		l->t_id++;
		nn->trans_ID=l->t_id;
		nn->d.d=atoi(strtok(NULL, "/"));
		nn->d.month=atoi(strtok(NULL, "/"));
		nn->d.year=atoi(strtok(NULL, ","));
		strcpy(nn->comment,strtok(NULL, "\0"));
		ch=fgetc(fp);
	}
	fclose(fp);
}

int main(int argc, char *argv[]){
	final l;
	node *p;
	init_fin(&l);
	//char *file="demo.csv";
	import_csv(&l, argv[1]);
	p=l.start;
	while(p){
		printf("%d,%d,%d/%d/%d,%s\n", p->amount, p->trans_ID, p->d.d, p->d.month, p->d.year, p->comment);
		p=p->next;
	}
	return 0;
}


