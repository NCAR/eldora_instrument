#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CXX= gcc68k
CC = gcc
LD= ld68k
#
OPT= -O2 -m68040
#
DEFINES= # -DTEST_ONLY
#
# Warning !!!!! Hacked for dev work.
#
ELDORA= /ELDORA
#
INCLUDES= -I/vw/h -I${ELDORA}/include -I${ELDORA}/rpc/include \
	-I${ELDORA}/vx/c++/include -I../include -I../bitmaps \
	-I${ELDORA}/vx/include -I/vw/config/tp41 -I/vw/h/drv \
	-I${ELDORA}/vx/math -I/local/include/vx++ \
	-I/net/projects/common/include
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
COMM_DIR= /net/projects
#
LIBS= 	../bin/ColorConverter.o \
	../bin/ParamNames.o ../bin/Display.o \
	../bin/Clock.o ../bin/Ddp.o \
	${ELDORA}/vx/bin/tp41Lib.o \
	${ELDORA}/vx/c++/bin/Task.o ${ELDORA}/vx/c++/bin/Flags.o \
	${ELDORA}/vx/c++/bin/Ring.o ${ELDORA}/vx/c++/bin/Pipe.o \
	${ELDORA}/vx/c++/bin/Isr.o ${ELDORA}/vx/math/fastMath.o \
	-L${ELDORA}/rpc/lib -lVxDispRpc -lVxHeader \
	-L../lib -lHoriz -lDual -lRadial -lVert -lRaw -lGraphics \
	-L${COMM_DIR}/common/lib/vxworks -lNet \
	-L/local/lib/gcc-lib/m68k-wrs-vxworks/current -lstdc++	-lgcc

GEOBJS= GeInit.o GeCtrl.o GeDraw.o 
LOOPOBJS= DisplayLoop.o
MOUSEOBJS= FieldMouse.o
OBJS= $(GEOBJS) $(LOOPOBJS) $(MOUSEOBJS)
#
SRCS= GeInit.cc GeCtrl.cc GeAlarm.cc GeDraw.cc DisplayLoop.cc \
	 FieldMouse.cc 
#
HEADERS= GeGlobal.hh GeAlarm.hh GeDraw.hh
#
.KEEP_STATE:

.MAIN: Ge.o

Ge.o: clear geobjs loopobjs mouseobjs
	$(LD) -X -Ur -o $@ $(OBJS) $(LIBS)

geobjs: $(GEOBJS)
loopobjs: $(LOOPOBJS)
mouseobjs: $(MOUSEOBJS)

depend:
	makedepend $(INCLUDES) $(SRCS)

clear:
	-rm Ge.o

test: tstem.o
	-rm test.o
	$(LD) -Ur -X -o test.o tstem.o $(OBJS) $(LIBS)

fakeHeader: fakeHeader.o
	${CC} -s -o $@ fakeHeader.o -L${ELDORA}/rpc/lib -lHeader \
	-lnsl -lrpcsvc -lg++
#	-L${ELDORA}/rpc/lib -lHeader -lnsl -lrpcsvc -lg++

fakeHeader.o:
	${CC} -O2 -I${ELDORA}/include -I${ELDORA}/rpc/include -c fakeHeader.cc
#	-I${ELDORA}/rpc/include -c fakeHeader.cc

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

GeInit.o: GeGlobal.hh ${ELDORA}/rpc/include/DisplayRpc.h
GeInit.o: ${ELDORA}/rpc/include/TapeHeader.h
GeInit.o: ${ELDORA}/rpc/include/CellSpacing.h /vw/h/rpc/rpc.h
GeInit.o: /vw/h/rpc/rpctypes.h /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GeInit.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GeInit.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GeInit.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
GeInit.o: /vw/h/sys/times.h /vw/h/netinet/in.h /vw/h/rpc/xdr.h
GeInit.o: /vw/h/rpc/rpctypes.h /vw/h/rpc/auth.h /vw/h/rpc/clnt.h
GeInit.o: /vw/h/rpc/rpc_msg.h /vw/h/rpc/auth_unix.h /vw/h/rpc/svc.h
GeInit.o: /vw/h/rpc/svc_auth.h ${ELDORA}/rpc/include/FieldRadar.h
GeInit.o: ${ELDORA}/rpc/include/Parameter.h
GeInit.o: ${ELDORA}/rpc/include/RadarDesc.h
GeInit.o: ${ELDORA}/rpc/include/Volume.h
GeInit.o: ${ELDORA}/rpc/include/Waveform.h
GeInit.o: ${ELDORA}/rpc/include/InSitu.h
GeInit.o: ${ELDORA}/rpc/include/NavDesc.h
GeInit.o: ${ELDORA}/rpc/include/Header.hh
GeInit.o: ${ELDORA}/rpc/include/Header.h
GeInit.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
GeInit.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
GeInit.o: ${ELDORA}/vx/c++/include/Task.hh /vw/h/taskLib.h /vw/h/vwModNum.h
GeInit.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
GeInit.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
GeInit.o: /vw/h/private/semLibP.h /vw/h/semLib.h /vw/h/private/classLibP.h
GeInit.o: /vw/h/private/objLibP.h /vw/h/stdlib.h /vw/h/sigLib.h
GeInit.o: /vw/h/signal.h /vw/h/sigevent.h /vw/h/sigCodes.h /vw/h/unistd.h
GeInit.o: /vw/h/private/sigLibP.h /vw/h/private/funcBindP.h /vw/h/string.h
GeInit.o: ${ELDORA}/vx/c++/include/Flags.hh /vw/h/stdio.h /vw/h/stdarg.h
GeInit.o: ../include/Mouse.hh ../include/point.h
GeInit.o: ${ELDORA}/vx/c++/include/Isr.hh /vw/h/intLib.h ../include/MouseP.hh
GeInit.o: ../include/Display.hh ../include/Window.hh
GeInit.o: ../include/GraphicController.hh ../include/qpdm.h
GeInit.o: ../include/WdwGbls.h ../include/GraphicControllerP.hh
GeInit.o: ../include/DataPoint.h /vw/h/math.h
GeInit.o: ${ELDORA}/rpc/include/DisplayRpc.hh
GeInit.o: ${ELDORA}/rpc/include/DispCommands.h
GeInit.o: ${ELDORA}/rpc/include/GblCommands.h
GeInit.o: ${ELDORA}/rpc/include/DispStatus.h
GeInit.o: ${ELDORA}/rpc/include/GblStatus.h ../include/ParamNames.hh
GeInit.o: ../include/ColorConverter.hh /vw/h/stdioLib.h ../include/Clock.hh
GeInit.o: ../include/DataBeam.hh ${ELDORA}/rpc/include/Ray.h
GeInit.o: ${ELDORA}/rpc/include/Platform.h
GeInit.o: ${ELDORA}/rpc/include/FieldParam.h ${ELDORA}/include/mathVar.h
GeInit.o: ${ELDORA}/vx/include/fastMath.h ../include/Ddp.hh
GeInit.o: ${ELDORA}/vx/c++/include/Pipe.hh ${ELDORA}/vx/c++/include/Ring.hh
GeInit.o: ${ELDORA}/rpc/include/HeaderRpc.h /vw/h/rpcLib.h
GeInit.o: ${ELDORA}/vx/include/tp41Lib.h /vw/h/sysLib.h /vw/h/bootLib.h
GeCtrl.o: GeGlobal.hh ${ELDORA}/rpc/include/DisplayRpc.h
GeCtrl.o: ${ELDORA}/rpc/include/TapeHeader.h
GeCtrl.o: ${ELDORA}/rpc/include/CellSpacing.h /vw/h/rpc/rpc.h
GeCtrl.o: /vw/h/rpc/rpctypes.h /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GeCtrl.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GeCtrl.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GeCtrl.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
GeCtrl.o: /vw/h/sys/times.h /vw/h/netinet/in.h /vw/h/rpc/xdr.h
GeCtrl.o: /vw/h/rpc/rpctypes.h /vw/h/rpc/auth.h /vw/h/rpc/clnt.h
GeCtrl.o: /vw/h/rpc/rpc_msg.h /vw/h/rpc/auth_unix.h /vw/h/rpc/svc.h
GeCtrl.o: /vw/h/rpc/svc_auth.h ${ELDORA}/rpc/include/FieldRadar.h
GeCtrl.o: ${ELDORA}/rpc/include/Parameter.h
GeCtrl.o: ${ELDORA}/rpc/include/RadarDesc.h
GeCtrl.o: ${ELDORA}/rpc/include/Volume.h
GeCtrl.o: ${ELDORA}/rpc/include/Waveform.h
GeCtrl.o: ${ELDORA}/rpc/include/InSitu.h
GeCtrl.o: ${ELDORA}/rpc/include/NavDesc.h
GeCtrl.o: ${ELDORA}/rpc/include/Header.hh
GeCtrl.o: ${ELDORA}/rpc/include/Header.h
GeCtrl.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
GeCtrl.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
GeCtrl.o: ${ELDORA}/vx/c++/include/Task.hh /vw/h/taskLib.h /vw/h/vwModNum.h
GeCtrl.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
GeCtrl.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
GeCtrl.o: /vw/h/private/semLibP.h /vw/h/semLib.h /vw/h/private/classLibP.h
GeCtrl.o: /vw/h/private/objLibP.h /vw/h/stdlib.h /vw/h/sigLib.h
GeCtrl.o: /vw/h/signal.h /vw/h/sigevent.h /vw/h/sigCodes.h /vw/h/unistd.h
GeCtrl.o: /vw/h/private/sigLibP.h /vw/h/private/funcBindP.h /vw/h/string.h
GeCtrl.o: ${ELDORA}/vx/c++/include/Flags.hh /vw/h/stdio.h /vw/h/stdarg.h
GeCtrl.o: ../include/Mouse.hh ../include/point.h
GeCtrl.o: ${ELDORA}/vx/c++/include/Isr.hh /vw/h/intLib.h ../include/MouseP.hh
GeCtrl.o: ../include/Display.hh ../include/Window.hh
GeCtrl.o: ../include/GraphicController.hh ../include/qpdm.h
GeCtrl.o: ../include/WdwGbls.h ../include/GraphicControllerP.hh
GeCtrl.o: ../include/DataPoint.h /vw/h/math.h
GeCtrl.o: ${ELDORA}/rpc/include/DisplayRpc.hh
GeCtrl.o: ${ELDORA}/rpc/include/DispCommands.h
GeCtrl.o: ${ELDORA}/rpc/include/GblCommands.h
GeCtrl.o: ${ELDORA}/rpc/include/DispStatus.h
GeCtrl.o: ${ELDORA}/rpc/include/GblStatus.h ../include/ParamNames.hh
GeCtrl.o: ../include/ColorConverter.hh /vw/h/stdioLib.h ../include/Clock.hh
GeCtrl.o: ../include/DataBeam.hh ${ELDORA}/rpc/include/Ray.h
GeCtrl.o: ${ELDORA}/rpc/include/Platform.h
GeCtrl.o: ${ELDORA}/rpc/include/FieldParam.h ${ELDORA}/include/mathVar.h
GeCtrl.o: ${ELDORA}/vx/include/fastMath.h ../include/Ddp.hh
GeCtrl.o: ${ELDORA}/vx/c++/include/Pipe.hh ${ELDORA}/vx/c++/include/Ring.hh
GeHeader.o: GeGlobal.hh ${ELDORA}/rpc/include/DisplayRpc.h
GeHeader.o: ${ELDORA}/rpc/include/TapeHeader.h
GeHeader.o: ${ELDORA}/rpc/include/CellSpacing.h /vw/h/rpc/rpc.h
GeHeader.o: /vw/h/rpc/rpctypes.h /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GeHeader.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GeHeader.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GeHeader.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
GeHeader.o: /vw/h/sys/times.h /vw/h/netinet/in.h /vw/h/rpc/xdr.h
GeHeader.o: /vw/h/rpc/rpctypes.h /vw/h/rpc/auth.h /vw/h/rpc/clnt.h
GeHeader.o: /vw/h/rpc/rpc_msg.h /vw/h/rpc/auth_unix.h /vw/h/rpc/svc.h
GeHeader.o: /vw/h/rpc/svc_auth.h ${ELDORA}/rpc/include/FieldRadar.h
GeHeader.o: ${ELDORA}/rpc/include/Parameter.h
GeHeader.o: ${ELDORA}/rpc/include/RadarDesc.h
GeHeader.o: ${ELDORA}/rpc/include/Volume.h
GeHeader.o: ${ELDORA}/rpc/include/Waveform.h
GeHeader.o: ${ELDORA}/rpc/include/InSitu.h
GeHeader.o: ${ELDORA}/rpc/include/NavDesc.h
GeHeader.o: ${ELDORA}/rpc/include/Header.hh
GeHeader.o: ${ELDORA}/rpc/include/Header.h
GeHeader.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
GeHeader.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
GeHeader.o: ${ELDORA}/vx/c++/include/Task.hh /vw/h/taskLib.h /vw/h/vwModNum.h
GeHeader.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
GeHeader.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
GeHeader.o: /vw/h/private/semLibP.h /vw/h/semLib.h /vw/h/private/classLibP.h
GeHeader.o: /vw/h/private/objLibP.h /vw/h/stdlib.h /vw/h/sigLib.h
GeHeader.o: /vw/h/signal.h /vw/h/sigevent.h /vw/h/sigCodes.h /vw/h/unistd.h
GeHeader.o: /vw/h/private/sigLibP.h /vw/h/private/funcBindP.h /vw/h/string.h
GeHeader.o: ${ELDORA}/vx/c++/include/Flags.hh /vw/h/stdio.h /vw/h/stdarg.h
GeHeader.o: ../include/Mouse.hh ../include/point.h
GeHeader.o: ${ELDORA}/vx/c++/include/Isr.hh /vw/h/intLib.h ../include/MouseP.hh
GeHeader.o: ../include/Display.hh ../include/Window.hh
GeHeader.o: ../include/GraphicController.hh ../include/qpdm.h
GeHeader.o: ../include/WdwGbls.h ../include/GraphicControllerP.hh
GeHeader.o: ../include/DataPoint.h /vw/h/math.h
GeHeader.o: ${ELDORA}/rpc/include/DisplayRpc.hh
GeHeader.o: ${ELDORA}/rpc/include/DispCommands.h
GeHeader.o: ${ELDORA}/rpc/include/GblCommands.h
GeHeader.o: ${ELDORA}/rpc/include/DispStatus.h
GeHeader.o: ${ELDORA}/rpc/include/GblStatus.h ../include/ParamNames.hh
GeHeader.o: ../include/ColorConverter.hh /vw/h/stdioLib.h ../include/Clock.hh
GeHeader.o: ../include/DataBeam.hh ${ELDORA}/rpc/include/Ray.h
GeHeader.o: ${ELDORA}/rpc/include/Platform.h
GeHeader.o: ${ELDORA}/rpc/include/FieldParam.h
GeHeader.o: ${ELDORA}/include/mathVar.h ${ELDORA}/vx/include/fastMath.h
GeHeader.o: ../include/Ddp.hh ${ELDORA}/vx/c++/include/Pipe.hh
GeHeader.o: ${ELDORA}/vx/c++/include/Ring.hh
GeAlarm.o: GeAlarm.hh GeGlobal.hh ${ELDORA}/rpc/include/DisplayRpc.h
GeAlarm.o: ${ELDORA}/rpc/include/TapeHeader.h
GeAlarm.o: ${ELDORA}/rpc/include/CellSpacing.h /vw/h/rpc/rpc.h
GeAlarm.o: /vw/h/rpc/rpctypes.h /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GeAlarm.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GeAlarm.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GeAlarm.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
GeAlarm.o: /vw/h/sys/times.h /vw/h/netinet/in.h /vw/h/rpc/xdr.h
GeAlarm.o: /vw/h/rpc/rpctypes.h /vw/h/rpc/auth.h /vw/h/rpc/clnt.h
GeAlarm.o: /vw/h/rpc/rpc_msg.h /vw/h/rpc/auth_unix.h /vw/h/rpc/svc.h
GeAlarm.o: /vw/h/rpc/svc_auth.h ${ELDORA}/rpc/include/FieldRadar.h
GeAlarm.o: ${ELDORA}/rpc/include/Parameter.h
GeAlarm.o: ${ELDORA}/rpc/include/RadarDesc.h
GeAlarm.o: ${ELDORA}/rpc/include/Volume.h
GeAlarm.o: ${ELDORA}/rpc/include/Waveform.h
GeAlarm.o: ${ELDORA}/rpc/include/InSitu.h
GeAlarm.o: ${ELDORA}/rpc/include/NavDesc.h
GeAlarm.o: ${ELDORA}/rpc/include/Header.hh
GeAlarm.o: ${ELDORA}/rpc/include/Header.h
GeAlarm.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
GeAlarm.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
GeAlarm.o: ${ELDORA}/vx/c++/include/Task.hh /vw/h/taskLib.h /vw/h/vwModNum.h
GeAlarm.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
GeAlarm.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
GeAlarm.o: /vw/h/private/semLibP.h /vw/h/semLib.h /vw/h/private/classLibP.h
GeAlarm.o: /vw/h/private/objLibP.h /vw/h/stdlib.h /vw/h/sigLib.h
GeAlarm.o: /vw/h/signal.h /vw/h/sigevent.h /vw/h/sigCodes.h /vw/h/unistd.h
GeAlarm.o: /vw/h/private/sigLibP.h /vw/h/private/funcBindP.h /vw/h/string.h
GeAlarm.o: ${ELDORA}/vx/c++/include/Flags.hh /vw/h/stdio.h /vw/h/stdarg.h
GeAlarm.o: ../include/Mouse.hh ../include/point.h
GeAlarm.o: ${ELDORA}/vx/c++/include/Isr.hh /vw/h/intLib.h ../include/MouseP.hh
GeAlarm.o: ../include/Display.hh ../include/Window.hh
GeAlarm.o: ../include/GraphicController.hh ../include/qpdm.h
GeAlarm.o: ../include/WdwGbls.h ../include/GraphicControllerP.hh
GeAlarm.o: ../include/DataPoint.h /vw/h/math.h
GeAlarm.o: ${ELDORA}/rpc/include/DisplayRpc.hh
GeAlarm.o: ${ELDORA}/rpc/include/DispCommands.h
GeAlarm.o: ${ELDORA}/rpc/include/GblCommands.h
GeAlarm.o: ${ELDORA}/rpc/include/DispStatus.h
GeAlarm.o: ${ELDORA}/rpc/include/GblStatus.h ../include/ParamNames.hh
GeAlarm.o: ../include/ColorConverter.hh /vw/h/stdioLib.h ../include/Clock.hh
GeAlarm.o: ../include/DataBeam.hh ${ELDORA}/rpc/include/Ray.h
GeAlarm.o: ${ELDORA}/rpc/include/Platform.h
GeAlarm.o: ${ELDORA}/rpc/include/FieldParam.h
GeAlarm.o: ${ELDORA}/include/mathVar.h ${ELDORA}/vx/include/fastMath.h
GeAlarm.o: ../include/Ddp.hh ${ELDORA}/vx/c++/include/Pipe.hh
GeAlarm.o: ${ELDORA}/vx/c++/include/Ring.hh /vw/h/hostLib.h /vw/h/lstLib.h
GeAlarm.o: /vw/h/rpcLib.h /vw/h/socket.h /vw/h/sys/socket.h /vw/h/ioLib.h
GeAlarm.o: /vw/h/limits.h /vw/h/net/uio.h /vw/h/fcntl.h /vw/h/sys/fcntlcom.h
GeDraw.o: GeGlobal.hh ${ELDORA}/rpc/include/DisplayRpc.h
GeDraw.o: ${ELDORA}/rpc/include/TapeHeader.h
GeDraw.o: ${ELDORA}/rpc/include/CellSpacing.h /vw/h/rpc/rpc.h
GeDraw.o: /vw/h/rpc/rpctypes.h /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GeDraw.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GeDraw.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GeDraw.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
GeDraw.o: /vw/h/sys/times.h /vw/h/netinet/in.h /vw/h/rpc/xdr.h
GeDraw.o: /vw/h/rpc/rpctypes.h /vw/h/rpc/auth.h /vw/h/rpc/clnt.h
GeDraw.o: /vw/h/rpc/rpc_msg.h /vw/h/rpc/auth_unix.h /vw/h/rpc/svc.h
GeDraw.o: /vw/h/rpc/svc_auth.h ${ELDORA}/rpc/include/FieldRadar.h
GeDraw.o: ${ELDORA}/rpc/include/Parameter.h
GeDraw.o: ${ELDORA}/rpc/include/RadarDesc.h
GeDraw.o: ${ELDORA}/rpc/include/Volume.h
GeDraw.o: ${ELDORA}/rpc/include/Waveform.h
GeDraw.o: ${ELDORA}/rpc/include/InSitu.h
GeDraw.o: ${ELDORA}/rpc/include/NavDesc.h
GeDraw.o: ${ELDORA}/rpc/include/Header.hh
GeDraw.o: ${ELDORA}/rpc/include/Header.h
GeDraw.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
GeDraw.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
GeDraw.o: ${ELDORA}/vx/c++/include/Task.hh /vw/h/taskLib.h /vw/h/vwModNum.h
GeDraw.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
GeDraw.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
GeDraw.o: /vw/h/private/semLibP.h /vw/h/semLib.h /vw/h/private/classLibP.h
GeDraw.o: /vw/h/private/objLibP.h /vw/h/stdlib.h /vw/h/sigLib.h
GeDraw.o: /vw/h/signal.h /vw/h/sigevent.h /vw/h/sigCodes.h /vw/h/unistd.h
GeDraw.o: /vw/h/private/sigLibP.h /vw/h/private/funcBindP.h /vw/h/string.h
GeDraw.o: ${ELDORA}/vx/c++/include/Flags.hh /vw/h/stdio.h /vw/h/stdarg.h
GeDraw.o: ../include/Mouse.hh ../include/point.h
GeDraw.o: ${ELDORA}/vx/c++/include/Isr.hh /vw/h/intLib.h ../include/MouseP.hh
GeDraw.o: ../include/Display.hh ../include/Window.hh
GeDraw.o: ../include/GraphicController.hh ../include/qpdm.h
GeDraw.o: ../include/WdwGbls.h ../include/GraphicControllerP.hh
GeDraw.o: ../include/DataPoint.h /vw/h/math.h
GeDraw.o: ${ELDORA}/rpc/include/DisplayRpc.hh
GeDraw.o: ${ELDORA}/rpc/include/DispCommands.h
GeDraw.o: ${ELDORA}/rpc/include/GblCommands.h
GeDraw.o: ${ELDORA}/rpc/include/DispStatus.h
GeDraw.o: ${ELDORA}/rpc/include/GblStatus.h ../include/ParamNames.hh
GeDraw.o: ../include/ColorConverter.hh /vw/h/stdioLib.h ../include/Clock.hh
GeDraw.o: ../include/DataBeam.hh ${ELDORA}/rpc/include/Ray.h
GeDraw.o: ${ELDORA}/rpc/include/Platform.h
GeDraw.o: ${ELDORA}/rpc/include/FieldParam.h ${ELDORA}/include/mathVar.h
GeDraw.o: ${ELDORA}/vx/include/fastMath.h ../include/Ddp.hh
GeDraw.o: ${ELDORA}/vx/c++/include/Pipe.hh ${ELDORA}/vx/c++/include/Ring.hh
GeDraw.o: GeDraw.hh ../include/Radial.hh ../include/Vert.hh
GeDraw.o: ../include/Dual.hh ../include/Horiz.hh /vw/h/sysLib.h
GeDraw.o: /vw/h/bootLib.h /vw/h/rebootLib.h ${ELDORA}/vx/include/tp41Lib.h
GeDraw.o: ../bitmaps/boxed_cross.h
DisplayLoop.o: GeGlobal.hh ${ELDORA}/rpc/include/DisplayRpc.h
DisplayLoop.o: ${ELDORA}/rpc/include/TapeHeader.h
DisplayLoop.o: ${ELDORA}/rpc/include/CellSpacing.h /vw/h/rpc/rpc.h
DisplayLoop.o: /vw/h/rpc/rpctypes.h /vw/h/vxWorks.h /vw/h/types/vxCpu.h
DisplayLoop.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
DisplayLoop.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
DisplayLoop.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
DisplayLoop.o: /vw/h/sys/types.h /vw/h/sys/times.h /vw/h/netinet/in.h
DisplayLoop.o: /vw/h/rpc/xdr.h /vw/h/rpc/rpctypes.h /vw/h/rpc/auth.h
DisplayLoop.o: /vw/h/rpc/clnt.h /vw/h/rpc/rpc_msg.h /vw/h/rpc/auth_unix.h
DisplayLoop.o: /vw/h/rpc/svc.h /vw/h/rpc/svc_auth.h
DisplayLoop.o: ${ELDORA}/rpc/include/FieldRadar.h
DisplayLoop.o: ${ELDORA}/rpc/include/Parameter.h
DisplayLoop.o: ${ELDORA}/rpc/include/RadarDesc.h
DisplayLoop.o: ${ELDORA}/rpc/include/Volume.h
DisplayLoop.o: ${ELDORA}/rpc/include/Waveform.h
DisplayLoop.o: ${ELDORA}/rpc/include/InSitu.h
DisplayLoop.o: ${ELDORA}/rpc/include/NavDesc.h
DisplayLoop.o: ${ELDORA}/rpc/include/Header.hh
DisplayLoop.o: ${ELDORA}/rpc/include/Header.h
DisplayLoop.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
DisplayLoop.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
DisplayLoop.o: ${ELDORA}/vx/c++/include/Task.hh /vw/h/taskLib.h
DisplayLoop.o: /vw/h/vwModNum.h /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h
DisplayLoop.o: /vw/h/memLib.h /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h
DisplayLoop.o: /vw/h/excLib.h /vw/h/private/semLibP.h /vw/h/semLib.h
DisplayLoop.o: /vw/h/private/classLibP.h /vw/h/private/objLibP.h
DisplayLoop.o: /vw/h/stdlib.h /vw/h/sigLib.h /vw/h/signal.h /vw/h/sigevent.h
DisplayLoop.o: /vw/h/sigCodes.h /vw/h/unistd.h /vw/h/private/sigLibP.h
DisplayLoop.o: /vw/h/private/funcBindP.h /vw/h/string.h
DisplayLoop.o: ${ELDORA}/vx/c++/include/Flags.hh /vw/h/stdio.h /vw/h/stdarg.h
DisplayLoop.o: ../include/Mouse.hh ../include/point.h
DisplayLoop.o: ${ELDORA}/vx/c++/include/Isr.hh /vw/h/intLib.h
DisplayLoop.o: ../include/MouseP.hh ../include/Display.hh
DisplayLoop.o: ../include/Window.hh ../include/GraphicController.hh
DisplayLoop.o: ../include/qpdm.h ../include/WdwGbls.h
DisplayLoop.o: ../include/GraphicControllerP.hh ../include/DataPoint.h
DisplayLoop.o: /vw/h/math.h ${ELDORA}/rpc/include/DisplayRpc.hh
DisplayLoop.o: ${ELDORA}/rpc/include/DispCommands.h
DisplayLoop.o: ${ELDORA}/rpc/include/GblCommands.h
DisplayLoop.o: ${ELDORA}/rpc/include/DispStatus.h
DisplayLoop.o: ${ELDORA}/rpc/include/GblStatus.h
DisplayLoop.o: ../include/ParamNames.hh ../include/ColorConverter.hh
DisplayLoop.o: /vw/h/stdioLib.h ../include/Clock.hh ../include/DataBeam.hh
DisplayLoop.o: ${ELDORA}/rpc/include/Ray.h
DisplayLoop.o: ${ELDORA}/rpc/include/Platform.h
DisplayLoop.o: ${ELDORA}/rpc/include/FieldParam.h
DisplayLoop.o: ${ELDORA}/include/mathVar.h ${ELDORA}/vx/include/fastMath.h
DisplayLoop.o: ../include/Ddp.hh ${ELDORA}/vx/c++/include/Pipe.hh
DisplayLoop.o: ${ELDORA}/vx/c++/include/Ring.hh GeDraw.hh ../include/Radial.hh
DisplayLoop.o: ../include/Vert.hh ../include/Dual.hh ../include/Horiz.hh
DisplayLoop.o: /vw/h/sysLib.h /vw/h/bootLib.h
FieldMouse.o: ../include/Mouse.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
FieldMouse.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
FieldMouse.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
FieldMouse.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
FieldMouse.o: /vw/h/semLib.h /vw/h/vwModNum.h /vw/h/taskLib.h
FieldMouse.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
FieldMouse.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
FieldMouse.o: /vw/h/private/semLibP.h /vw/h/private/classLibP.h
FieldMouse.o: /vw/h/private/objLibP.h /vw/h/stdlib.h ../include/point.h
FieldMouse.o: ${ELDORA}/vx/c++/include/Isr.hh /vw/h/intLib.h
FieldMouse.o: ../include/MouseP.hh GeGlobal.hh
FieldMouse.o: ${ELDORA}/rpc/include/DisplayRpc.h
FieldMouse.o: ${ELDORA}/rpc/include/TapeHeader.h
FieldMouse.o: ${ELDORA}/rpc/include/CellSpacing.h /vw/h/rpc/rpc.h
FieldMouse.o: /vw/h/rpc/rpctypes.h /vw/h/sys/times.h /vw/h/netinet/in.h
FieldMouse.o: /vw/h/rpc/xdr.h /vw/h/rpc/rpctypes.h /vw/h/rpc/auth.h
FieldMouse.o: /vw/h/rpc/clnt.h /vw/h/rpc/rpc_msg.h /vw/h/rpc/auth_unix.h
FieldMouse.o: /vw/h/rpc/svc.h /vw/h/rpc/svc_auth.h
FieldMouse.o: ${ELDORA}/rpc/include/FieldRadar.h
FieldMouse.o: ${ELDORA}/rpc/include/Parameter.h
FieldMouse.o: ${ELDORA}/rpc/include/RadarDesc.h
FieldMouse.o: ${ELDORA}/rpc/include/Volume.h
FieldMouse.o: ${ELDORA}/rpc/include/Waveform.h
FieldMouse.o: ${ELDORA}/rpc/include/InSitu.h
FieldMouse.o: ${ELDORA}/rpc/include/NavDesc.h
FieldMouse.o: ${ELDORA}/rpc/include/Header.hh
FieldMouse.o: ${ELDORA}/rpc/include/Header.h
FieldMouse.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
FieldMouse.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
FieldMouse.o: ${ELDORA}/vx/c++/include/Task.hh /vw/h/sigLib.h /vw/h/signal.h
FieldMouse.o: /vw/h/sigevent.h /vw/h/sigCodes.h /vw/h/unistd.h
FieldMouse.o: /vw/h/private/sigLibP.h /vw/h/private/funcBindP.h
FieldMouse.o: /vw/h/string.h ${ELDORA}/vx/c++/include/Flags.hh /vw/h/stdio.h
FieldMouse.o: /vw/h/stdarg.h ../include/Display.hh ../include/Window.hh
FieldMouse.o: ../include/GraphicController.hh ../include/qpdm.h
FieldMouse.o: ../include/WdwGbls.h ../include/GraphicControllerP.hh
FieldMouse.o: ../include/DataPoint.h /vw/h/math.h
FieldMouse.o: ${ELDORA}/rpc/include/DisplayRpc.hh
FieldMouse.o: ${ELDORA}/rpc/include/DispCommands.h
FieldMouse.o: ${ELDORA}/rpc/include/GblCommands.h
FieldMouse.o: ${ELDORA}/rpc/include/DispStatus.h
FieldMouse.o: ${ELDORA}/rpc/include/GblStatus.h
FieldMouse.o: ../include/ParamNames.hh ../include/ColorConverter.hh
FieldMouse.o: /vw/h/stdioLib.h ../include/Clock.hh ../include/DataBeam.hh
FieldMouse.o: ${ELDORA}/rpc/include/Ray.h
FieldMouse.o: ${ELDORA}/rpc/include/Platform.h
FieldMouse.o: ${ELDORA}/rpc/include/FieldParam.h
FieldMouse.o: ${ELDORA}/include/mathVar.h ${ELDORA}/vx/include/fastMath.h
FieldMouse.o: ../include/Ddp.hh ${ELDORA}/vx/c++/include/Pipe.hh
FieldMouse.o: ${ELDORA}/vx/c++/include/Ring.hh
