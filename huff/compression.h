//
//  compression.h
//  huff
//
//  Created by Ethan Laur on 7/29/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#ifndef huff_compression_h
#define huff_compression_h

#include <sys/types.h>
#include <stdio.h>
#include "binarytree.h"

struct file_header
{
	unsigned char  nncompressed; //the header is followed by 256 - nncompressed
						    //struct compression_table_entry entries
	unsigned char  wastebits;    //used to determine when to stop decompressing
						    //the number of extra zero bits at EOF
};

struct compression_table_entry
{
	unsigned char value;
	unsigned short percent; //this is actually the percent
					    //rounded to the nearest thousandth
					    //and then multiplied by 1000
};

struct compression_table
{
	struct compression_table_entry *table_data;
	unsigned short                  table_count;
};

struct internal_compression_table_entry
{
	unsigned char value;
	char          *str;
};

struct internal_compression_table
{
	struct internal_compression_table_entry *table_data;
	unsigned short                           table_count;
};

typedef unsigned char wbits;
#define ftotalbits(fsize, waste) ((fsize - sizeof(wbits)) + waste)
void compress                              (FILE *in, FILE *out, binarytree compression);            //√
void decompress                            (FILE *in, FILE *out);                                    //
binarytree * initdatalist                  (unsigned short n);                                       //√
unsigned short populatedatalist_file       (binarytree **datalist, char *fname);                     //√
unsigned short populatedatalist_table      (binarytree **datalist, struct compression_table table);  //√
void genhuffmantree                        (binarytree *datalist, unsigned short n);                 //√
struct compression_table gentable          (binarytree datalist, unsigned short n);                  //√
struct internal_compression_table genitable(binarytree datalist, unsigned short n);                  //√
void BinaryTree2DoubleLinkedList           (binarytree root, struct tree_node **head);               //√
void assign_leafs(binarytree list, struct internal_compression_table *tab);

#endif
