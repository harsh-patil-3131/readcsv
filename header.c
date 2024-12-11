#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

// initializing final structure
void init_fin(final *l)
{
	l->t_id = 00;
	l->start = NULL;
	l->last = NULL;
	return;
}

// reverses string(str[])
void reverse(char str[], int len)
{
	int i = 0;
	char ch;
	while (i < len / 2)
	{
		ch = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = ch;
		i++;
	}
}

// converts string to integer(same as that of itoa)
char *sitoa(int a, char *buffer)
{
	int i = 0, rem;
	if (a == 0)
	{
		buffer[0] = '0';
		buffer[1] = '\0';
		return buffer;
	}
	while (a != 0)
	{
		rem = a % 10;
		buffer[i++] = rem + '0';
		a = a / 10;
	}
	buffer[i] = '\0';
	reverse(buffer, i);
	return buffer;
}

// finds category of spending of given integer
char *getcat(int i)
{
	char *categories[] = {"Food", "Entertain", "Travel", "Education", "Investment", "Others"};
	return categories[i];
}

// Adds given values to given node
void add_in_node(node *nn, int amt, int t_id, char *dt, int cate, char *cmt)
{
	char xyz[20];
	strcpy(xyz, dt);
	nn->amount = amt;
	nn->trans_ID = t_id;
	nn->d.d = atoi(strtok(xyz, "/"));
	nn->d.month = atoi(strtok(NULL, "/"));
	nn->d.year = atoi(strtok(NULL, "\0"));
	nn->cate = cate;
	strcpy(nn->comment, cmt);
}

// compares date d(structure) and dt(string)
int cmp(date d, char dt[])
{
	char xyz[20];
	strcpy(xyz, dt);
	if (((10000 * (d.year)) + (100 * (d.month)) + d.d) < atoi(strtok(xyz, "/")) + (100 * atoi(strtok(NULL, "/"))) + (10000 * atoi(strtok(NULL, "\0"))))
	{
		return 1;
	}
	return 0;
}

// compares date d(structure) and dt(string)
int cmp2(date d, char dt[])
{
	char xyz[20];
	strcpy(xyz, dt);
	if (((10000 * (d.year)) + (100 * (d.month)) + d.d) <= atoi(strtok(xyz, "/")) + (100 * atoi(strtok(NULL, "/"))) + (10000 * atoi(strtok(NULL, "\0"))))
	{
		return 1;
	}
	return 0;
}

// adds transaction in linked list such that sorted w.r.t date
node *addTransaction(final *l, int amt, char dt[], int cate, char *cmt)
{
	node *nn = malloc(sizeof(node));
	if (nn == NULL)
	{
		printf("nn==NULL\n");
		return nn;
	}
	if (l->start == NULL)
	{
		l->start = nn;
		l->last = nn;
		nn->next = NULL;
		l->t_id++;
		add_in_node(nn, amt, l->t_id, dt, cate, cmt);
	}
	else
	{
		node *p = l->start, *q;
		if (cmp(p->d, dt) == 0)
		{
			nn->next = l->start;
			l->start = nn;
			l->t_id++;
			add_in_node(nn, amt, l->t_id, dt, cate, cmt);
			return nn;
		}
		while (p && cmp(p->d, dt))
		{
			q = p;
			p = p->next;
		}
		if (p == NULL)
		{
			q->next = nn;
			nn->next = NULL;
			l->t_id++;
			add_in_node(nn, amt, l->t_id, dt, cate, cmt);
			return nn;
		}
		q->next = nn;
		nn->next = p;
		l->t_id++;
		add_in_node(nn, amt, l->t_id, dt, cate, cmt);
	}
	return nn;
}

// Stores given file into Linked list in sorted manner w.r.t date
void import_data(final *l, char *file)
{
	char line[50], ch, b[12], c[20];
	int i, a, d;
	FILE *fp;
	fp = fopen(file, "r");
	if (!fp)
	{
		printf("ERROR: %s does not exist\n", file);
		return;
	}
	ch = fgetc(fp);
	while (ch != EOF && !feof(fp))
	{
		i = 0;
		while (ch != '\n' && ch != EOF)
		{
			line[i] = ch;
			i++;
			ch = fgetc(fp);
		}
		line[i] = '\0';
		// printf("line: %s\n", line);
		a = atoi(strtok(line, ","));
		strcpy(b, strtok(NULL, ","));

		d = atoi(strtok(NULL, ","));
		strcpy(c, strtok(NULL, "\0"));

		addTransaction(l, a, b, d, c);
		ch = fgetc(fp);
	}
	printf("Data imported Successfully\n");
	fclose(fp);
}

// exports data present in linked list in given file
void export_data(final *l, char *file)
{
	FILE *fp;
	char buffer[20];
	node *p = l->start;
	fp = fopen(file, "w");

	while (p)
	{
		fputs(sitoa(p->amount, buffer), fp);
		fputc(',', fp);
		fputs(sitoa(p->d.d, buffer), fp);
		fputc('/', fp);
		fputs(sitoa(p->d.month, buffer), fp);
		fputc('/', fp);
		fputs(sitoa(p->d.year, buffer), fp);
		fputc(',', fp);
		fputs(sitoa(p->cate, buffer), fp);
		fputc(',', fp);
		fputs(p->comment, fp);
		fputc('\n', fp);
		p = p->next;
	}
	fclose(fp);
}

// Prints information of transaction between given dates
void viewTransactions(final *l, char dt1[], char dt2[])
{
	node *p = l->start;
	while (p && cmp(p->d, dt1))
	{
		p = p->next;
	}
	if (p == NULL)
	{
		printf("no transaction present\n");
		return;
	}
	printf("__________________________________________________________________________\n");
	printf("|    Date     | Tran_id | Amount  |    Category    |       Comment       |\n");
	printf("|-------------|---------|---------|----------------|---------------------|\n");
	while (p && cmp2(p->d, dt2))
	{
		printf("| %02d/%02d/%4d  |   %3d   | %7d | %-14s | %-19s |\n",
			   p->d.d, p->d.month, p->d.year, // Date
			   p->trans_ID,					  // Transaction ID
			   p->amount,					  // Amount
			   getcat(p->cate-1),				  // Category (limit to 14 characters)
			   p->comment);
		p = p->next;
	}
	printf("|________________________________________________________________________|\n");
}

// Updates the Transaction Information
void updateTransaction(final *l, int id, char dt[], int amount, int cate, char cmt[])
{
	node *p = l->start, *q;
	if (p == NULL)
	{
		l->start = addTransaction(l, amount, dt, cate, cmt);
		return;
	}
	if (p->trans_ID == id)
	{
		l->start = p->next;
		free(p);
		p = addTransaction(l, amount, dt, cate, cmt);
		p->trans_ID = id;
		l->t_id--;
		return;
	}
	while (p && (p->trans_ID != id))
	{
		q = p;
		p = p->next;
	}
	if (p == NULL)
	{
		printf("transaction with id=%d not present\n", id);
		return;
	}

	q->next = p->next;
	free(p);
	p = addTransaction(l, amount, dt, cate, cmt);
	p->trans_ID = id;
	l->t_id--;
}

// Deletes Transaction Permanently of transaction id "id"
void deleteTransaction(final *l, int id)
{
	node *p = l->start, *q;
	if (p->trans_ID == id)
	{
		l->start = l->start->next;
		free(p);
		return;
	}
	while (p && (p->trans_ID != id))
	{
		q = p;
		p = p->next;
	}
	if (p == NULL)
	{
		printf("No id formed\n");
		return;
	}
	q->next = p->next;
	free(p);
}

// Prints information of transactions of given Category
void generateReport(final *l, int reportType)
{
	node *p = l->start;

	printf("__________________________________________________________________________\n");
	printf("|    Date     | Tran_id | Amount  |    Category    |       Comment       |\n");
	printf("|-------------|---------|---------|----------------|---------------------|\n");

	while (p)
	{
		if (p->cate == reportType)
		{
			printf("| %02d/%02d/%4d  |   %3d   | %7d | %-14s | %-19s |\n",
				   p->d.d, p->d.month, p->d.year, // Date
				   p->trans_ID,					  // Transaction ID
				   p->amount,					  // Amount
				   getcat(p->cate-1),				  // Category (limit to 14 characters)
				   p->comment);
		}
		p = p->next;
	}
	printf("|________________________________________________________________________|\n");
	return;
}

// Prints all the Transaction in Linked List
void printall(final *l)
{
	node *p = l->start;

	// Print header with properly aligned column names
	printf("__________________________________________________________________________\n");
	printf("|    Date     | Tran_id | Amount  |    Category    |       Comment       |\n");
	printf("|-------------|---------|---------|----------------|---------------------|\n");

	// Print data rows
	while (p)
	{
		printf("| %02d/%02d/%4d  |   %3d   | %7d | %-14s | %-19s |\n",
			   p->d.d, p->d.month, p->d.year, // Date
			   p->trans_ID,					  // Transaction ID
			   p->amount,					  // Amount
			   getcat(p->cate-1),				  // Category (limit to 14 characters)
			   p->comment);					  // Comment (limit to 19 characters)
		p = p->next;
	}

	printf("|________________________________________________________________________|\n");
}

// Prints Menu of the Program
void printmenu()
{
	printf("\t\t11)View All transactions\t 21)Add Transaction\t31)Import Data\n");
	printf("\t\t12)Generate Report\t\t 22)Delete Transaction\t32)Export Data\n");
	printf("\t\t13)Generate Bar chart\t\t 23)Update Transaction\n");
	printf("\t\t14)View specific transactions    24)Clear Transactions\n");
	printf("\t\t15)Search through comment\t 25)Total Spending\n");
	printf("\t\t10)EXIT\n");
}

// Generates the relative visual Bar Chart of all categories
void generateSpendingBarChart(final *l)
{
	// Array to store total spending for each category
	int categoryTotals[6] = {0, 0, 0, 0, 0, 0};
	char *categories[] = {"Food", "Entertain", "Travel", "Education", "Investment", "Others"};

	// Traverse the linked list and calculate totals for each category
	node *p = l->start;
	while (p != NULL)
	{
		categoryTotals[p->cate - 1] += p->amount;
		p = p->next;
	}

	// Find the maximum total for scaling the bar chart
	int maxTotal = 0;
	for (int i = 0; i < 6; i++)
	{
		if (categoryTotals[i] > maxTotal)
		{
			maxTotal = categoryTotals[i];
		}
	}

	// If there's no spending, display a message and return
	if (maxTotal == 0)
	{
		printf("No transactions to display.\n");
		return;
	}

	// Display the bar chart
	printf("Spending by Category:\n");
	for (int i = 0; i < 6; i++)
	{
		// Calculate bar length relative to the maximum total
		int barLength = (categoryTotals[i] * 80) / maxTotal; // Scale to 80 characters max
		printf("%-12s: ", categories[i]);

		for (int j = 0; j < barLength; j++)
		{
			printf("#");
		}

		printf(" (%d)\n", categoryTotals[i]); // Print the total spending for the category
	}
}

// Searches all the Transactions from their comment
void searchTransactionByComment(final *l, char *keyword)
{

	node *p = l->start;
	int found = 0;

	printf("__________________________________________________________________________\n");
	printf("|    Date     | Tran_id | Amount  |    Category    |       Comment       |\n");
	printf("|-------------|---------|---------|----------------|---------------------|\n");

	while (p)
	{
		if (strstr(p->comment, keyword) != NULL)
		{ // Check if the keyword exists in the comment
			printf("| %02d/%02d/%4d  |   %3d   | %7d | %-14s | %-19s |\n",
				   p->d.d, p->d.month, p->d.year, // Date
				   p->trans_ID,					  // Transaction ID
				   p->amount,					  // Amount
				   getcat(p->cate-1),				  // Category (limit to 14 characters)
				   p->comment);
			found = 1;
		}
		p = p->next;
	}

	printf("|________________________________________________________________________|\n");

	printf("\tTotal %d matches found\n", found);
}

// cleares all the transactions present
void clearAllTransactions(final *l)
{
	node *p, *q;
	p = l->start;

	while (p)
	{
		q = p;
		p = p->next;
		free(q);
	}

	l->start = NULL;
	l->last = NULL;
	l->t_id = 0;

	printf("All Transactions Cleared\n");
}

// Calculate Total Spending
int calculateTotalSpending(final *l, int category)
{
	node *p = l->start;
	int total = 0;

	while (p)
	{
		if (category == 0 || p->cate == category)
		{
			total += p->amount;
		}
		p = p->next;
	}

	return total;
}