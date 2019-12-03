#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"

/* Testing bfpopcount() */

int main() {
	srand((unsigned)time(NULL));
	unsigned int i;
	unsigned int len = 80;
	char *msg = "Testing bfpopcount()";
	char *status[] = { "[PASS]", "[FAIL]" };
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	int count = 0;
	bitfield *instance = bfnew(10, 10);
	unsigned int x, y;
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			if (rand() % 2) {
				bfsetbit(instance, x, y);
				count++;
			}
		}
	}
	if (bfpopcount(instance) != count) {
		retval = 1;
		goto ret;
	}
 ret:
	bfdel(&instance);
	printf("%s\n", status[retval]);
	return retval;
}
