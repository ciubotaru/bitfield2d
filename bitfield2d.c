#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "bitfield2d.h"
#include "bitfield2d-internals.h"

static inline void bfcleartail(bitfield *instance) {
	int tail = (instance->rows *instance->columns) % LONG_BIT;
	if (tail != 0) {
		/* create a mask for the tail */
		unsigned long mask = (1UL << tail) - 1UL;
		/* clear the extra bits */
		instance->field[BITNSLOTS(instance->rows, instance->columns) - 1] &=
		    mask;
	}
}

bitfield *bfnew(const unsigned int rows, const unsigned int columns) {
	bitfield *output = (bitfield *) malloc(sizeof(bitfield));
	output->rows = rows;
	output->columns = columns;
	output->field = calloc(1, BITNSLOTS(rows, columns) * sizeof(unsigned long));
	return output;
}

void __bfdel(bitfield **instance) {
	if (*instance == NULL)
		return;
	free((*instance)->field);
	free(*instance);
	*instance = NULL;
}

void _bfdel(unsigned int count, ...) {
	unsigned int i;
	bitfield **instance = NULL;
	va_list args;
	va_start(args, count);
	for (i = 0; i < count; i++) {
		instance = va_arg(args, bitfield **);
		__bfdel(instance);
	}
	va_end(args);
}

void bfsetbit(bitfield *instance, const unsigned int row_nr, const unsigned int column_nr) {
	BITSET(instance, row_nr, column_nr);
}

unsigned int bfgetbit(const bitfield *instance, const unsigned int row_nr,
		      const unsigned int column_nr) {
	return BITGET(instance, row_nr, column_nr);
}

void bfclearbit(bitfield *instance, const unsigned int row_nr, const unsigned int column_nr) {
	BITCLEAR(instance, row_nr, column_nr);
}

void bftogglebit(bitfield *instance, const unsigned int row_nr,
		 const unsigned int column_nr) {
	BITTOGGLE(instance, row_nr, column_nr);
}

bitfield *bfand(const bitfield *input1, const bitfield *input2) {
	if (input1->rows != input2->rows || input1->columns != input2->columns)
		return NULL;
	int i;
	bitfield *output = bfnew(input1->rows, input1->columns);
	for (i = 0; i < BITNSLOTS(input1->rows, input1->columns); i++) {
		output->field[i] = BITSLOT_AND(input1, input2, i);
	}
	return output;
}

bitfield *bfor(const bitfield *input1, const bitfield *input2) {
	if (input1->rows != input2->rows || input1->columns != input2->columns)
		return NULL;
	int i;
	bitfield *output = bfnew(input1->rows, input1->columns);
	for (i = 0; i < BITNSLOTS(input1->rows, input1->columns); i++) {
		output->field[i] = BITSLOT_OR(input1, input2, i);
	}
	return output;
}

bitfield *bfxor(const bitfield *input1, const bitfield *input2) {
	if (input1->rows != input2->rows || input1->columns != input2->columns)
		return NULL;
	int i;
	bitfield *output = bfnew(input1->rows, input1->columns);
	for (i = 0; i < BITNSLOTS(input1->rows, input1->columns); i++) {
		output->field[i] = BITSLOT_XOR(input1, input2, i);
	}
	return output;
}

bitfield *bfnot(const bitfield *input) {
	bitfield *output = bfnew(input->rows, input->columns);
	int i;
	for (i = 0; i < BITNSLOTS(input->rows, input->columns); i++) {
		output->field[i] = ~input->field[i];
	}
	bfcleartail(output);
	return output;
}

void bfresize(bitfield *instance, const unsigned int new_rows,
	      const unsigned int new_columns) {
	unsigned long *tmp =
	    calloc(1, BITNSLOTS(new_rows, new_columns) * sizeof(unsigned long));
	int i, j;
	unsigned int src_cur_bit, src_cur_slot, src_end_bit, dst_cur_bit,
	    dst_cur_slot, dst_end_bit, chunk_len;
	unsigned long chunk, mask;
	for (i = 0; i < instance->rows; i++) {
		src_cur_bit = i * instance->columns;
		src_end_bit = src_cur_bit + MIN(instance->columns, new_columns) - 1;
		dst_cur_bit = i * new_columns;
		dst_end_bit = dst_cur_bit + MIN(instance->columns, new_columns) - 1;
		while (src_cur_bit <= src_end_bit) {
			chunk_len =
			    MIN(MIN
				(LONG_BIT - src_cur_bit % LONG_BIT,
				 LONG_BIT - dst_cur_bit % LONG_BIT),
				MIN(src_end_bit - src_cur_bit,
				    dst_end_bit - dst_cur_bit) + 1);
			src_cur_slot = BITSLOT_FLAT(src_cur_bit);
			dst_cur_slot = BITSLOT_FLAT(dst_cur_bit);
			mask = BITMASK_ONES(chunk_len,
					    BITPOS_FLAT(src_cur_bit));
			chunk = ((instance->field[src_cur_slot] & mask)) >>
			    BITPOS_FLAT(src_cur_bit);
			tmp[dst_cur_slot] |=
			    (chunk << BITPOS_FLAT(dst_cur_bit));
			src_cur_bit += chunk_len;
			dst_cur_bit += chunk_len;
		}
	}
	free(instance->field);
	instance->field = tmp;
	instance->rows = new_rows;
	instance->columns = new_columns;
}

bitfield *bfsub(const bitfield *input, const unsigned int start_row,
		const unsigned int start_column, const unsigned int nr_rows,
		const unsigned int nr_columns) {
	if (input->rows < start_row + nr_rows || input->columns < start_column + nr_columns)
		return NULL;
	bitfield *output = bfnew(nr_rows, nr_columns);
	int i_src, i_dst;
	unsigned int src_cur_bit, src_cur_slot, src_end_bit, src_end_slot,
	    src_slots, dst_cur_bit, dst_cur_slot, dst_end_bit, dst_end_slot,
	    dst_slots;
	unsigned int chunk_len;
	unsigned long chunk, mask;
	for (i_src = start_row, i_dst = 0; i_dst < nr_rows; i_src++, i_dst++) {
		src_cur_bit = i_src * input->columns + start_column;
		src_end_bit = src_cur_bit + nr_columns - 1;
		src_cur_slot = BITSLOT_FLAT(src_cur_bit);
		src_end_slot = BITSLOT_FLAT(src_end_bit);
		src_slots = src_end_slot - src_cur_slot + 1;
		dst_cur_bit = i_dst * nr_columns;
		dst_end_bit = dst_cur_bit + nr_columns - 1;
		dst_cur_slot = BITSLOT_FLAT(dst_cur_bit);
		dst_end_slot = BITSLOT_FLAT(dst_end_bit);
		dst_slots = dst_end_slot - dst_cur_slot + 1;
		while (src_cur_bit <= src_end_bit) {
			chunk_len =
			    MIN(MIN
				(LONG_BIT - src_cur_bit % LONG_BIT,
				 LONG_BIT - dst_cur_bit % LONG_BIT),
				src_end_bit - src_cur_bit + 1);
			src_cur_slot = BITSLOT_FLAT(src_cur_bit);
			dst_cur_slot = BITSLOT_FLAT(dst_cur_bit);
			mask = BITMASK_ONES(chunk_len,
					    BITPOS_FLAT(src_cur_bit));
			chunk = ((input->field[src_cur_slot] & mask)) >>
			    BITPOS_FLAT(src_cur_bit);
			output->field[dst_cur_slot] |=
			    (chunk << BITPOS_FLAT(dst_cur_bit));
			src_cur_bit += chunk_len;
			dst_cur_bit += chunk_len;
		}
	}
	return output;
}

unsigned int bfpopcount(const bitfield *instance) {
	unsigned int bits = 0;
	unsigned int i;
	for (i = 0; i < BITNSLOTS(instance->rows, instance->columns); i++)
		/* this is GCC and Clang only */
		bits += __builtin_popcountl(instance->field[i]);
	return bits;
}

void bfprint(bitfield *instance) {
	int i, j;
	for (i = 0; i < instance->rows; i++) {
		for (j = 0; j < instance->columns; j++) {
			printf("%i", bfgetbit(instance, i, j));
		}
		printf("\n");
	}
}

/*
void bfrand(bitfield *instance) {
	int i, j;
	for (i = 0; i < instance->rows; i++) {
		for (j = 0; j < instance->columns; j++) {
			if (rand() % 2)
				BITSET(instance, i, j);
		}
	}
}
*/
void bfsetall(bitfield *instance) {
	int i;
	for (i = 0; i < BITNSLOTS(instance->rows, instance->columns); i++) {
		instance->field[i] = ~0UL;
	}
	bfcleartail(instance);
}
