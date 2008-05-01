#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C 
CC= gcc68k
LD = ld68k
#
# Agressive optimization, but need to keep pointers volatile (should fix code).
OPT= -O2 -fvolatile
#
DEFINES= -DTP41LIB -m68040 -fno-builtin -nostdinc
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I/vx/config/eldora.tp41 -I/vx/h/drv
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS= tp41Lib.o
#
OBJS= dport.o cio.o mbox.o
#
SRCS= cio.c dport.c mbox.c
#
HEADERS= tp41Lib.h
#
# Include files go here.
INC= /ELDORA/vx/include
#
# Binaries go here.
BIN= /ELDORA/vx/bin
#
# Libraries here.
LIBDIR= /ELDORA/lib/vx

.KEEP_STATE:

all: obj update

obj: $(OBJS)
	$(LD) -r -X -o $(LIBS) $(OBJS)
	ar68k cvr libtp41.a $(OBJS)
	mv libtp41.a $(LIBDIR)
	ranlib68k $(LIBDIR)/libtp41.a; \

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;
	cp $(LIBS) $(BIN)

asm:
	for i in $(SRCS); \
		do (echo "$(CC) -S $(CFLAGS) $$i";$(CC) -S $(CFLAGS) $$i); \
	done;

get_rcs:
	for i in $(SRCS); \
		do (echo "co $$i";co $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "co $$i";co $$i); \
	done;

get_rcs_locked:
	for i in $(SRCS); \
		do (echo "co -l $$i";co -l $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;

# Modify this to include an access list if desired.
set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






