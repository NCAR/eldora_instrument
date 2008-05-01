#
#	Makefile for Mcpl
#	Author:
#	Date:
#
# GNU C 
CC= gcc68k
#
OPT= -O2
#
# SUN C
#CC= cc
#
#OPT= -O  -fast
#
DEFINES=
#
INCLUDES= -I/vx/h -I/ELDORA/rpc/tape
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= mad_xfer.o canned_xfer.o
#
SRCS= mad_xfer.c
#
HEADERS= mcpl_def.h
#
.KEEP_STATE:

obj: $(OBJS)

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






