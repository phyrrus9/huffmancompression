//
//  compression.c
//  huff
//
//  Created by Ethan Laur on 7/29/14.
//  Copyright (c) 2014 Ethan Laur. All rights reserved.
//

#include "compression.h"
#include <sys/stat.h>
#include <stdlib.h>

binarytree * initdatalist(unsigned char n)
{
	binarytree *datalist = NULL;
	unsigned char i;
	datalist = malloc(sizeof(binarytree) * n);
	for (i = 0; i < n; i++)
		datalist[i] = tree_node_init();
	return datalist;
}

unsigned char populatedatalist_file(binarytree **datalist, char *fname)
{
	FILE *f = fopen(fname, "rb");
	struct stat st;
	double tsize = 0.0;
	unsigned char found[0xff] = { 0 }, foundn = 0, c, d;
	double foundc[0xff] = { 0.0 };
	stat(fname, &st);
	tsize = st.st_size;
	while (foundn < 0xff && fread(&c, 1, 1, f) > 0)
	{
		foundc[c]++;
		if (found[c] == 0)
		{
			found[c] = 1;
			foundn++;
		}
	}
	fclose(f);
	*datalist = initdatalist(foundn);
	for (c = 0, d = 0; c < 0xff && d < foundn; c++)
	{
		if (found[c])
		{
			(*datalist)[d]->percent = (foundc[c] / tsize) * 100;
			(*datalist)[d]->value = c;
			(*datalist)[d]->leaf = 0x1;
			d++;
		}
	}
	return foundn;
}

unsigned char populatedatalist_table(binarytree **datalist, struct compression_table table)
{
	unsigned char c, n = table.table_count;
	*datalist = initdatalist(n);
	for (c = 0; c < n; c++)
	{
		(*datalist)[c]->percent = table.table_data[c].percent;
		(*datalist)[c]->value = table.table_data[c].value;
		(*datalist)[c]->leaf = 0x1;
	}
	return n;
}
