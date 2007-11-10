CC = gcc
PREFIX = /usr/local
CFLAGS = -std=gnu99 -O2 -fomit-frame-pointer -Wall -D_BSD_SOURCE -D_GNU_SOURCE
LDFLAGS =

BINARY = b43-dasm.bin
OBJECTS = main.o util.o

all: $(BINARY)

main.o: util.h list.h

util.o: util.h

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINARY) $(OBJECTS) $(LDFLAGS)

install: all
	-install -o 0 -g 0 -m 755 $(BINARY) $(PREFIX)/bin/
	-cp b43-dasm b43-dasm.inst
	-sed -i -e 's/installed=0/installed=1/' b43-dasm.inst
	-install -o 0 -g 0 -m 755 b43-dasm.inst $(PREFIX)/bin/b43-dasm
	-rm -f b43-dasm.inst

clean:
	-rm -f *~ *.o *.orig *.rej $(BINARY)
