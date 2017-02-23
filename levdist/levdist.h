/*
 * levdist.h
 *
 *  Created on: 2016/12/12
 *      Author: sin
 */

#ifndef SRC_LEVDIST_H_
#define SRC_LEVDIST_H_

#ifndef MAX
#define MAX(x, y)   ((x) < (y)? (y) : (x))
#endif
#ifndef MIN
#define MIN(x, y)   ((x) > (y)? (y) : (x))
#endif
#ifndef ABS
#define ABS(x)  ((x) < 0 ? -(x) : (x))
#endif //ABS

//long ceilpow2(const long val);
int clz64(unsigned int x)
{
	int n;
	union {
		unsigned long long as_uint64;
		double             as_double;
	} data;
	data.as_double = (double)x + 0.5;
	n = 1054 - (data.as_uint64 >> 52);
	return n;
}

int clz32(unsigned int x)
{
	int n;
	union {
		unsigned int as_uint32;
		float        as_float;
	} data;

	data.as_float = (float)x + 0.5;
	n = 158 - (data.as_uint32 >> 23);
	return n;
}

/*
int
nlz(int x)
{
int c = 0;
if (x == 0) return 32;
if (x & 0xffff0000) { x &= 0xffff0000; c |= 0x10; }
if (x & 0xff00ff00) { x &= 0xff00ff00; c |= 0x08; }
if (x & 0xf0f0f0f0) { x &= 0xf0f0f0f0; c |= 0x04; }
if (x & 0xcccccccc) { x &= 0xcccccccc; c |= 0x02; }
if (x & 0xaaaaaaaa) { c |= 0x01; }
return c ^ 31;
}
*/
long r_edist(char s[], int m, char t[], int n);

void setframe(long * frame, const long n, const long m);
long dp_edist(long * frame, char t[], long n, char p[], long m);
long weaving_edist(long * frame, const char t[], const long n, const char p[], const long m);

#endif /* SRC_LEVDIST_H_ */
