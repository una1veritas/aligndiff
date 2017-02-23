/*
 * levdist.h
 *
 *  Created on: 2016/12/12
 *      Author: sin
 */

#ifndef SRC_LEVDIST_H_
#define SRC_LEVDIST_H_

void setframe(long * frame, const long n, const long m);
long dp_edist(long * frame, char t[], long n, char p[], long m);
long weaving_edist(long * frame, const char t[], const long n, const char p[], const long m);

#endif /* SRC_LEVDIST_H_ */
