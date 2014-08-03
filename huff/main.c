//
//  main.c
//  huff
//
//  Created by Ethan Laur on 7/29/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "binarytree.h"
#include "compression.h"

int main(int argc, char * argv[])
{
	binarytree * datalist = NULL;
	unsigned char n, c;
	struct internal_compression_table tab;
	n = c = populatedatalist_file(&datalist, argv[1]);
	genhuffmantree(datalist, n);
	printf("%lu\n", sizeof(struct tree_node));
	print_leafs(datalist[0]);
	tab = genitable(*datalist, n);
	return 0;
}
