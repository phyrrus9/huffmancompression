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
	unsigned char ncompressed; //the header is followed by ncompressed
						  //struct compression_table_entry entries
	unsigned char wastebits;   //used to determine when to stop decompressing
						  //the number of extra zero bits at EOF
};

struct compression_table_entry
{
	unsigned char value;
	float         percent;
};

struct compression_table
{
	struct compression_table_entry *table_data;
	unsigned char                   table_count;
};

struct internal_compression_table_entry
{
	unsigned char value;
	char          *str;
};

struct internal_compression_table
{
	struct internal_compression_table_entry *table_data;
	unsigned char                            table_count;
};

typedef unsigned char wbits;
#define ftotalbits(fsize, waste) ((fsize - sizeof(wbits)) + waste)
void compress                              (FILE *in, FILE *out, binarytree compression);           //
void decompress                            (FILE *in, FILE *out);                                   //
binarytree * initdatalist                  (unsigned char n);                                       //√
unsigned char populatedatalist_file        (binarytree **datalist, char *fname);                    //√
unsigned char populatedatalist_table       (binarytree **datalist, struct compression_table table); //√
void genhuffmantree                        (binarytree *datalist, unsigned char n);                 //√
struct compression_table gentable          (binarytree datalist, unsigned char n);                  //
struct internal_compression_table genitable(binarytree datalist, unsigned char n);                  //√
char *gentablestr                          (binarytree datalist, unsigned char c);                  //√
void gentablestr_r                         (struct tree_node *node, unsigned char c, char *str);    //√
void BinaryTree2DoubleLinkedList           (binarytree root, struct tree_node **head);              //√

#endif
