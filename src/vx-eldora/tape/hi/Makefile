#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CCC= gcc
CC= ${CC}
LD= ld
#
OPT= -O2
#
DEFINES= -DNO_XVPS
#
ELDORA= /ELDORA
INCLUDES= -I${ELDORA}/include -I${ELDORA}/rpc/tape \
	-I${ELDORA}/rpc/include -I$(UITHOME)/include/c++ \
	-I/opt/x11r5/include
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CFLAGS= ${CCFLAGS}
#
LIBS= -L$(UITHOME)/lib -L/opt/x11r5/lib \
	/ELDORA/rpc/tape/libtape.a -lUIC -lxview -lolgx -lX11 -lg++ -lm
#
OBJS= Main.o BaseWdw.o RecordComm.o StatusWdw.o WarningWdw.o
#
SRCS= Main.cc BaseWdw.cc RecordComm.cc StatusWdw.cc WarningWdw.cc
#
HEADERS= BaseWdw.hh RecordComm.hh StatusWdw.hh WarningWdw.hh
#
INC=
#
BIN=
#
LIB=
#
LIBNAME=
#
# This include cuts around the GNU make use of CXX.
include /local/include/make/sun.cc.mk
#
.KEEP_STATE:

RecCtrl: ${OBJS}
	${CCC} -o $@ ${OBJS} ${LIBS}

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






