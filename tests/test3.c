#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"

/* Testing bfresize() */

int main() {
	srand((unsigned)time(NULL));
	unsigned int i;
	unsigned int len = 80;
	char *msg = "Testing bfresize()";
	char *status[] = { "[PASS]", "[FAIL]" };
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	bitfield *instance = bfnew(10, 10);
	unsigned int x, y;
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			bfsetbit(instance, x, y);
		}
	}
	bfresize(instance, 11, 11);
	for (x = 0; x < 11; x++) {
		if (bfgetbit(instance, x, 10) != 0) {
			retval = 1;
			goto ret;
		}
	}
	for (y = 0; y < 10; y++) {
		if (bfgetbit(instance, 10, y) != 0) {
			retval = 1;
			goto ret;
		}
	}
	bfresize(instance, 9, 9);
	for (x = 0; x < 9; x++) {
		for (y = 0; y < 9; y++) {
			if (bfgetbit(instance, x, y) != 1) {
				retval = 1;
				goto ret;
			}
		}
	}
	bfresize(instance, 10, 10);
	for (x = 0; x < 10; x++) {
		if (bfgetbit(instance, x, 9) != 0) {
			retval = 1;
			goto ret;
		}
	}
	for (y = 0; y < 10; y++) {
		if (bfgetbit(instance, 9, y) != 0) {
			retval = 1;
			goto ret;
		}
	}
 ret:
	bfdel(&instance);
	printf("%s\n", status[retval]);
	return retval;
}
