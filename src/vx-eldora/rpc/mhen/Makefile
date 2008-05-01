#
#	Makefile for 
#	Author:
#	Date:
#
#include /local/include/make/sun.cc.mk
CCC= gcc
# GNU C++ 
VXCCC= gcc68k
#
OPT= -O2
#
EXTRA_DEFINES= 
DEFINES= -DUNIX ${EXTRA_DEFINES}
VXDEFINES= ${EXTRA_DEFINES}
#
INCLUDES= -I../include
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
VXFLAGS= ${OPT} ${INCLUDES} -I/vx/h -I/local/include/vx++ -I/ELDORA/vx/mhen/include -I/ELDORA/vx/hskp/include ${VXDEFINES}
#
include /local/include/make/sun.cc.mk
#
LIBS=
#
OBJS= RadarCmdClnt.o xdr_Radar.o
VXOBJS= RadarCmdSvr.o xdr68k_Radar.o
#
SRCS= RadarCmdClnt.cc xdr_Radar.cc RadarCmdSvr.cc
#
HEADERS= LclCommands.h RadarGbls.h RadarRpc.hh
#
INC= ../include
#
BIN= ../lib
#
.KEEP_STATE:

all: unix vx update

unix: $(OBJS)
	ar cvr libRadarRpc.a $(OBJS)
	install libRadarRpc.a $(BIN)


vx: RadarCmdSvr xdr68k_Radar
	ar68k cvr libVxRadarRpc.a RadarCmdSvr.o xdr68k_Radar.o
	install libVxRadarRpc.a $(BIN)
	ranlib68k ${BIN}/libVxRadarRpc.a 

RadarCmdSvr:
	$(VXCCC) -c ${VXFLAGS} RadarCmdSvr.cc

xdr68k_Radar:
	$(VXCCC) -c ${VXFLAGS} -o $@.o xdr68k_Radar.cc
	
ascCtrl: ascCtrl.o $(OBJS)
	${CCC} -s -o $@ ascCtrl.o -L. -lRadarRpc -lnsl -lrpcsvc -lg++

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;

tst:
	${VXCCC} ${VXFLAGS} -DDEBUG_ONLY -o tst.o -c RadarCmdSvr.cc
	ld68k -Ur -X -o mhentst.o tst.o -L. -lVxRadarRpc \
	-L/local/lib/gcc-lib/68k/current -lg++ -lgcc

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
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L -aeric $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L -aeric $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h

depend:
	makedepend ${INCLUDES} ${SRCS}
# DO NOT DELETE

# DO NOT DELETE THIS LINE -- make depend depends on it.
