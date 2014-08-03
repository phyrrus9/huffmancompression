//
//  binarytree.h
//  huff
//
//  Created by Ethan Laur on 7/29/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef huff_binarytree_h
#define huff_binarytree_h

struct tree_node
{
	struct tree_node *l, *r;
	float percent;
	unsigned char value, leaf;
};

typedef struct tree_node *binarytree;

binarytree tree_init(void);
struct tree_node *tree_node_init(void);
void free_node(struct tree_node *n);
struct tree_node *copy_node(struct tree_node *n);
void sortnodes(binarytree *nodes, int count);
void print_leafs_p(binarytree list, char *val);
void print_leafs(binarytree list);
unsigned short count_nodes(binarytree tree);

#endif
