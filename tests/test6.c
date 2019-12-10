#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitmatrix.h"
#include "bitmatrix-internals.h"

/* Testing bm_and(), bm_not(), bm_or() and bm_xor() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bm_and(), bm_not(), bm_or() and bm_xor()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	struct bitmatrix *input1 = bm_new(10, 10);
	struct bitmatrix *input2 = bm_new(10, 10);
	int x, y;
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (rand() % 2)
				BITSET(input1, x, y);
			if (rand() % 2)
				BITSET(input2, x, y);
		}
	}

	struct bitmatrix *output1 = bm_xor(input1, input2);
	struct bitmatrix *and = bm_and(input1, input2);
	struct bitmatrix *or = bm_or(input1, input2);
	struct bitmatrix *not = bm_not(and);
	struct bitmatrix *output2 = bm_and(or, not);
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bm_getbit(output1, x, y) != bm_getbit(output2, x, y)) {
				retval = 1;
				goto ret;
			}
		}
	}
ret:
	bm_del(&input1, &input2, &and, &or, &not, &output1, &output2);
	printf("%s\n", status[retval]);
	return retval;
}
