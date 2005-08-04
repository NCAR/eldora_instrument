#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CCC= gcc68k
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vw/h -I../include
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= 
#
SRCS=
#
HEADERS= Wdog.hh
#
BINDIR= ../bin
#
INCDIR= ../include
#
CXX = ${CCC}
CXXFLAGS = ${CCFLAGS}
#
.KEEP_STATE:

all:

tst: wtst.o

asm:

get_rcs:
	for i in $(HEADERS); \
		do (echo "co $$i";co $$i); \
	done;

get_rcs_locked:
	for i in $(HEADERS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;

set_rcs:
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






