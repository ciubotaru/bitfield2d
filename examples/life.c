#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <bitmatrix.h>

#define ROWS 10
#define COLS 10

int main() {
	bitmatrix *map = bm_new(ROWS, COLS);
	unsigned int i, j, self, neighbours;
	/* draw a "glider" */
	bm_setbit(map, 0, 1);
	bm_setbit(map, 1, 2);
	bm_setbit(map, 2, 0);
	bm_setbit(map, 2, 1);
	bm_setbit(map, 2, 2);
	bitmatrix *tmp = NULL;
	while(1) {
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLS; j++) {
				if (bm_getbit(map, i, j)) printf("1");
				else printf("0");
			}
			printf("\n");
		}
		tmp = bm_new(ROWS, COLS);
		printf("-----------\n");
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLS; j++) {
				neighbours = 0;
				neighbours += bm_getbit(map, (i + ROWS - 1) % ROWS, (j + COLS - 1) % COLS);
				neighbours += bm_getbit(map, (i + ROWS - 1) % ROWS, (j + COLS) % COLS);
				neighbours += bm_getbit(map, (i + ROWS - 1) % ROWS, (j + COLS + 1) % COLS);
				neighbours += bm_getbit(map, (i + ROWS) % ROWS, (j + COLS - 1) % COLS);
				neighbours += bm_getbit(map, (i + ROWS) % ROWS, (j + COLS + 1) % COLS);
				neighbours += bm_getbit(map, (i + ROWS + 1) % ROWS, (j + COLS - 1) % COLS);
				neighbours += bm_getbit(map, (i + ROWS + 1) % ROWS, (j + COLS) % COLS);
				neighbours += bm_getbit(map, (i + ROWS + 1) % ROWS, (j + COLS + 1) % COLS);
				self = bm_getbit(map, i , j);
				if (neighbours == 3) bm_setbit(tmp, i, j);
				else if (self && neighbours == 2) bm_setbit(tmp, i, j);
			}
		}
		bm_del(&map);
		map = tmp;
		tmp = NULL;
		usleep(1000000); /* yes, this is deprecated */
	}
	return 0;
}