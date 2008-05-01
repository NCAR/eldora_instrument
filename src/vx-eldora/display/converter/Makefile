#
#	Makefile for ColorConverter
#	Author: R. Neitzel
#	Date: 9 Apr 91
#
# GNU C 
CXX= gcc68k
LD = ld68k
#
OPT= -O2 -fno-builtin -nostdinc -nostdinc++ -m68040
#
DEFINES=
#
INCLUDES= -I/vw/h -I../include -I/ELDORA/rpc/include -I/ELDORA/include \
	-I/local/include/vx++
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= ColorConverter.o
#
SRCS= ColorConverter.cc
#
HEADERS= ColorConverter.hh
#
.KEEP_STATE:
.MAIN: objs

objs: $(OBJS)
	cp ColorConverter.o ../bin
	-diff ../include/ColorConverter.hh ColorConverter.hh >tmpdiff
	if test -s tmpdiff ; \
	then \
	(echo "Updating file in ../include"; cp ColorConverter.hh ../include); \
	fi;
	rm -f tmpdiff

tst: objs contst.o

depend:
	makedepend $(INCLUDES) $(SRCS)

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
	for i in $(SRCS) $(HEADERS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;

put_rcs_new:
	ci -f2.0 $(SRCS) $(HEADERS)


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






# DO NOT DELETE THIS LINE -- make depend depends on it.

ColorConverter.o: ColorConverter.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
ColorConverter.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
ColorConverter.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
ColorConverter.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
ColorConverter.o: /vw/h/sys/types.h /vw/h/stdioLib.h /vw/h/stdio.h
ColorConverter.o: /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
ColorConverter.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h
ColorConverter.o: /vw/h/stdarg.h /ELDORA/rpc/include/CellSpacing.h
ColorConverter.o: /vw/h/rpc/rpc.h /vw/h/rpc/rpctypes.h /vw/h/sys/times.h
ColorConverter.o: /vw/h/netinet/in.h /vw/h/rpc/xdr.h /vw/h/rpc/rpctypes.h
ColorConverter.o: /vw/h/rpc/auth.h /vw/h/rpc/clnt.h /vw/h/rpc/rpc_msg.h
ColorConverter.o: /vw/h/rpc/auth_unix.h /vw/h/rpc/svc.h /vw/h/rpc/svc_auth.h
ColorConverter.o: ../include/DataPoint.h ../include/point.h
ColorConverter.o: ../include/WdwGbls.h /vw/h/math.h /vw/h/string.h
ColorConverter.o: /vw/h/taskLib.h /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h
ColorConverter.o: /vw/h/excLib.h /vw/h/private/semLibP.h /vw/h/semLib.h
ColorConverter.o: /vw/h/private/classLibP.h /vw/h/stdlib.h
