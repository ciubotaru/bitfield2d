#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"
#include "bitfield2d-internals.h"

/* Testing bfsetall() and bfclearall() */

int main()
{
	srand((unsigned)time(NULL));
	int i, cmp;		//counter
	int len = 80;
	char *msg = "Testing bfsetall() and bfclearall()";
	char *status[] = { "[PASS]", "[FAIL]" };
	int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	struct bitfield *input = bfnew(10, 10);
	int x, y;
	bfsetall(input);
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bfgetbit(input, x, y) != 1) {
				retval = 1;
				goto ret;
			}
		}
	}
	bfclearall(input);
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bfgetbit(input, x, y) != 0) {
				retval = 1;
				goto ret;
			}
		}
	}
ret:
	bfdel(&input);
	printf("%s\n", status[retval]);
	return retval;
}
