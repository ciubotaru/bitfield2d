
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield2d.h"

/* Testing bfnew() and bfdel() */

int main() {
	srand((unsigned)time(NULL));
	unsigned int i;
	unsigned int len = 80;
	char *msg = "Testing bfnew() and bfdel()";
	char *status[] = { "[PASS]", "[FAIL]" };
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	int retval = 0;
	bitfield *instance = bfnew(10, 10);
	if (!instance) {
		retval = 1;
		goto ret;
	}
	bfdel(&instance);
	if (instance) {
		retval = 1;
		goto ret;
	}
 ret:
	printf("%s\n", status[retval]);
	return retval;
}
