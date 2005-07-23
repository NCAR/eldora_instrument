#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CXX= gcc68k
LD= ld68k
AR= ar68k
#
# This where the ELDORA tree is rooted.
ELDORA= /ELDORA
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I. -I/vw/h -I/local/include/vx++ -I../include \
	-I${ELDORA}/rpc/include -I/ELDORA/include -I/ELDORA/vx/math
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= Horiz.o HorizDraw.o HorizMisc.o
#
SRCS= Horiz.cc HorizDraw.cc HorizMisc.cc
#
HEADERS= Horiz.hh
#
LIBNAME= libHoriz.a
#
#
INC= ../include
#
BIN= ../bin
#
LIB= ../lib
#
.KEEP_STATE:
.MAIN: all

all: obj update

obj: $(OBJS)
	$(AR) crv ${LIBNAME} $(OBJS)

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;
	cp ${LIBNAME} $(LIB)
	ranlib68k $(LIB)/${LIBNAME}

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
		do (echo "rcs -c\"// \" -L $$i";rcs -c"// " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\"// \" -L $$i";rcs -c"// " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






