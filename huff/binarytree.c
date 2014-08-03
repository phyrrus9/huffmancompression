//
//  binarytree.c
//  huff
//
//  Created by Ethan Laur on 7/29/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include "binarytree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

binarytree tree_init(void)
{
	binarytree ret = NULL;
	ret = tree_node_init();
	return ret;
}

struct tree_node *tree_node_init(void)
{
	struct tree_node *ret = malloc(sizeof(struct tree_node));
	ret->l = ret->r = NULL;
	ret->percent = 0;
	ret->leaf = 0;
	ret->value = 0.0;
	return ret;
}

void free_node(struct tree_node *n)
{
	if (n == NULL)return;
	if (n->l != NULL)
		free_node(n->l);
	if (n->r != NULL)
		free_node(n->l);
	free(n);
	n = NULL;
}

struct tree_node *copy_node(struct tree_node *n)
{
	struct tree_node *ret = NULL;
	if (n == NULL) return NULL;
	ret = tree_node_init();
	if (n->l != NULL)
		ret->l = copy_node(n->l);
	if (n->r != NULL)
		ret->r = copy_node(n->r);
	ret->leaf = n->leaf;
	ret->percent = n->percent;
	ret->value = n->value;
	return ret;
}

void sortnodes(binarytree *nodes, int count)
{
	int i, j;
	binarytree temp = NULL;
	for (i = (count - 1); i > 0; i--)
		for (j = 1; j <= i; j++)
			if (nodes[j - 1]->percent > nodes[j]->percent)
			{
				temp = nodes[j - 1];
				nodes[j - 1] = nodes[j];
				nodes[j] = temp;
			}
}

void print_leafs_p(binarytree list, char *val)
{
	if (list->leaf)
		printf("Value: %x\t%.2f%%\t%s\n", list->value, list->percent, val);
	if (list->l)
	{
		char *vall = strdup(val);
		strcat(vall, "0");
		print_leafs_p(list->l, vall);
		free(vall);
	}
	if (list->r)
	{
		char *valr = strdup(val);
		strcat(valr, "1");
		print_leafs_p(list->r, valr);
		free(valr);
	}
}

void print_leafs(binarytree list)
{
	char *val = malloc(256);
	memset(val, 0, 256);
	print_leafs_p(list, val);
}
