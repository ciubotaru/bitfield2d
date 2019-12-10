#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitmatrix.h"
#include "bitmatrix-internals.h"

/* Testing bm_mul() */

int main() {
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bm_mul()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	unsigned int input1_r = 15;
	unsigned int input1_c = 12;
	unsigned int input2_r = input1_c;
	unsigned int input2_c = 14;
	struct bitmatrix *input1 = bm_new(input1_r, input1_c);
	struct bitmatrix *input2 = bm_new(input2_r, input2_c);
	int x, y, z;
	for (x = 0; x < input1_r; x++) {
		for (y = 0; y < input1_c; y++) {
			if (rand() % 2)
				bm_setbit(input1, x, y);
		}
	}
	for (x = 0; x < input2_r; x++) {
		for (y = 0; y < input2_c; y++) {
			if (rand() % 2)
				bm_setbit(input2, x, y);
		}
	}
	bitmatrix *output = bm_mul(input1, input2);
	/**
	 * this test needs to be rewritten from scratch
	 * e.g. check a single random value in  output
	**/
	bitmatrix *check = bm_new(input1_r, input2_c);
	bitmatrix *tmp1, *tmp2;
	unsigned int popcount;
	for (x = 0; x < input1_r; x++) {
		for (y = 0; y < input2_c; y++) {
			tmp1 = bm_sub(input1, x, 0, 1, input1_c);
			tmp2 = bm_sub(input2, 0, y, input2_r, 1);
			popcount = 0;
			for (z = 0;
			     z < BITNSLOTS(1, input1_c);
			     z++) {
				popcount +=
				    __builtin_popcountl(tmp1->field[z] & tmp2->
							field[z]);
			}
			if (popcount % 2)
				BITSET(check, x, y);
			bm_del(&tmp1, &tmp2);
		}
	}
	for (x = 0; x < input1_r; x++) {
		for (y = 0; y < input2_c; y++) {
			if (bm_getbit(output, x, y) != bm_getbit(check, x, y)) {
				retval = 1;
				goto ret;
			}
		}
	}
 ret:
	bm_del(&input1, &input2, &output, &check);
	printf("%s\n", status[retval]);
	return retval;
}
