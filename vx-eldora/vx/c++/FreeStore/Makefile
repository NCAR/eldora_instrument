#
#       Makefile for [Pacakge? ]
#       Author: Rich Neitzel
#       Date:   Fri Nov 18 1994
#
# GNU C++ 
CCC= gcc68k
CC= ${CC}
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h -I/local/include/vx++
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CFLAGS= ${CCFLAGS}
#
LIBS=
#
OBJS=FreeStore.o
#
SRCS=FreeStore.cc
#
HEADERS=FreeStore.hh
#
INC=../include
#
BIN=../bin
#
LIB=
#
LIBNAME=
#
# This include cuts around the GNU make use of CXX.
CXX= ${CCC}
CXXFLAGS = ${CCFLAGS}
#
.KEEP_STATE:

all: $(OBJS)

asm:
	for i in $(SRCS); \
		do (echo "$(CCC) -S $(CCFLAGS) $$i";$(CCC) -S $(CCFLAGS) $$i); \
	done;

depend:
	makedepend ${INCLUDES} ${SRCS}

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
		do (echo "rcs -c\"// \" -L $$i";rcs -c"// " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\"// \" -L $$i";rcs -c"// " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h
