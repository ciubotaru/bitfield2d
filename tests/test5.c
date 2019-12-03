#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"

/* Testing bfsub() */

int main() {
	srand((unsigned)time(NULL));
	unsigned int i;
	unsigned int len = 80;
	char *msg = "Testing bfsub()";
	char *status[] = { "[PASS]", "[FAIL]" };
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	bitfield *input = bfnew(20, 20);
	unsigned int x, y;
	for (x = 0; x < 20; x++) {
		for (y = 0; y < 20; y++) {
			if (rand() % 2) {
				bfsetbit(input, x, y);
			}
		}
	}
	bitfield *output = bfsub(input, 5, 5, 10, 10);
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (bfgetbit(input, x + 5, y + 5) !=
			    bfgetbit(output, x, y)) {
				retval = 1;
				goto ret;
			}
		}
	}
 ret:
	bfdel(&input, &output);
	printf("%s\n", status[retval]);
	return retval;
}
