CC = gcc-9
PROG = blist_test
CFLAGS = -Wall -Wextra -Wpedantic -ggdb -std=gnu11 -O0
LDLIBS =

default: bin/$(PROG)

bin/$(PROG): bin/$(PROG).o bin/blist.o
	$(CC) $^ $(LDLIBS) -o $@

bin/$(PROG).o: src/$(PROG).c
	$(CC) $(CFLAGS) -o $@ -c $<

bin/blist.o: src/blist.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -fv bin/*.o bin/$(PROG)
