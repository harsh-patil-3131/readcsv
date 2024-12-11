#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

int main()
{
	final l;
	init_fin(&l);
	int choice = 0, amount, cate, id;
	char file[20], date[12], date2[12], comment[20];
	do
	{
		if (choice == 1)
		{
			printf(">>>");
		}
		else
		{
			printf("(1 for Menu)>>>");
		}
		scanf("%d", &choice);
		switch (choice)
		{

		case 1:
			printmenu();
			break;

		case 11:
			printall(&l);
			break;

		case 12:
			printf("\t1:Food     \t2:Entertain \t3:Travel\n");
			printf("\t4:Education\t5:Investment\t6:Others\n");
			printf(">>>Category of Report: ");
			scanf("%d", &cate);
			if (cate >= 1 && cate <= 6)
			{
				generateReport(&l, cate);
			}
			else
			{
				printf("Invalid Category\n");
			}
			break;

		case 13:
			// printall(&l);
			generateSpendingBarChart(&l);
			break;

		case 14:
			// viewTransactions(final *l, char dt1[], char dt2[]);
			printf(">>>Give Initial date in format DD/MM/YYYY: ");
			scanf("%s", date);
			printf(">>>Give Final date in format DD/MM/YYYY: ");
			scanf("%s", date2);
			viewTransactions(&l, date, date2);
			break;

		case 15:
			printf(">>>Give string to be searched: ");
			scanf("%s", comment);
			searchTransactionByComment(&l, comment);
			break;

		case 21:
			printf(">>>Amount of new transaction: ");
			scanf("%d", &amount);
			printf(">>>Date of transaction in format DD/MM/YYYY: ");
			scanf("%s", date);
			printf("\t1:Food     \t2:Entertain \t3:Travel\n");
			printf("\t4:Education\t5:Investment\t6:Others\n");
			printf(">>>Category of spending: ");
			scanf("%d", &cate);
			printf(">>>comment on transaction: ");
			scanf("%s", comment);
			addTransaction(&l, amount, date, cate, comment);
			break;

		case 22:
			printf(">>>Transaction of ID to be deleted: ");
			scanf("%d", &id);
			deleteTransaction(&l, id);
			break;

		case 23:
			// updateTransaction(final *l, int id, char dt[], int amount, int cate, char cmt[]);
			printf(">>>Previous Transaction ID: ");
			scanf("%d", &id);
			printf(">>>Amount of new transaction: ");
			scanf("%d", &amount);
			printf(">>>Date of transaction in format DD/MM/YYYY: ");
			scanf("%s", date);
			printf("\t1:Food     \t2:Entertain \t3:Travel\n");
			printf("\t4:Education\t5:Investment\t6:Others\n");
			printf(">>>Category of spending: ");
			scanf("%d", &cate);
			printf(">>>comment on transaction: ");
			scanf("%s", comment);
			updateTransaction(&l, id, date, amount, cate, comment);
			break;
		
		case 24:
			clearAllTransactions(&l);
			break;

		case 25:
			printf("\t0 for all\n");
			printf("\t1:Food     \t2:Entertain \t3:Travel\n");
			printf("\t4:Education\t5:Investment\t6:Others\n");
			printf(">>>Category of spending: ");
			scanf("%d", &cate);
			if(cate==0){
				printf(">>>Total spending in all categories: %d\n", calculateTotalSpending(&l, cate));
			}
			else{
    			printf(">>>Total spending in \"%s\" category: %d\n",getcat(cate-1), calculateTotalSpending(&l, cate));
   			}
			break;
		
		case 31:
			printf(">>>Give file name to be imported: ");
			scanf("%s", file);
			import_data(&l, file);
			printf(">>>Data imported Successfully\n");
			break;

		case 32:
			printf(">>>Give file name to be exported: ");
			scanf("%s", file);
			export_data(&l, file);
			printf(">>>Data exported Successfully\n");
			break;

		case 10:
			exit(1);
			break;

		default:
			printf(">>>Wrong choice");
			break;
		}
	} while (choice != 10);

	return 0;
}