SRCFILES = $(wildcard *.c)
TESTFILES = $(patsubst %.c, %, $(SRCFILES))

#TESTFILES=test1 
LIBS = -L.. -I..

all: build-tests run-tests

build-tests: $(TESTFILES)
$(TESTFILES): %: %.c
	$(CC) $(CFLAGS) -Wl,-rpath=$(CURDIR) -o $@ $< $(LIBS) -lbitmatrix

run-tests: $(TESTFILES)
	for i in $(TESTFILES); do \
	env LD_LIBRARY_PATH=.. ./$$i; \
	done

clean:
	rm -rvf $(TESTFILES)

distclean:
	rm Makefile

.PHONY: all build-tests run-tests clean distclean
