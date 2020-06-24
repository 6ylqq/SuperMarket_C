/*********************************************************
 * From C PROGRAMMING: A MODERN APPROACH, Second Edition *
 * By K. N. King                                         *
 * Copyright (c) 2008, 1996 W. W. Norton & Company, Inc. *
 * All rights reserved.                                  *
 * This program may be freely distributed for class use, *
 * provided that this copyright notice is retained.      *
 *********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NAME_LEN 25
#define MAX_DISCOUNT_LEN 30

errno_t err;

typedef struct goods {
	int number;
	int price;
	char name[MAX_NAME_LEN + 1];
	char goods_discount[MAX_DISCOUNT_LEN + 1];
	int on_hand;
	struct goods *next;
}goodslist;
goodslist *inventory;   /* points to first good */


void insert(void);
void search(void);
void update(void);
void deletes(void);
void print(void);
void bubblesort(void);
void goodslist_init(void);
void save_to_file_exit(void);
void DestroyGoods(goodslist* inventory);

/**********************************************************
 * main: Prompts the user to enter an operation code,     *
 *       then calls a function to perform the requested   *
 *       action. Repeats until the user enters the        *
 *       command 'q'. Prints an error message if the user *
 *       enters an illegal code.                          *
 **********************************************************/
int main(void)
{
	int choice;
	goodslist_init();
	while(1) 
	{
		printf("Supermarket Goods Manage System\n");-
		printf("********************************************\n");
		printf("1. show all the goods:\n");
		printf("2. update one good:\n");
		printf("3. insert one good:\n");
		printf("4. delete one good:\n");
		printf("5. search one good:\n");
		printf("6. save and exit:\n");
		printf("7. bubble sort by price(optional question):\n");
		printf("********************************************\n");
			printf("enter your choice: ");
			scanf_s("%d", &choice,20);
			switch (choice)
			{
			case 1:print(); break;
			case 2:update(); break;
			case 3:insert(); break;
			case 4:deletes(); break;
			case 5:search(); break;
			case 6:save_to_file_exit(); break;
			case 7:bubblesort(); break;
			default:printf("Please enter a right choise!"); break;
			}
			printf("\n");
	}
}

/**********************************************************
 * find_part: Looks up a good number in the inventory     *
 *            list. Returns a pointer to the node         *
 *            containing the good number; if the good     *
 *            number is not found, returns NULL.          *
 **********************************************************/


/**********************************************************
 * goodslist_init: read goods info from the file named    *
 *            " XXXX.txt" .and readfile function will     *
 *            creat the goods list.the realization of     *
 *            readfile function is in myfileio.c .        *
 **********************************************************/
void goodslist_init(void)
{
	FILE *fp;
	int count = 0;
	goodslist *cur, *prev;
	inventory = NULL;
	prev = inventory;
	cur = NULL;
	
	if ((err = fopen_s(&fp,"goodsinfo.txt", "r")) != 0)
	{
		if ((err = fopen_s(&fp, "goodsinfo.txt", "w")) != 0)
		{
			printf("TIPS£ºCAN NOT CREAT GOODS DATABASE FILE \n");
			return;
		}
		else
		{
			printf("TIPS: now there are 0 items in list\n");
			printf("Please insert new one!\n");
		}
	}
	else {
		inventory = (goodslist*)malloc(sizeof(goodslist));
		fscanf_s(fp, "%d", &inventory->number,20);
		fscanf_s(fp, "       %s", inventory->name,20);
		fscanf_s(fp, "   %d", &inventory->price,20);
		fscanf_s(fp, "      %s", inventory->goods_discount,20);
		fscanf_s(fp, "      %d\n", &inventory->on_hand,20);
		inventory->next=NULL;
		while (!feof(fp))
		{
			cur = (goodslist*)malloc(sizeof(goodslist));
			for (prev = inventory;
				prev->next != NULL;
				prev = prev->next
				);
			if (cur == NULL) {
				printf("Database is full; can't add more goods.\n");
				return;
			}
			prev->next = cur;
			cur->next = NULL;
			fscanf_s(fp, "%d", &cur->number,20);
			fscanf_s(fp, "       %s",cur->name,20);
			fscanf_s(fp, "   %d", &cur->price,20);
			fscanf_s(fp, "      %s", cur->goods_discount,20);
			fscanf_s(fp, "      %d\n", &cur->on_hand,20);
			prev = cur;
			
			count++;
		}
		
		printf("TIPS: now there are %d items in list\n", count+1);
	}
	cur = NULL;
	prev = NULL;
	fclose(fp);
	fp = NULL;

}
/**********************************************************
 * save_to_file_exit: info_flush save list items into     *
 *            the file" XXXX.txt" .and free memory of     *
 *            will creat the goods list.the realization of*
 *            info_flush function is in myfileio.c .      *
 **********************************************************/
void save_to_file_exit(void)
{
	goodslist *cur, *prev;
	int savecount = 0;
	FILE *fp;
	if ((err = fopen_s(&fp, "newgoodsinfo.txt", "w")) != 0)
	{
		printf("TIPS£ºcan not open goods file database!\n");
		return;
	}
	for (cur = inventory, prev = NULL;
		cur != NULL;
		prev = cur, cur = cur->next)
	{

		fprintf(fp, "%d", cur->number);
		fprintf(fp, "       %s", cur->name);
		fprintf(fp, "   %d",cur->price);
		fprintf(fp, "      %s", cur->goods_discount);
		fprintf(fp, "      %d\n",cur->on_hand);
		savecount++;

	}
	fclose(fp);
	fp = NULL;
	DestroyGoods(inventory);
	cur = NULL;
	prev = NULL;
	printf("TIPS£º%d items save into newgoodsinfo.txt \n", savecount );
}

/**********************************************************
 * insert: Prompts the user for information about a new   *
 *         good and then inserts the good into the        *
 *         inventory list; the list remains sorted by     *
 *         good number. Prints an error message and       *
 *         returns prematurely if the good already exists *
 *         or space could not be allocated for the good.  *
 **********************************************************/
void insert(void)
{
	goodslist *current, *pre,*check;
	
	pre = inventory;
	
	
		current = (goodslist*)malloc(sizeof(goodslist));
		if (pre == NULL)
		{
			inventory = current;
			
		}
		else 
		{
			while (pre->next != NULL)
				pre = pre->next;
			pre->next = current;
		}
		current->next = NULL;
		printf("Enter Good number:");
		scanf_s("%d", &current->number,20);
		
		for (check = inventory;check->next != NULL ;check = check->next)
		{
			if (check->number == current->number)
			{
				printf("Good already exists.\n");
				check = NULL;
				free(current);
				current = NULL;
				pre->next = NULL;
				return;
			}
		}
		check = NULL;
		
		printf("Enter good name:");
		scanf_s("%s", current->name,20);
		printf("Enter good price:");
		scanf_s("%d", &current->price,20);
		printf("Enter good discount:");
		scanf_s("%s", current->goods_discount,20);
		printf("Enter quantity on hand:");
		scanf_s("%d", &current->on_hand,20);
		print();
		current = NULL;
		pre = NULL;
}

/**********************************************************
 * search: Prompts the user to enter a good number, then  *
 *         looks up the good in the database. If the good *
 *         exists, prints all the information of the good *
 *         if not, prints an error message.               *
 **********************************************************/
void search(void)
{
	goodslist *prev;
	printf("Enter good number:");
	int i;
	scanf_s("%d", &i,20);
	for (prev=inventory;
		prev->number != i&&prev->next!=NULL;
		prev = prev->next);
	if (prev->next == NULL)
		if (prev->number != i)
		{
			printf("This good isn't isist!");
			return;
		}
	printf("Good Number   Good Name                   Good Price   Good Discount   "
		"Quantity on Hand\n");
	printf("%7d       %-25s   %7d      %7s      %11d\n", prev->number, prev->name, prev->price, prev->goods_discount,
		prev->on_hand);
	prev = NULL;
}

/**********************************************************
 * update: Prompts the user to enter a good number.       *
 *         Prints an error message if the good doesn't    *
 *         exist; otherwise, prompts the user to enter    *
 *         all the new information of the good            *
 **********************************************************/
void update(void)
{
	goodslist *p;
	p = inventory;
	printf("Enter good number:");
	int i;
	scanf_s("%d", &i);
	
	for (; p->number != i&&p->next!=NULL; p = p->next);
	if (p->next == NULL)
		if (p->number != i)
		{
			printf("This good is not exist!");
			return;
		}
	printf("Enter new good imformation:\n");
	printf("Enter good name:");
	scanf_s("%s", p->name,20);
	printf("Enter good price:");
	scanf_s("%d", &p->price,20);
	printf("Enter good discount:");
	scanf_s("%s", p->goods_discount,20);
	printf("Enter quantity on hand:");
	scanf_s("%d",&p->on_hand,20);
	p = NULL;
	print();
}

/**********************************************************
 * delete: Prompts the user to enter a part number.       *
		   delete the item in list, free the memory       *
 **********************************************************/
void deletes(void)
{
	goodslist *p,*f;
	printf("Enter good number:");
	int i;
	scanf_s("%d", &i,20);
	f = inventory;
	for (p = inventory; 
		p->number != i&&p->next!=NULL;
		f=p, p = p->next);
	if (p->next == NULL)
		if(p->number!=i)
	{
		printf("This good is not exist!");
		return;
	}
	printf("Delete Good Name:%s", p->name);
	if (p == inventory)
		inventory = p->next;
	else
	{
		f->next = p->next;
	}
	free(p);
	f = NULL;
	p = NULL;
}
/**********************************************************
 * bubblesort: Prompts the user to enter a choice, then   *
 *         sort by price or number. finally show the list *
 **********************************************************/
void bubblesort(void)
{
	printf("sort by price(1) or number(2):");
	int i;
	scanf_s("%d", &i, 20);
	goodslist *pre, *cur,*next,*end, *temp,*head;
	head = (goodslist*)malloc(sizeof(goodslist));
	head->next = inventory;
	end = NULL;
	if (i == 1)
	{
		while (head->next->next != end)
		{
			pre = head;
			cur = pre->next;
			next = cur->next;
			while (next != end)
			{
				if (cur->price > next->price)
				{
					cur->next = next->next;
					pre->next = next;
					next->next = cur;
					temp = next;
					next = cur;
					cur = temp;
				}
				pre = pre->next;
				cur = cur->next;
				next = next->next;
			}
			end = cur;
		}
		inventory = head->next;
	}
	else {
		while (head->next->next != end)
		{
			pre = head;
			cur = pre->next;
			next = cur->next;
			while (next != end)
			{
				if (cur->number > next->number)
				{
					cur->next = next->next;
					pre->next = next;
					next->next = cur;
					temp = next;
					next = cur;
					cur = temp;
				}
				pre = pre->next;
				cur = cur->next;
				next = next->next;
			}
			end = cur;
		}
		inventory = head->next;
	}
	print();
	pre = NULL;
	cur = NULL; next = NULL; end = NULL; temp = NULL; head = NULL;
}

/**********************************************************
 * print: Prints a listing of all parts in the database,  *
 *        showing the part number, part name, and         *
 *        quantity on hand. Part numbers will appear in   *
 *        ascending order.                                *
 **********************************************************/
void print(void)
{
	goodslist *p;
	p = inventory;
	printf("Good Number   Good Name                   Good Price   Good Discount   "
		"Quantity on Hand\n");
	for (; p->next != NULL; p = p->next)
		printf("%7d       %-25s   %7d      %7s      %11d\n", p->number, p->name, p->price, p->goods_discount,
			p->on_hand);
	if(p->next==NULL)
		printf("%7d       %-25s   %7d      %7s      %11d\n", p->number, p->name, p->price, p->goods_discount,
			p->on_hand);
	p = NULL;
}



/**********************************************************
 * from here is the part of file function                 *
 **********************************************************/
void DestroyGoods(goodslist* inventory) {
	if (!inventory) return; 
	goodslist *current, *prev;
	current = inventory;
	while (current->next!=NULL)
	{
		prev = current;
		current = current->next;
		free(prev);
	}
	prev = NULL;
	free(current);
	current = NULL;
}
