CC = gcc
export CC
LIBS = -I.
VERSION = 0.0.1
export VERSION
TESTDIR=tests
EXAMPLEDIR=examples
LD_LIBRARY_PATH := $(shell pwd):$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH

.PHONY: test clean distclean

all: shared static

shared: libbitmatrix.so.$(VERSION)
libbitmatrix.so.$(VERSION): bitmatrix.c bitmatrix.h bitmatrix-internals.h
	$(CC) $(CFLAGS) -c -fpic bitmatrix.c -o bm-shared.o -I. $(LIBS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libbitmatrix.so.1 -o libbitmatrix.so.$(VERSION) bm-shared.o $(LIBS)
	for i in libbitmatrix.so.1 libbitmatrix.so; do ln -svf libbitmatrix.so.$(VERSION) $$i; done

static: libbitmatrix.a
libbitmatrix.a: bitmatrix.c
	$(CC) $(CFLAGS) -c bitmatrix.c -o bm-static.o $(LIBS)
	$(AR) rcs libbitmatrix.a bm-static.o

test: check

check:
	$(MAKE) -C $(TESTDIR)

examples: example

example:
	$(MAKE) -C $(EXAMPLEDIR)

clean:
	rm -rvf *.a *.o *.so*
	$(MAKE) -C $(TESTDIR) clean
	$(MAKE) -C $(EXAMPLEDIR) clean
