#
#	Makefile for Isr.cc
#	Author: Richard Neitzel
#	Date: 30 Nov 1993
#
# GNU C++ 
CCC= gcc68k
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= Isr.o
#
SRCS= Isr.cc
#
HEADERS= Isr.hh
#
INC= ../include
#
BIN= ../bin
#
# This include cuts around the GNU make use of CXX.
CXX= ${CCC}
CXXFLAGS = ${CCFLAGS}

.KEEP_STATE:
.MAIN: all

all: $(OBJS)

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






