CC = gcc
PREFIX = /usr/local
CFLAGS = -std=gnu99 -O2 -fomit-frame-pointer -Wall -D_BSD_SOURCE -D_GNU_SOURCE
LDFLAGS =

BINARY = bcm43xx-dasm.bin
OBJECTS = main.o util.o

all: $(BINARY)

main.o: util.h list.h

util.o: util.h

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINARY) $(OBJECTS) $(LDFLAGS)

install: all
	-install -o 0 -g 0 -m 755 $(BINARY) $(PREFIX)/bin/
	-cp bcm43xx-dasm bcm43xx-dasm.inst
	-sed -i -e 's/installed=0/installed=1/' bcm43xx-dasm.inst
	-install -o 0 -g 0 -m 755 bcm43xx-dasm.inst $(PREFIX)/bin/bcm43xx-dasm
	-rm -f bcm43xx-dasm.inst

clean:
	-rm -f *~ *.o *.orig *.rej $(BINARY)
