#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CXX= gcc68k
CC= ${CC}
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
# This where the ELDORA tree is rooted.
ELDORA= /ELDORA
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h -I/local/include/vx++ -I../include -I${ELDORA}/rpc/include \
	-I/ELDORA/include -I/ELDORA/vx/math
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CFLAGS= ${CCFLAGS}
#
LIBS=
#
OBJS= Vert.o VertDraw.o
#
SRCS= Vert.cc VertDraw.cc
#
HEADERS= Vert.hh
#
INC= ../include
#
BIN=
#
LIB= ../lib
#
LIBNAME= libVert.a
#
#
.KEEP_STATE:

all: lib update

lib: ${OBJS}
	${AR} crv ${LIBNAME} ${OBJS}

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;
	cp ${LIBNAME} ${LIB}
	${RANLIB} ${LIB}/${LIBNAME}

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






