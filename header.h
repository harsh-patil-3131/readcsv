#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure for storing date
typedef struct date
{
	int d;
	int month;
	int year;
} date;

// Node of linked list
typedef struct node
{
	int amount;
	int trans_ID;
	date d;
	int cate;
	char comment[20];
	struct node *next;
} node;

// structure containing address of first node
typedef struct final
{
	int t_id;
	node *start;
	node *last;
} final;

// Functions

void init_fin(final *l);

void reverse(char str[], int len);

char *sitoa(int a, char *buffer);

char *getcat(int i);

void add_in_node(node *nn, int amt, int t_id, char *dt, int cate, char *cmt);

int cmp(date d, char dt[]);

int cmp2(date d, char dt[]);

node *addTransaction(final *l, int amt, char dt[], int cate, char *cmt);

void import_data(final *l, char *file);

void export_data(final *l, char *file);

void viewTransactions(final *l, char dt1[], char dt2[]);

void updateTransaction(final *l, int id, char dt[], int amount, int cate, char cmt[]);

void deleteTransaction(final *l, int id);

void generateReport(final *l, int reportType);

void printall(final *l);

void printmenu();

void generateSpendingBarChart(final *l);

void searchTransactionByComment(final *l, char *keyword);

void clearAllTransactions(final *l);

int calculateTotalSpending(final *l, int category);