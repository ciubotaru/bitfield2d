#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitmatrix.h"
#include "bitmatrix-internals.h"

/* Testing bm_setall() and bm_clearall() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bm_setall() and bm_clearall()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	struct bitmatrix *input = bm_new(10, 10);
	int x, y;
	bm_setall(input);
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bm_getbit(input, x, y) != 1) {
				retval = 1;
				goto ret;
			}
		}
	}
	bm_clearall(input);
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bm_getbit(input, x, y) != 0) {
				retval = 1;
				goto ret;
			}
		}
	}
ret:
	bm_del(&input);
	printf("%s\n", status[retval]);
	return retval;
}
