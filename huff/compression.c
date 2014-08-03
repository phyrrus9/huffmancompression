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
#include <string.h>

binarytree * initdatalist(unsigned short n)
{
	binarytree *datalist = NULL;
	unsigned short i;
	datalist = malloc(sizeof(binarytree) * (n + 5));
	for (i = 0; i < n; i++)
		datalist[i] = tree_node_init();
	return datalist;
}

unsigned short populatedatalist_file(binarytree **datalist, char *fname)
{
	FILE *f = fopen(fname, "rb");
	struct stat st;
	double tsize = 0.0;
	unsigned char found[256] = { 0 };
	unsigned short foundn = 0, c, d;
	double foundc[256] = { 0.0 };
	stat(fname, &st);
	tsize = st.st_size;
	while (foundn <= 256 && fread(&c, 1, 1, f) > 0)
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
	for (c = 0, d = 0; c <= 256; c++)
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

unsigned short populatedatalist_table(binarytree **datalist, struct compression_table table)
{
	unsigned short c, n = table.table_count;
	*datalist = initdatalist(n);
	for (c = 0; c < n; c++)
	{
		(*datalist)[c]->percent = table.table_data[c].percent;
		(*datalist)[c]->value = table.table_data[c].value;
		(*datalist)[c]->leaf = 0x1;
	}
	return n;
}

char dowrite(FILE *out, char *data)
{
	unsigned char byte, i;
	char *tmp = NULL;
	while (strlen(data) > 7)
	{
		/* do a byte conversion */
		for (i = 0, byte = 0; i < 8; i++)
			if (data[i] == '1')
				byte |= 1 << (7 - i);
		/* bit slide */
		tmp = strdup(&data[8]);
		strcpy(data, tmp);
		free(tmp);
		/* write buffer to file */
		fwrite(&byte, 1, 1, out);
	}
	return strlen(data) > 0;
}

void compress(FILE *in, FILE *out, binarytree compression)
{
	char *tmp = malloc(1026); //enough for a full double buffer + null terminator + 1 byte pad
	unsigned short tmpc, i, extra = 0, nnodes = count_nodes(compression);
	struct internal_compression_table table = genitable(compression, nnodes);
	struct compression_table otable = gentable(compression, nnodes);
	struct file_header head = { 256 - nnodes, 0};
	printf("\tBegin compression\n");
	memset(tmp, 0, 1026);
	printf("\tWriting header and compression table\n");
	fwrite(&head, sizeof(struct file_header), 1, out);
	for (i = 0; i < nnodes; i++)
		fwrite(&otable.table_data[i], sizeof(struct compression_table_entry), 1, out);
	printf("\t\tCompressing data\n");
	while (fread(&tmpc, 1, 1, in) > 0)
	{
		for (i = 0; table.table_data[i].value != tmpc && i <= 256; i++);
		strcat(tmp, table.table_data[i].str);
		dowrite(out, tmp);
	}
	printf("\t\tWriting waste\n");
	while (dowrite(out, tmp))
	{
		++extra;
		strcat(tmp, "0");
	}
	printf("\t\tUpdating header\n");
	fseek(out, 0L, SEEK_SET);
	head.wastebits = extra;
	fwrite(&head, sizeof(struct file_header), 1, out);
	fflush(out);
	free(tmp);
}
