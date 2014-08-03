//
//  compression_helpers.c
//  huff
//
//  Created by Ethan Laur on 8/2/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include "compression.h"
#include <stdlib.h>
#include <string.h>

unsigned char found;

void genhuffmantree(binarytree *datalist, unsigned short n)
{
	sortnodes(datalist, n);
	while (n > 1)
	{
		binarytree temp = tree_init();
		temp->l = datalist[0];
		temp->r = datalist[1];
		temp->percent = temp->l->percent + temp->r->percent;
		datalist[0] = temp;
		n -= 1;
		datalist[1] = copy_node(datalist[n]);
		free(datalist[n]);
		datalist[n] = NULL;
		sortnodes(datalist, n);
	}
}

struct compression_table gentable(binarytree datalist, unsigned short n)
{
	struct compression_table ret;
	struct tree_node *dll = NULL, *dlltmp = NULL, *datalisttmp = copy_node(datalist);
	unsigned short i;
	ret.table_data = malloc(sizeof(struct compression_table_entry) * n);
	BinaryTree2DoubleLinkedList(datalisttmp, &dll);
	for (dlltmp = dll, i = 0; i < n && dlltmp != NULL; dlltmp = dlltmp->r)
	{
		if (dlltmp->leaf)
		{
			ret.table_data[i].value = dlltmp->value;
			ret.table_data[i++].percent = (unsigned short)(dlltmp->percent * 1000);
		}
	}
	//free_node(datalisttmp);
	return ret;
}

struct internal_compression_table genitable(binarytree datalist, unsigned short n)
{
	struct internal_compression_table ret;
	unsigned short i;
	ret.table_data = malloc(sizeof(struct internal_compression_table_entry) * n);
	ret.table_count = n;
	for (i = 0; i < n; i++)
		ret.table_data[i].str = NULL;
	assign_leafs(datalist, &ret);
	return ret;
}

void assign_leafs_p(binarytree list, char *val, struct internal_compression_table *tab)
{
	unsigned short i;
	if (list->leaf)
	{
		for (i = 0; tab->table_data[i].str; i++);
		tab->table_data[i].str = malloc(strlen(val) + 1);
		strcpy(tab->table_data[i].str, val);
		tab->table_data[i].value = list->value;
	}
	else
	{
		if (list->l)
		{
			char *vall = strdup(val);
			strcat(vall, "0");
			assign_leafs_p(list->l, vall, tab);
			free(vall);
		}
		if (list->r)
		{
			char *valr = strdup(val);
			strcat(valr, "1");
			assign_leafs_p(list->r, valr, tab);
			free(valr);
		}
	}
}

void assign_leafs(binarytree list, struct internal_compression_table *tab)
{
	char *val = malloc(256);
	memset(val, 0, 256);
	assign_leafs_p(list, val, tab);
	free(val);
}

void BinaryTree2DoubleLinkedList(binarytree root, struct tree_node **head)
{
	if (root == NULL) return;
	static struct tree_node* prev = NULL;
	BinaryTree2DoubleLinkedList(root->l, head);
	if (prev == NULL)
		*head = root;
	else
	{
		root->l = prev;
		prev->r = root;
	}
	prev = root;
	BinaryTree2DoubleLinkedList(root->r, head);
}
