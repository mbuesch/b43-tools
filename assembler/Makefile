CC		= gcc
LEX		= flex
YACC		= bison

PREFIX		?= /usr/local

CFLAGS		?= -O2 -fomit-frame-pointer -D_BSD_SOURCE -D_GNU_SOURCE
CFLAGS		+= -std=c99 -Wall -D_BSD_SOURCE -D_GNU_SOURCE
LDFLAGS		+= -lfl

BIN		= b43-asm.bin
OBJECTS		= parser.o scanner.o main.o initvals.o util.o args.o

# YACC related CFLAGS
CFLAGS		+= -DYYSTYPE="void *" -DYYERROR_VERBOSE -DYYDEBUG -Wno-unused

all: $(BIN)

scanner.c: scanner.l parser.c main.h
	$(LEX) -o scanner.c --header-file=scanner.h scanner.l

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c -o scanner.o scanner.c

parser.c: parser.y main.h util.h
	$(YACC) --defines -o parser.c parser.y

parser.o: parser.c
	$(CC) $(CFLAGS) -c -o parser.o parser.c

main.o: parser.c main.h list.h util.h args.h initvals.h

initvals.o: initvals.h main.h list.h util.h args.h

util.o: util.h

args.o: args.h main.h util.h

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJECTS) $(LDFLAGS)

install: all
	-install -o 0 -g 0 -m 755 $(BIN) $(PREFIX)/bin/
	-cp b43-asm b43-asm.inst
	-sed -i -e 's/installed=0/installed=1/' b43-asm.inst
	-install -o 0 -g 0 -m 755 b43-asm.inst $(PREFIX)/bin/b43-asm
	-rm -f b43-asm.inst

clean:
	-rm -f *~ *.o *.orig *.rej scanner.c scanner.h parser.c parser.h

distclean: clean
	-rm -f $(BIN)
