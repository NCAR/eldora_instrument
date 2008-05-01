#
#	Makefile for Radial displays
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
INCLUDES= -I. -I/vx/h -I/local/include/vx++ -I../include -I${ELDORA}/rpc/include \
	-I/ELDORA/include -I/ELDORA/vx/math
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CXXFLAGS= ${CCFLAGS}
#
CC_OBJS=
#
LIBS=
#
# Radial files.
ROBJS= Radial.o RadialDraw.o
#
# Data loop files.
LOOPOBJS= RadialDataLoop.o
#
OBJS= $(ROBJS)
#
RSRCS= Radial.cc RadialDraw.cc
#
LOOPSRCS= RadialDataLoop.cc
#
SRCS= $(RSRCS)
#
RHDRS= Radial.hh
#
LOOPHDRS=
#
HEADERS= $(RHDRS) $(LOOPHDRS)
#
LIBNAME= libRadial.a
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

r2tst: r2tst.o RadialDisp.o
	/locallib/g++68-ld -X -r -o test2.o r2tst.o RadialDisp.o \
	$(BINDIR)/GraphicLib.o /ELDORA/vx/c++/bin/builtin++.o \
	/ELDORA/vx/c++/bin/ctor_dummy.o $(BINDIR)/Clock.o

r4tst: r4tst.o RadialDisp.o
	${LD} -X -r -o test4.o r4tst.o RadialDisp.o \
	$(BINDIR)/GraphicLib.o /ELDORA/vx/c++/bin/builtin++.o \
	/ELDORA/vx/c++/bin/ctor_dummy.o $(BINDIR)/Clock.o

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

Radial.o: Radial.hh ../include/Display.hh ../include/Window.hh
Radial.o: ../include/GraphicController.hh /vx/h/vxWorks.h /vx/h/types/vxCpu.h
Radial.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
Radial.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
Radial.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
Radial.o: /vx/h/semLib.h /vx/h/vwModNum.h ../include/point.h
Radial.o: ../include/qpdm.h ../include/WdwGbls.h
Radial.o: ../include/GraphicControllerP.hh ../include/DataPoint.h
Radial.o: /vx/h/math.h /vx/h/string.h /local/include/vx++/iostream.h
Radial.o: /local/include/vx++/streambuf.h /local/include/vx++/libio.h
Radial.o: /local/include/vx++/_G_config.h /local/include/vx++/strstream.h
Radial.o: /local/include/vx++/strfile.h /ELDORA/rpc/include/DisplayRpc.hh
Radial.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
Radial.o: /vx/h/netinet/in.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
Radial.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
Radial.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
Radial.o: /vx/h/stdlib.h /ELDORA/rpc/include/DispCommands.h
Radial.o: /ELDORA/rpc/include/GblCommands.h /ELDORA/rpc/include/DispStatus.h
Radial.o: /ELDORA/rpc/include/GblStatus.h /ELDORA/rpc/include/Header.hh
Radial.o: /ELDORA/rpc/include/Header.h /ELDORA/rpc/include/TapeHeader.h
Radial.o: /ELDORA/rpc/include/CellSpacing.h /ELDORA/rpc/include/FieldRadar.h
Radial.o: /ELDORA/rpc/include/Parameter.h /ELDORA/rpc/include/RadarDesc.h
Radial.o: /ELDORA/rpc/include/Volume.h /ELDORA/rpc/include/Waveform.h
Radial.o: /ELDORA/rpc/include/InSitu.h /ELDORA/rpc/include/NavDesc.h
Radial.o: ../include/ParamNames.hh ../include/ColorConverter.hh
Radial.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h /vx/h/objLib.h
Radial.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
Radial.o: /vx/h/private/classLibP.h /vx/h/stdarg.h ../include/Clock.hh
Radial.o: ../include/DataBeam.hh /ELDORA/rpc/include/Ray.h
Radial.o: /ELDORA/rpc/include/Platform.h /ELDORA/rpc/include/FieldParam.h
Radial.o: /ELDORA/include/mathVar.h /ELDORA/vx/math/fastMath.h
Radial.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
Radial.o: /vx/h/excLib.h /vx/h/private/eventP.h /vx/h/private/semLibP.h
RadialDraw.o: Radial.hh ../include/Display.hh ../include/Window.hh
RadialDraw.o: ../include/GraphicController.hh /vx/h/vxWorks.h
RadialDraw.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
RadialDraw.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
RadialDraw.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
RadialDraw.o: /vx/h/semLib.h /vx/h/vwModNum.h ../include/point.h
RadialDraw.o: ../include/qpdm.h ../include/WdwGbls.h
RadialDraw.o: ../include/GraphicControllerP.hh ../include/DataPoint.h
RadialDraw.o: /vx/h/math.h /vx/h/string.h /local/include/vx++/iostream.h
RadialDraw.o: /local/include/vx++/streambuf.h /local/include/vx++/libio.h
RadialDraw.o: /local/include/vx++/_G_config.h /local/include/vx++/strstream.h
RadialDraw.o: /local/include/vx++/strfile.h /ELDORA/rpc/include/DisplayRpc.hh
RadialDraw.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
RadialDraw.o: /vx/h/netinet/in.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
RadialDraw.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
RadialDraw.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
RadialDraw.o: /vx/h/stdlib.h /ELDORA/rpc/include/DispCommands.h
RadialDraw.o: /ELDORA/rpc/include/GblCommands.h
RadialDraw.o: /ELDORA/rpc/include/DispStatus.h
RadialDraw.o: /ELDORA/rpc/include/GblStatus.h /ELDORA/rpc/include/Header.hh
RadialDraw.o: /ELDORA/rpc/include/Header.h /ELDORA/rpc/include/TapeHeader.h
RadialDraw.o: /ELDORA/rpc/include/CellSpacing.h
RadialDraw.o: /ELDORA/rpc/include/FieldRadar.h
RadialDraw.o: /ELDORA/rpc/include/Parameter.h /ELDORA/rpc/include/RadarDesc.h
RadialDraw.o: /ELDORA/rpc/include/Volume.h /ELDORA/rpc/include/Waveform.h
RadialDraw.o: /ELDORA/rpc/include/InSitu.h /ELDORA/rpc/include/NavDesc.h
RadialDraw.o: ../include/ParamNames.hh ../include/ColorConverter.hh
RadialDraw.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h /vx/h/objLib.h
RadialDraw.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
RadialDraw.o: /vx/h/private/classLibP.h /vx/h/stdarg.h ../include/Clock.hh
RadialDraw.o: ../include/DataBeam.hh /ELDORA/rpc/include/Ray.h
RadialDraw.o: /ELDORA/rpc/include/Platform.h /ELDORA/rpc/include/FieldParam.h
RadialDraw.o: /ELDORA/include/mathVar.h /ELDORA/vx/math/fastMath.h
