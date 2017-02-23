/*
 * debug_table.c
 *
 *  Created on: 2017/01/03
 *      Author: sin
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug_table.h"

long * debug_table;

static const char grays[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

void show_table(long * frame, long * table, long n, long m) {
	long cellval;

	if (n > 1024 || m > 1024) {
		printf("\ntable seems being too big.\n");
		return;
	}
	// show DP table
	printf("\ntable contents:\n");
	for (long r = 0; r < m+1; r++) {
		for (long c = 0; c < n+1; c++) {
			if ( c == 0 ) {
				cellval = frame[m - r];
			} else if ( r == 0 ) {
				cellval = frame[m + c];
			} else {
				cellval = table[m*(c-1)+(r-1)];
			}
			cellval = ((n+m+1-cellval) * (strlen(grays)-1)) / (n+m+1);
			if ( c == 0 )
				//printf("%3ld|",cellval);
				printf("%c|", grays[cellval]);
			else
				//printf("%3ld ", cellval);
				printf("%c ", grays[cellval]);
		}
		printf("\n");
		if ( r == 0 ) {
			for (long c = 0; c < n+1; c++) {
				//printf("+---");
				printf("--");
			}
			printf("\n");
		}
		fflush(stdout);
	}
	printf("\n\n");
	fflush(stdout);
}

long compare_table(long * t0, long * t1, long n, long m) {
	long c, r;
	long count = 0;
	for (c = 0; c < n; c++) {
		for (r = 0; r < m; r++) {
			if (t0[m*c + r] != t1[m*c + r]) {
				count++;
				if ( count < 8 )
					printf("different @ %ld, %ld\n", c, r);
			}
		}
	}
	if (count > 0)
		printf("%ld different elements in table.\n", count);
	printf("\n");
	return count;
}

