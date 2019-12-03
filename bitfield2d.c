#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "bitfield2d.h"
#include "bitfield2d-internals.h"

static inline void bfcleartail(struct bitfield *instance) {
	int tail = (instance->x * instance->y) % LONG_BIT;
	if (tail != 0) {
		/* create a mask for the tail */
		unsigned long mask = (1UL << tail) - 1UL;
		/* clear the extra bits */
		instance->field[BITNSLOTS(instance->x, instance->y) - 1] &=
		    mask;
	}
}

bitfield *bfnew(const unsigned int x, const unsigned int y) {
	bitfield *output = (bitfield *) malloc(sizeof(bitfield));
	output->x = x;
	output->y = y;
	output->field = calloc(1, BITNSLOTS(x, y) * sizeof(unsigned long));
	return output;
}

void __bfdel(bitfield ** instance) {
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

void bfsetbit(bitfield * instance, const unsigned int x, const unsigned int y) {
	BITSET(instance, x, y);
}

unsigned int bfgetbit(const bitfield * instance, const unsigned int x,
		      const unsigned int y) {
	return BITGET(instance, x, y);
}

void bfclearbit(bitfield * instance, const unsigned int x, const unsigned int y) {
	BITCLEAR(instance, x, y);
}

void bftogglebit(bitfield * instance, const unsigned int x,
		 const unsigned int y) {
	BITTOGGLE(instance, x, y);
}

bitfield *bfand(bitfield * const input1, bitfield * const input2) {
	if (input1->x != input2->x || input1->y != input2->y)
		return NULL;
	int i;
	bitfield *output = bfnew(input1->x, input1->y);
	for (i = 0; i < BITNSLOTS(input1->x, input1->y); i++) {
		output->field[i] = BITSLOT_AND(input1, input2, i);
	}
	return output;
}

bitfield *bfor(bitfield * const input1, bitfield * const input2) {
	if (input1->x != input2->x || input1->y != input2->y)
		return NULL;
	int i;
	bitfield *output = bfnew(input1->x, input1->y);
	for (i = 0; i < BITNSLOTS(input1->x, input1->y); i++) {
		output->field[i] = BITSLOT_OR(input1, input2, i);
	}
	return output;
}

bitfield *bfxor(bitfield * const input1, bitfield * const input2) {
	if (input1->x != input2->x || input1->y != input2->y)
		return NULL;
	int i;
	bitfield *output = bfnew(input1->x, input1->y);
	for (i = 0; i < BITNSLOTS(input1->x, input1->y); i++) {
		output->field[i] = BITSLOT_XOR(input1, input2, i);
	}
	return output;
}

bitfield *bfnot(bitfield * const input) {
	bitfield *output = bfnew(input->x, input->y);
	int i;
	for (i = 0; i < BITNSLOTS(input->x, input->y); i++) {
		output->field[i] = ~input->field[i];
	}
	bfcleartail(output);
	return output;
}

void bfresize(bitfield * instance, const unsigned int new_x,
	      const unsigned int new_y) {
	unsigned long *tmp =
	    calloc(1, BITNSLOTS(new_x, new_y) * sizeof(unsigned long));
	int i, j;
	unsigned int src_cur_bit, src_cur_slot, src_end_bit, dst_cur_bit,
	    dst_cur_slot, dst_end_bit, chunk_len;
	unsigned long chunk, mask;
	for (i = 0; i < instance->x; i++) {
		src_cur_bit = i * instance->y;
		src_end_bit = src_cur_bit + MIN(instance->y, new_y) - 1;
		dst_cur_bit = i * new_y;
		dst_end_bit = dst_cur_bit + MIN(instance->y, new_y) - 1;
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
	instance->x = new_x;
	instance->y = new_y;
}

bitfield *bfsub(bitfield * input, const unsigned int x_start,
		const unsigned int y_start, const unsigned int x_len,
		const unsigned int y_len) {
	if (input->x < x_start + x_len || input->y < y_start + y_len)
		return NULL;
	bitfield *output = bfnew(x_len, y_len);
	int i_src, i_dst;
	unsigned int src_cur_bit, src_cur_slot, src_end_bit, src_end_slot,
	    src_slots, dst_cur_bit, dst_cur_slot, dst_end_bit, dst_end_slot,
	    dst_slots;
	unsigned int chunk_len;
	unsigned long chunk, mask;
	for (i_src = x_start, i_dst = 0; i_dst < x_len; i_src++, i_dst++) {
		src_cur_bit = i_src * input->y + y_start;
		src_end_bit = src_cur_bit + y_len - 1;
		src_cur_slot = BITSLOT_FLAT(src_cur_bit);
		src_end_slot = BITSLOT_FLAT(src_end_bit);
		src_slots = src_end_slot - src_cur_slot + 1;
		dst_cur_bit = i_dst * y_len;
		dst_end_bit = dst_cur_bit + y_len - 1;
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

unsigned int bfpopcount(const struct bitfield *instance) {
	unsigned int bits = 0;
	unsigned int i;
	for (i = 0; i < BITNSLOTS(instance->x, instance->y); i++)
		/* this is GCC and Clang only */
		bits += __builtin_popcountl(instance->field[i]);
	return bits;
}

void bfprint(bitfield * instance) {
	int i, j;
	for (i = 0; i < instance->x; i++) {
		for (j = 0; j < instance->y; j++) {
			printf("%i", bfgetbit(instance, i, j));
		}
		printf("\n");
	}
}

/*
void bfrand(bitfield * instance) {
	int i, j;
	for (i = 0; i < instance->x; i++) {
		for (j = 0; j < instance->y; j++) {
			if (rand() % 2)
				BITSET(instance, i, j);
		}
	}
}
*/
void bfsetall(bitfield * instance) {
	int i;
	for (i = 0; i < BITNSLOTS(instance->x, instance->y); i++) {
		instance->field[i] = ~0UL;
	}
	bfcleartail(instance);
}
