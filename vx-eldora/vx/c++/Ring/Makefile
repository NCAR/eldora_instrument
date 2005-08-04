#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CCC= gcc68k
UCCC= gcc
#
OPT= -O2 -fno-builtin -nostdinc -nostdinc++
#
DEFINES=
#
INCLUDES= -I/vw/h
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= Ring.o 
#
SRCS= Ring.cc
#
HEADERS= Ring.hh
#
BINDIR= ../bin
#
INCDIR= ../include
# This include cuts around the GNU make use of CXX.
CXX = ${CCC}
CXXFLAGS = ${CCFLAGS}

.KEEP_STATE:

all: $(OBJS)

ringtst.o: objs

asm:
	for i in $(SRCS); \
		do (echo "$(CCC) -S $(CCFLAGS) $$i";$(CCC) -S $(CCFLAGS) $$i); \
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






