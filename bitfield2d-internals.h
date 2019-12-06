#ifndef CHAR_BIT
#include <limits.h>
#endif
#ifndef LONG_BIT
#define LONG_BIT (unsigned int) (sizeof(unsigned long) * CHAR_BIT)
#endif
#define BITINDEX(bf, a, b) (a * (bf)->columns + b)
#define BITNSLOTS(x, y) (((x * y) + LONG_BIT - 1) / LONG_BIT)
#define BITSLOT(bf, a, b) (BITSLOT_FLAT(BITINDEX(bf, a, b)))
#define BITSLOT_FLAT(a) (a / LONG_BIT)
#define BITMASK_ONES(m, n) (n >= LONG_BIT ?  0UL : ~(m >= LONG_BIT ? 0UL : ~0UL << m) << n)
#define BITPOS(bf, a, b) (BITPOS_FLAT(BITINDEX(bf, a, b)))
#define BITPOS_FLAT(a) (a % LONG_BIT)
#define BITMASK(bf, a, b) (1UL << (BITPOS(bf, a, b)))
#define BITGET(bf, a, b) (((bf)->field[BITSLOT(bf, a, b)] >> BITPOS(bf, a, b)) & 1UL)
#define BITSET(bf, a, b) ((bf)->field[BITSLOT(bf, a, b)] |= BITMASK(bf, a, b))
#define BITCLEAR(bf, a, b) ((bf)->field[BITSLOT(bf, a, b)] &= ~BITMASK(bf, a, b))
#define BITTEST(bf, a, b) ((bf)->field[BITSLOT(bf, a, b)] & BITMASK(bf, a, b))
#define BITTOGGLE(bf, a, b) ((bf)->field[BITSLOT(bf, a, b)] ^= BITMASK(bf, a, b))
#define BITSLOT_AND(a, b, i) ((a)->field[i] & (b)->field[i])
#define BITSLOT_OR(a, b, i) ((a)->field[i] | (b)->field[i])
#define BITSLOT_XOR(a, b, i) ((a)->field[i] ^ (b)->field[i])

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) > (Y)) ? (Y) : (X))

struct bitfield {
	unsigned int rows;
	unsigned int columns;
	unsigned long *field;
};
