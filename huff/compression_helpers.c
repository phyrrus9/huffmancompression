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

void genhuffmantree(binarytree *datalist, unsigned char n)
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

struct compression_table gentable(binarytree datalist, unsigned char n)
{
	struct compression_table ret;
	struct tree_node *dll = NULL, *dlltmp = NULL, *datalisttmp = copy_node(datalist);
	unsigned char i;
	ret.table_data = malloc(sizeof(struct compression_table_entry) * n);
	BinaryTree2DoubleLinkedList(datalisttmp, &dll);
	for (dlltmp = dll, i = 0; i < n && dlltmp != NULL; dlltmp = dlltmp->r)
	{
		if (dlltmp->leaf)
		{
			ret.table_data[i].value = dlltmp->value;
			ret.table_data[i++].percent = dlltmp->percent;
		}
	}
	return ret;
}

struct internal_compression_table genitable(binarytree datalist, unsigned char n)
{
	struct internal_compression_table ret;
	struct compression_table table = gentable(datalist, n);
	char *tmp;
	unsigned char i;
	ret.table_data = malloc(sizeof(struct internal_compression_table_entry) * n);
	ret.table_count = n;
	for (i = 0; i < n; i++)
	{
		ret.table_data[i].value = table.table_data[i].value;
		tmp = gentablestr(datalist, table.table_data[i].value);
		ret.table_data[i].str = malloc(strlen(tmp) + 1);
		memset(ret.table_data[i].str, 0, strlen(tmp) + 1);
		strcpy(ret.table_data[i].str, tmp);
		free(tmp);
	}
	return ret;
}

void gentablestr_r(struct tree_node *node, unsigned char c, char *str)
{
	if (node->value != c && !found)
	{
		if (node->l != NULL)
		{
			char *strl = strdup(str);
			strcat(strl, "0");
			gentablestr_r(node->l, c, strl);
			if (node->l->value == c)
			{
				strcpy(str, strl);
				goto done;
			}
			/*else if (node->l->leaf)
				str[strlen(str) - 1] = 0;*/
		}
		if (node->r != NULL)
		{
			char *strr = strdup(str);
			strcat(strr, "1");
			gentablestr_r(node->r, c, strr);
			if (node->r->value == c)
			{
				strcpy(str, strr);
				goto done;
			}
			/*else if (node->r->leaf)
				str[strlen(str) - 1] = 0;*/
		}
	}
	else goto done;
	return;
done:
	found = 0x1;
}

char *gentablestr(binarytree datalist, unsigned char c)
{
	found = 0x0;
	char *tmp = malloc(256), *ret = NULL;
	memset(tmp, 0, 256);
	gentablestr_r(datalist, c, tmp);
	ret = malloc(strlen(tmp) + 1);
	memset(ret, 0, strlen(tmp) + 1);
	strcpy(ret, tmp);
	memset(tmp, 0, 256);
	free(tmp);
	return ret;
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
