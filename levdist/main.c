#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "levdist.h"
#include "stopwatch.h"
#include "textfromfile.h"

#include "debug_table.h"

#define MEGA_B 1048576UL
#define KILO_B 1024UL
#define STR_MAXLENGTH (64 * KILO_B)


int getargs(const int argc, const char * argv[], char * text, char * patt, long * n, long *m) {
	char tmp[STR_MAXLENGTH];
	if ( argc != 3 )
		return EXIT_FAILURE;

	text[STR_MAXLENGTH-1] = 0;
	patt[STR_MAXLENGTH-1] = 0;

	if ( textfromfile(argv[1], STR_MAXLENGTH, text) != 0
		|| textfromfile(argv[2], STR_MAXLENGTH, patt) != 0 ) {
		return EXIT_FAILURE;
	}
	*n = (text[STR_MAXLENGTH-1] == 0? strlen(text) : STR_MAXLENGTH);
	*m = (patt[STR_MAXLENGTH-1] == 0? strlen(patt) : STR_MAXLENGTH);
	if ( *n < *m ) {
		long t = *n;
		*n = *m;
		*m = t;
		strcpy(tmp,text);
		strcpy(text,patt);
		strcpy(patt,tmp);
	}

	if ( *n < 1000 && *m < 1000 )
		fprintf(stdout, "Input: \n%s \n(%lu), \n%s \n(%lu)\n\n", text, *n, patt, *m);
	else
		fprintf(stdout, "Input: (%lu), (%lu)\n\n", *n, *m);
	fflush(stdout);

	return 0;
}

int main (int argc, const char * argv[]) {
	char * text, *patt;
	long *frame;
	long m, n;
	long d;
	
	text = malloc(sizeof(char)*STR_MAXLENGTH);
	patt = malloc(sizeof(char)*STR_MAXLENGTH);
	if ( text == NULL || patt == NULL ) {
		fprintf(stderr, "malloc error.\n");
		fflush(stderr);
		goto exit_error;
	}

	if ( getargs(argc, argv, text, patt, &n, &m) != 0 )
		goto exit_error;

	frame = (long*)malloc(sizeof(long)*(m+n+1));
	setframe(frame, n, m);

	printf("frame:\n");
	for(int i = 0; i < m+n+1; i++)
		printf("%ld, ",frame[i]);
	printf("\n");

#ifdef DEBUG_TABLE
	debug_table = (long*) malloc(sizeof(long)*m*n);

	long * dp_table;
	dp_table = (long*) malloc(sizeof(long)*n*m);
#endif

	stopwatch sw;
	stopwatch_start(&sw);

	d = dp_edist(frame, text, n, patt, m);

	stopwatch_stop(&sw);

	printf("Edit distance (by Pure DP): %lu\n", d);
	printf("%lu sec %lu milli sec.\n", stopwatch_secs(&sw), stopwatch_millis(&sw));
	printf("\n");

#ifdef DEBUG_TABLE
	for(int c = 0; c < n; c++)
		for(int r = 0; r < m; r++)
			dp_table[m*c+r] = debug_table[m*c+r];
	show_table(frame, dp_table, n, m);

#endif


	fprintf(stdout, "computing edit distance by Waving DP.\n");
	fflush(stdout);

	stopwatch_start(&sw);

	d = weaving_edist(frame, text, n, patt, m);

	stopwatch_stop(&sw);

	printf("Edit distance (by Weaving DP): %ld\n", d);
	printf("%ld sec %ld milli sec.\n", stopwatch_secs(&sw), stopwatch_millis(&sw));

#ifdef DEBUG_TABLE
	setframe(frame, n, m);
	show_table(frame, debug_table, n, m);

	if ( compare_table(debug_table, dp_table, n, m) != 0) {
		printf("table compare failed.\n");
	} else {
		printf("two tables are identical.\n");
	}
	fflush(stdout);
	free(debug_table);
	free(dp_table);
#endif

	free(frame);
exit_error:
	free(text);
	free(patt);

    return 0;
}

