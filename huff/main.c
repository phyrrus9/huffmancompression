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
	FILE *in, *out;
	binarytree * datalist = NULL;
	unsigned short n;
	printf("Begin huffman encode\n"
		  "\tGathering data\n");
	n = populatedatalist_file(&datalist, argv[1]);
	printf("\tGenerating tree\n");
	genhuffmantree(datalist, n);
	printf("\tGetting files ready\n");
	in = fopen(argv[1], "rb");
	out = fopen(argv[2], "wb");
	compress(in, out, *datalist);
	printf("\tCleaning up\n");
	fclose(out);
	fclose(in);
	printf("Done\n");
	return 0;
}
