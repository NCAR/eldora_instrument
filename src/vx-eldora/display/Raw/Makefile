#
#	Makefile for Raw displays
#	Author: Richard E. K. Neitzel
#	Date: 1 March 1991
#
# GNU C++
CCC= gcc68k
CXX= ${CCC}
LD= ld68k
AR= ar68k
#
# This where the ELDORA tree is rooted.
ELDORA= /ELDORA
#
OPT = -O2
#
DEFINES=
#
CC_INC=
#
INCLUDES= -I. -I/vx/h -I/local/include/vx++ -I../include
INCLUDES+= -I${ELDORA}/rpc/include -I/ELDORA/include -I/ELDORA/vx/math
INCLUDES+= -I/ELDORA/vx/c++/include
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CXXFLAGS= ${CCFLAGS}
#
CC_OBJS=
#
LIBS=
#
# Raw files.
ROBJS= Raw.o RawDraw.o
#
OBJS= $(ROBJS)
#
RSRCS= Raw.cc RawDraw.cc
#
SRCS= $(RSRCS)
#
RHDRS= Raw.hh
#
HEADERS= $(RHDRS)
#
LIBNAME= libRaw.a
#
INC= ../include
#
BIN= ../bin
#
LIB= ../lib
#

.KEEP_STATE:

all: obj update

update:
	$(AR) crv ${LIBNAME} $(OBJS)
	
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i ${INC}) \
		fi;); \
		rm tmpdiff; \
	done;
	cp ${LIBNAME} $(LIB)
	ranlib68k $(LIB)/${LIBNAME}

obj: $(OBJS)

depend:
	makedepend $(INCLUDES) $(SRCS)

rawt.o: rawtst.o
	ld68k -X -r -o $@ rawtst.o -L../lib -lRaw ../bin/Display.o \
	../bin/Clock.o ../bin/ColorConverter.o -lGraphics \
	-L/ELDORA/rpc/lib -lVxHeader ../bin/ParamNames.o \
	-L/local/lib/gcc-lib/68k/current -lstdc++ -lgcc

asm:
	for i in $(SRCS); \
		do (echo "$(CCC) -S $(CCFLAGS) $$i";$(CCC) -S $(CCFLAGS) $$i);\
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
	-for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;
	-for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;

put_rcs_new:
	ci -f2.0 $(SRCS) $(HEADERS)

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






# DO NOT DELETE THIS LINE -- make depend depends on it.
