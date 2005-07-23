#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CXX= gcc68k
LD = ld68k
AR= ar68k
#
# This where the ELDORA tree is rooted.
ELDORA= /ELDORA
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h -I/local/include/vx++ -I../include -I/ELDORA/include \
	-I${ELDORA}/rpc/include -I/ELDORA/vx/math
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= Dual.o DualDraw.o
#
SRCS= Dual.cc DualDraw.cc
#
HEADERS= Dual.hh
#
INC=../include
#
BIN=../bin
#
LIB=../lib
#
LIBNAME=libDual.a
#

.KEEP_STATE:

all: objs update

objs: $(OBJS)
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

dtst.o: objs dualtst.o
	$(LD) -r -X -o $@ dualtst.o $(OBJS) /ELDORA/vx/math/fastMath.o \
	../../converter/ColorConverter.o

debug:
	make DEFINES=-DNO_DISPLAY dualtst.o

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
