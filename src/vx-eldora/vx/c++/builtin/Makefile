#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C 
CC= gcc68
#
OPT= -O2 -fno-builtin -nostdinc -nostdinc++
#
# SUN C
#CC= cc
#
#OPT= -O  -fast
#
DEFINES=
#
INCLUDES= -I/vx/h
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
LOBJS= b_new.o b_del.o set_new_handler.o ctor_dtor.o
#
DOBJ= ctor_dummy.o
#
SRCS= b_new.c b_del.c set_new_handler.c ctor_dtor.c ctor_dummy.s
#
HEADERS= builtin.h Gbl_Init_Dest.h Asynch.hh
#
.KEEP_STATE:

builtin++.o: $(LOBJS) $(DOBJ)
	ld68k -r -X -o $@ $(LOBJS)

install: builtin++.o install.headers
	mv builtin++.o ctor_dtor.o ../bin

install.headers:
	chmod 664 ../include/*
	cp *.h ../include
	chmod 444 ../include/*

ctor_dummy.o: ctor_dummy.s
	$(CC) -c ctor_dummy.s

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
