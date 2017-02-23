/*
 * levdist.c
 *
 *  Created on: 2016/12/12
 *      Author: sin
 */
#include <stdio.h>
#include <stdlib.h>

#include "levdist.h"
#include "debug_table.h"

long r_edist(char s[], int m, char t[], int n) {
	long a, b, c;
	if (m == 0 && n == 0)
		return 0;
	if (m == 0)
		return n;
	if (n == 0)
		return m;
	a = r_edist(s, m, t, n-1) + 1;
	b = r_edist(s, m-1, t, n) + 1;
	c = r_edist(s, m-1, t, n-1) + ((s[m-1] == t[n-1])? 0 : 1);
	return (a < b ? (a < c ? a: c): (b < c ? b : c));
}

void setframe(long * frame, const long n, const long m) {
	for (long i = 0; i < n + m + 1; i++) {
		frame[i] = ABS(m - i);  // m (pattern, left) frame
	}
}

long dp_edist(long * frame, char t[], long n, char p[], long m) {
	long * dist;
	long col, row;
	long ins, del, repl, result;
	const int lcs_switch = 1;

	//printf("text %s (%d), pattern %s (%d)\n", t, n, p, m);

	dist = (long*) malloc(sizeof(long)*n*m);
	if ( dist == NULL )
		return n+m+1; // error

	// n -- the number of columns (the text length), m -- the number of rows (the pattern length)
	//table calcuration
	// let and top frames of the table corresponds to col = -1, row = -1
	for(col = 0; col < n; col++) { // column, text axis
		for (row = 0; row < m; row++) {  // row, pattern axis
			ins = ( col == 0 ? frame[(m-1)-row] : dist[row + m*(col-1)] ) + 1;
			del = ( row == 0 ? frame[m+1+col] : dist[(row-1) + m*col] ) + 1;
			repl = ( col == 0 || row == 0 ?
					( col == 0 ? frame[m-row] : frame[m+col] ) :
					dist[(row-1) + m*(col-1)] );
			//
			if ( t[col] == p[row] ) {
				if ( ins < del && ins < repl ) {
					repl = ins;
				} else if (del < ins && del < repl ) {
					repl = del;
				}
			} else {
				repl += 1;
				if ( ins <= del && (lcs_switch || ins < repl) ) {
					repl = ins;
				} else if (del < ins && (lcs_switch || del < repl) ) {
					repl = del;
				}
			}
			dist[row + m*col] = repl;
			//printf("[%ld,%ld] %c|%c : %ld/%ld/%ld+%d >%ld,\n", col, row, t[col], p[row], del,ins, repl, (t[col] != p[row]), dist[col*m+row]);
#ifdef DEBUG_TABLE
			debug_table[row+m*col] = dist[row+m*col];
#endif
		}
	}

	result = dist[n*m-1];
	free(dist);
	return result;
}

//#define SNAKE_HEADS
long weaving_edist(long * frame, const char t[], const long n, const char p[], const long m)
{
	long col, row;
	long del, ins, repl; // del = delete from pattern, downward; ins = insert to pattern, rightward
	long warp_start, warp_last;

	const int lcs_switch = 1;

#ifdef SNAKE_HEADS
	long snake[n+m+1];
	long tail;
	const long dist_max = 64;
	const long num_heads_max = dist_max * dist_max;
	struct {
		long warpix, head, tail;
	} snake_heads[num_heads_max];
	long num_heads = 0;
#endif // SNAKE_HEADS

	if (frame == NULL)
		return n+m+1;

#ifdef SNAKE_HEADS
	for(long i = 0; i < n+m+1; i++) {
		snake[i] = -1;
	}
#endif

	for (long depth = 0; depth < n + m - 1; depth++) {
		if ( depth < m ) {
			warp_start = m - depth;
		} else {
			warp_start = depth - (m - 2);
		}
		if (depth < n) {
			warp_last = depth + m;
		} else {
			warp_last = ((n-1) << 1) + m - depth;
		}
		//printf("depth %ld (%ld, %ld):\n", depth, warp_start, warp_last);
		for (long warpix = warp_start; warpix <= warp_last; warpix += 2) {
			/*
			if (warpix < 0 || warpix > n + m + 1) {
				printf("warp value error: %ld\n", warpix);
				//fflush(stdout);
			}
			*/
			col = (depth + warpix - m)>>1;
			row = (depth - warpix + m)>>1;

			//printf("%ld = (%ld, %ld), ", warpix, col, row);
			del = frame[warpix+1] + 1;
			ins = frame[warpix-1] + 1;
			repl = frame[warpix];
			//printf(" %ld, %ld (%ld) [%ld,%ld] %c|%c : %ld/%ld/%ld+%ld ",
			//		depth, warpix, warpix-m, col,row,t[col],p[row], del,ins, frame[warpix], (t[col] != p[row]));
			if ( t[col] == p[row] ) {
				if (del < ins && del < repl) {
					repl = del;
				} else if ( ins < del && ins < repl ) {
					repl = ins;
				}
			} else /* ( t[col] != p[row] ) */ {
				repl += 1;
				if (del <= ins && (lcs_switch || del < repl) ) {
					repl = del;
				} else if ( ins < del && (lcs_switch || ins < repl) ) {
					repl = ins;
				}
			}
			//printf("-> %ld\n", repl);
			//
#ifdef SNAKE_HEADS
			if ( ABS(warpix - ((m+n)>>1)) < (dist_max>>1) ) {
				if ( t[col] == p[row] ) {
					if ( snake[warpix] == -1 )
						snake[warpix] = depth; // start the snake
					// else
					//  continue the snake
				} else { // finish the snake on character mismatch
					if ( snake[warpix] != -1 ) {
						tail = snake[warpix];
						snake[warpix] = -1;
						if ( num_heads < num_heads_max ) {
							//printf("snake head %ld (%ld), %ld -> %ld\n", warpix, warpix-m, tail, depth);
							snake_heads[num_heads].head = depth;
							snake_heads[num_heads].tail = tail;
							snake_heads[num_heads].warpix = warpix;
							num_heads++;
						}
					}
				}
				if ( (t[col] == p[row]) &&
						((depth >= m && warpix == warp_start) || (depth >= n && warpix == warp_last)) ) {
					// finish the snake on the warp end
					if ( snake[warpix] != -1 ) {
						tail = snake[warpix];
						if ( num_heads < num_heads_max ) {
							//printf("snake head %ld (%ld), %ld -> %ld\n", warpix, warpix-m, tail, depth);
							snake_heads[num_heads].head = depth + 2;
							snake_heads[num_heads].tail = tail;
							snake_heads[num_heads].warpix = warpix;
							num_heads++;
						}
					}
				}
			}
#endif
			//
			frame[warpix] = repl;
#ifdef DEBUG_TABLE
			debug_table[m*col + row] = repl;
#endif
		}
#ifdef SNAKE_HEADS
		for(long i = 0; i < n+m+1; i ++) {
			if ( ((depth + i) & 1) == 0 && (warp_start <= i && i <= warp_last) )
				if ( snake[i] == -1 )
					printf(".");
				else
					printf("+");
			else
				printf(" ");
		}
		printf("\n");
#endif
	}

#ifdef SNAKE_HEADS
	for(long i = 0; i < num_heads; i ++) {
		printf("snake head %ld (%ld), [%ld, %ld)\n",
				snake_heads[i].warpix, snake_heads[i].warpix-m, snake_heads[i].tail, snake_heads[i].head);
	}
	printf("\n");
#endif

	return frame[n];
}
