CC = cc
PREFIX = /usr/local
CFLAGS = -std=c99 -O2 -fomit-frame-pointer -Wall -D_BSD_SOURCE
LDFLAGS =


OBJECTS = ssb_sprom.o utils.o

all: ssb_sprom

ssb_sprom: $(OBJECTS)
	$(CC) $(CFLAGS) -o ssb-sprom $(OBJECTS) $(LDFLAGS)

install: all
	-install -o 0 -g 0 -m 755 ssb-sprom $(PREFIX)/bin/

clean:
	-rm -f *~ *.o *.orig *.rej ssb-sprom

# dependencies
ssb_sprom.o: ssb_sprom.h utils.h
utils.o: utils.h ssb_sprom.h
