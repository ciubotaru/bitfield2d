#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"
#include "bitfield2d-internals.h"

/* Testing bfand(), bfnot(), bfor() and bfxor() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bfand(), bfnot(), bfor() and bfxor()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	struct bitfield *input1 = bfnew(10, 10);
	struct bitfield *input2 = bfnew(10, 10);
	int x, y;
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (rand() % 2)
				BITSET(input1, x, y);
			if (rand() % 2)
				BITSET(input2, x, y);
		}
	}

	struct bitfield *output1 = bfxor(input1, input2);
	struct bitfield *and = bfand(input1, input2);
	struct bitfield *or = bfor(input1, input2);
	struct bitfield *not = bfnot(and);
	struct bitfield *output2 = bfand(or, not);
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bfgetbit(output1, x, y) != bfgetbit(output2, x, y)) {
				retval = 1;
				goto ret;
			}
		}
	}
ret:
	bfdel(&input1, &input2, &and, &or, &not, &output1, &output2);
	printf("%s\n", status[retval]);
	return retval;
}
