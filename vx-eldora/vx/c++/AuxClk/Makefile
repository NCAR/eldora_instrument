#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CCC= gcc68k
CC= ${CCC}
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CFLAGS= ${CCFLAGS}
#
LIBS=
#
OBJS= AuxClk.o
#
SRCS= AuxClk.cc
#
HEADERS= AuxClk.hh
#
INC= ../include
#
BIN=
#
LIB= ../bin
#
LIBNAME= libAux.a
#
# This include cuts around the GNU make use of CXX.
CXX = ${CC}
CXXFLAGS = ${CCFLAGS}
#
.KEEP_STATE:

all: lib

lib: ${OBJS}
	${AR} crv ${LIBNAME} ${OBJS}
	${RANLIB} ${LIBNAME}

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






