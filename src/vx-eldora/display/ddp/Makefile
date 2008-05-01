#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CXX= gcc68k
#
#
ELDORA= /ELDORA
OPT= -O4
#
DEFINES=
#
INCLUDES= -I/vx/h -I../include -I/ELDORA/vx/c++/include -I/local/include/vx++
INCLUDES+= -I/ELDORA/vx/include -I/ELDORA/rpc/include
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= Ddp.o Mbp.o
#
SRCS= Ddp.cc Mbp.cc
#
HEADERS= Ddp.hh Mbp.hh
#
BINDIR= ../bin
#
INCDIR= ../include


.KEEP_STATE:
.MAIN: all

all: $(OBJS)
	cp $(OBJS) $(BINDIR)
	-for i in $(HEADERS); \
		do (diff $$i $(INCDIR)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i"; cp $$i $(INCDIR)) \
			fi;); \
	done;
	-rm tmpdiff

depend:
	makedepend $(INCLUDES) $(DEFINES) $(SRCS)

tst: ddptst.o $(OBJS)

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

Ddp.o: Ddp.hh /ELDORA/vx/c++/include/Pipe.hh /ELDORA/vx/c++/include/Ring.hh
Ddp.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
Ddp.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
Ddp.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
Ddp.o: /vx/h/memLib.h /vx/h/vwModNum.h /vx/h/string.h /vx/h/stdio.h
Ddp.o: /vx/h/classLib.h /vx/h/objLib.h /vx/h/errno.h /vx/h/private/objLibP.h
Ddp.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/stdlib.h /vx/h/semLib.h
Ddp.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
Ddp.o: /vx/h/excLib.h /vx/h/private/eventP.h /vx/h/private/semLibP.h
Ddp.o: /ELDORA/vx/c++/include/Isr.hh /vx/h/intLib.h
Ddp.o: /ELDORA/rpc/include/Header.hh /ELDORA/rpc/include/Header.h
Ddp.o: /ELDORA/rpc/include/TapeHeader.h /ELDORA/rpc/include/CellSpacing.h
Ddp.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
Ddp.o: /vx/h/netinet/in.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
Ddp.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
Ddp.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
Ddp.o: /ELDORA/rpc/include/FieldRadar.h /ELDORA/rpc/include/Parameter.h
Ddp.o: /ELDORA/rpc/include/RadarDesc.h /ELDORA/rpc/include/Volume.h
Ddp.o: /ELDORA/rpc/include/Waveform.h /ELDORA/rpc/include/InSitu.h
Ddp.o: /ELDORA/rpc/include/NavDesc.h /local/include/vx++/iostream.h
Ddp.o: /local/include/vx++/streambuf.h /local/include/vx++/libio.h
Ddp.o: /local/include/vx++/_G_config.h
Mbp.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
Mbp.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h Mbp.hh
Mbp.o: /ELDORA/vx/c++/include/Pipe.hh /ELDORA/vx/c++/include/Ring.hh
Mbp.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
Mbp.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
Mbp.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
Mbp.o: /vx/h/memLib.h /vx/h/vwModNum.h /vx/h/string.h /vx/h/stdio.h
Mbp.o: /vx/h/classLib.h /vx/h/objLib.h /vx/h/errno.h /vx/h/private/objLibP.h
Mbp.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/stdlib.h /vx/h/semLib.h
Mbp.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
Mbp.o: /vx/h/excLib.h /vx/h/private/eventP.h /vx/h/private/semLibP.h
Mbp.o: /ELDORA/vx/include/tp41Lib.h
