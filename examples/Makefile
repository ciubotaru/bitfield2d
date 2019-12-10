SRCFILES = $(wildcard *.c)
EXAMPLES = $(patsubst %.c, %, $(SRCFILES))
LIBS = -I..

all: build-examples
build-examples: $(EXAMPLES)
$(EXAMPLES): %: %.c
	$(CC) $(CFLAGS) -o $@ $< ../libbitmatrix.a $(LIBS)

clean:
	rm -rvf $(EXAMPLES)

distclean:
	rm Makefile

.PHONY: build-tests run-tests clean distclean
