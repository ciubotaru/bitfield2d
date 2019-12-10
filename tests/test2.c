#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitmatrix.h"

/* Testing bm_getbit(), bm_setbit(), bm_clearbit() and bm_togglebit() */

int main() {
	srand((unsigned)time(NULL));
	unsigned int i;
	unsigned int len = 80;
	char *msg =
	    "Testing bm_getbit(), bm_setbit(), bm_clearbit() and bm_togglebit()";
	char *status[] = { "[PASS]", "[FAIL]" };
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	bitmatrix *instance = bm_new(10, 10);
	unsigned int x, y;
	x = rand() % 10;
	y = rand() % 10;
	bm_setbit(instance, x, y);
	if (bm_getbit(instance, x, y) != 1) {
		retval = 1;
		goto ret;
	}
	bm_clearbit(instance, x, y);
	if (bm_getbit(instance, x, y) != 0) {
		retval = 1;
		goto ret;
	}
	bm_togglebit(instance, x, y);
	if (bm_getbit(instance, x, y) != 1) {
		retval = 1;
		goto ret;
	}
 ret:
	bm_del(&instance);
	printf("%s\n", status[retval]);
	return retval;
}
