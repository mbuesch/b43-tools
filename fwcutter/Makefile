VERSION = 012

CC ?= cc
PREFIX ?= /usr/local
CFLAGS ?= -Os -fomit-frame-pointer
CFLAGS += -std=c99 -Wall -pedantic -D_BSD_SOURCE
LDFLAGS ?=


OBJECTS = fwcutter.o md5.o
BIN	= b43-fwcutter

CFLAGS += -DFWCUTTER_VERSION_=$(VERSION)

all: $(BIN)

fwcutter.o: fwcutter.h fwcutter_list.h md5.h

md5.o: md5.h

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJECTS) $(LDFLAGS)

install: all
	-install -d -o 0 -g 0 -m 755 $(PREFIX)/bin/
	-install -o 0 -g 0 -m 755 $(BIN) $(PREFIX)/bin/
	-install -d -o 0 -g 0 -m 755 $(PREFIX)/man/man1/
	-install -o 0 -g 0 -m 644 $(BIN).1 $(PREFIX)/man/man1/

clean:
	-rm -f *.o $(BIN)

distclean: clean
	-rm -f *.fw *.orig *.rej *~
