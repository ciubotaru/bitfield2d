#ifndef CHAR_BIT
#include <limits.h>
#endif
#ifndef LONG_BIT
#define LONG_BIT (unsigned int) (sizeof(unsigned long) * CHAR_BIT)
#endif
#define BITINDEX(bm, a, b) (a * (bm)->columns + b)
#define BITNSLOTS(x, y) (((x * y) + LONG_BIT - 1) / LONG_BIT)
#define BITSLOT(bm, a, b) (BITSLOT_FLAT(BITINDEX(bm, a, b)))
#define BITSLOT_FLAT(a) (a / LONG_BIT)
#define BITMASK_ONES(m, n) (n >= LONG_BIT ?  0UL : ~(m >= LONG_BIT ? 0UL : ~0UL << m) << n)
#define BITPOS(bm, a, b) (BITPOS_FLAT(BITINDEX(bm, a, b)))
#define BITPOS_FLAT(a) (a % LONG_BIT)
#define BITMASK(bm, a, b) (1UL << (BITPOS(bm, a, b)))
#define BITGET(bm, a, b) (((bm)->field[BITSLOT(bm, a, b)] >> BITPOS(bm, a, b)) & 1UL)
#define BITSET(bm, a, b) ((bm)->field[BITSLOT(bm, a, b)] |= BITMASK(bm, a, b))
#define BITCLEAR(bm, a, b) ((bm)->field[BITSLOT(bm, a, b)] &= ~BITMASK(bm, a, b))
#define BITTEST(bm, a, b) ((bm)->field[BITSLOT(bm, a, b)] & BITMASK(bm, a, b))
#define BITTOGGLE(bm, a, b) ((bm)->field[BITSLOT(bm, a, b)] ^= BITMASK(bm, a, b))
#define BITSLOT_AND(a, b, i) ((a)->field[i] & (b)->field[i])
#define BITSLOT_OR(a, b, i) ((a)->field[i] | (b)->field[i])
#define BITSLOT_XOR(a, b, i) ((a)->field[i] ^ (b)->field[i])

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) > (Y)) ? (Y) : (X))

struct bitmatrix {
	unsigned int rows;
	unsigned int columns;
	unsigned long *field;
};
