#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <bitfield2d.h>

#define ROWS 10
#define COLS 10

int main() {
	bitfield *map = bfnew(ROWS, COLS);
	unsigned int i, j, self, neighbours;
	/* draw a "glider" */
	bfsetbit(map, 0, 1);
	bfsetbit(map, 1, 2);
	bfsetbit(map, 2, 0);
	bfsetbit(map, 2, 1);
	bfsetbit(map, 2, 2);
	bitfield *tmp = NULL;
	while(1) {
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLS; j++) {
				if (bfgetbit(map, i, j)) printf("1");
				else printf("0");
			}
			printf("\n");
		}
		tmp = bfnew(ROWS, COLS);
		printf("-----------\n");
		for (i = 0; i < ROWS; i++) {
			for (j = 0; j < COLS; j++) {
				neighbours = 0;
				neighbours += bfgetbit(map, (i + ROWS - 1) % ROWS, (j + COLS - 1) % COLS);
				neighbours += bfgetbit(map, (i + ROWS - 1) % ROWS, (j + COLS) % COLS);
				neighbours += bfgetbit(map, (i + ROWS - 1) % ROWS, (j + COLS + 1) % COLS);
				neighbours += bfgetbit(map, (i + ROWS) % ROWS, (j + COLS - 1) % COLS);
				neighbours += bfgetbit(map, (i + ROWS) % ROWS, (j + COLS + 1) % COLS);
				neighbours += bfgetbit(map, (i + ROWS + 1) % ROWS, (j + COLS - 1) % COLS);
				neighbours += bfgetbit(map, (i + ROWS + 1) % ROWS, (j + COLS) % COLS);
				neighbours += bfgetbit(map, (i + ROWS + 1) % ROWS, (j + COLS + 1) % COLS);
				self = bfgetbit(map, i , j);
				if (neighbours == 3) bfsetbit(tmp, i, j);
				else if (self && neighbours == 2) bfsetbit(tmp, i, j);
			}
		}
		bfdel(&map);
		map = tmp;
		tmp = NULL;
		usleep(1000000); /* yes, this is deprecated */
	}
	return 0;
}