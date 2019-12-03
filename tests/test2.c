#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"

/* Testing bfgetbit(), bfsetbit(), bfclearbit() and bftogglebit() */

int main() {
	srand((unsigned)time(NULL));
	unsigned int i;
	unsigned int len = 80;
	char *msg =
	    "Testing bfgetbit(), bfsetbit(), bfclearbit() and bftogglebit()";
	char *status[] = { "[PASS]", "[FAIL]" };
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	bitfield *instance = bfnew(10, 10);
	unsigned int x, y;
	x = rand() % 10;
	y = rand() % 10;
	bfsetbit(instance, x, y);
	if (bfgetbit(instance, x, y) != 1) {
		retval = 1;
		goto ret;
	}
	bfclearbit(instance, x, y);
	if (bfgetbit(instance, x, y) != 0) {
		retval = 1;
		goto ret;
	}
	bftogglebit(instance, x, y);
	if (bfgetbit(instance, x, y) != 1) {
		retval = 1;
		goto ret;
	}
 ret:
	bfdel(&instance);
	printf("%s\n", status[retval]);
	return retval;
}
