CC = gcc
export CC
LIBS = -I.
VERSION = 0.0.1
export VERSION
TESTDIR=tests
LD_LIBRARY_PATH := $(shell pwd):$(LD_LIBRARY_PATH)
export LD_LIBRARY_PATH

.PHONY: test clean distclean

all: shared

shared: libbitfield2d.so.$(VERSION)
libbitfield2d.so.$(VERSION): bitfield2d.c bitfield2d.h bitfield2d-internals.h
	$(CC) $(CFLAGS) -c -fpic bitfield2d.c -o bf-shared.o -I. $(LIBS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,libbitfield2d.so.1 -o libbitfield2d.so.$(VERSION) bf-shared.o $(LIBS)
	for i in libbitfield2d.so.1 libbitfield2d.so; do ln -svf libbitfield2d.so.$(VERSION) $$i; done

test: check

check:
	$(MAKE) -C $(TESTDIR)

clean:
	rm -rvf *.a *.o *.so*
	$(MAKE) -C $(TESTDIR) clean
