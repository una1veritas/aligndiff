//============================================================================
// Name        : aligndiff.cpp
// Author      : Sin Shimozono
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <cstdio>
#include <cstdlib>
#include <cinttypes>

#include <iostream>
#include <fstream>

extern "C" {
#include "cutils.h"
}

using namespace std;

typedef uint32_t uint32;
typedef int32_t int32;
typedef uint16_t uint16;

const uint32 STR_LENGTH = 128*1024;
const uint32 BUF_LENGTH = 1024;

int main(const int argc, const char * argv[]) {

	ifstream file1, file2;
	char buf[BUF_LENGTH];
	uint16 * str1, * str2;

	file1.open(argv[1]);
	file2.open(argv[2]);

	if ( !file1.is_open() || !file2.is_open() ) {
		cerr << "failed to open file(s)." << endl;
		return EXIT_FAILURE;
	}

	uint32 count = 0;
	char c;
	while ( file1.get(c) ) {
		count++;
	}
	file1.clear();
	file1.seekg(0);

	uint32 str1len = c2pow32(MIN(count, STR_LENGTH));
	str1 = new uint16[str1len];

	for (int i = 0; i < str1len; ++i ) {
		file1.get(c);
		str1[i] = c;
	}

	file1.close();

	cout << "got " << count << " chars." << endl;
	cout << "allocated " << str1len << " uint16s." << endl;

	delete [] str1;

	return EXIT_SUCCESS;

}
