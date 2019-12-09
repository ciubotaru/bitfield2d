#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"

/* Testing bftranspose() */

int main() {
	srand((unsigned)time(NULL));
	unsigned int i;
	unsigned int len = 80;
	char *msg = "Testing bftranspose()";
	char *status[] = { "[PASS]", "[FAIL]" };
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	bitfield *input = bfnew(10, 10);
	if (!input) {
		retval = 1;
		goto ret;
	}
	unsigned int x, y;
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (rand() % 2) bfsetbit(input, x, y);
		}
	}
	bitfield *output = bftranspose(input);
	if (!output) {
		retval = 1;
		goto ret;
	}
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bfgetbit(input, x, y) != bfgetbit(output, y, x)) {
				retval = 1;
				goto ret;
			}
		}
	}
 ret:
	printf("%s\n", status[retval]);
	return retval;
}
