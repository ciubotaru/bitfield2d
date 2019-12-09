#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"
#include "bitfield2d-internals.h"

/* Testing bfmul() */

int main() {
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bfmul()";
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
	struct bitfield *input1 = bfnew(input1_r, input1_c);
	struct bitfield *input2 = bfnew(input2_r, input2_c);
	int x, y, z;
	for (x = 0; x < input1_r; x++) {
		for (y = 0; y < input1_c; y++) {
			if (rand() % 2)
				bfsetbit(input1, x, y);
		}
	}
	for (x = 0; x < input2_r; x++) {
		for (y = 0; y < input2_c; y++) {
			if (rand() % 2)
				bfsetbit(input2, x, y);
		}
	}
	bitfield *output = bfmul(input1, input2);
	/**
	 * this test needs to be rewritten from scratch
	 * e.g. check a single random value in  output
	**/
	bitfield *check = bfnew(input1_r, input2_c);
	bitfield *tmp1, *tmp2;
	unsigned int popcount;
	for (x = 0; x < input1_r; x++) {
		for (y = 0; y < input2_c; y++) {
			tmp1 = bfsub(input1, x, 0, 1, input1_c);
			tmp2 = bfsub(input2, 0, y, input2_r, 1);
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
			bfdel(&tmp1, &tmp2);
		}
	}
	for (x = 0; x < input1_r; x++) {
		for (y = 0; y < input2_c; y++) {
			if (bfgetbit(output, x, y) != bfgetbit(check, x, y)) {
				retval = 1;
				goto ret;
			}
		}
	}
 ret:
	bfdel(&input1, &input2, &output, &check);
	printf("%s\n", status[retval]);
	return retval;
}
