CC		?= gcc

PREFIX		?= /usr/local

CFLAGS		?= -O2 -fomit-frame-pointer
CFLAGS		+= -std=gnu99 -Wall -D_BSD_SOURCE -D_GNU_SOURCE
LDFLAGS		?=

BIN = b43-dasm
OBJECTS = main.o util.o args.o

all: $(BIN)

main.o: main.h util.h list.h args.h

util.o: util.h

args.o: args.h main.h util.h

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJECTS) $(LDFLAGS)

install: all
	-install -o 0 -g 0 -m 755 $(BIN) $(PREFIX)/bin/
	-install -o 0 -g 0 -m 755 b43-ivaldump $(PREFIX)/bin/

clean:
	-rm -f *~ *.o *.orig *.rej

distclean: clean
	-rm -f $(BIN)
