#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CXX= gcc68k
#
# This where the ELDORA tree is rooted.
ELDORA= /ELDORA
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h -I/local/include/vx++ -I../include -I/ELDORA/vx/include \
	-I${ELDORA}/rpc/include -I/ELDORA/include
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= Display.o
#
SRCS= Display.cc
#
HEADERS= Display.hh
#
INC= ../include
#
BIN= ../bin
#

all: $(OBJS) update

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;
	cp ${OBJS} ${BIN}

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

depend:
	makedepend $(INCLUDES) $(SRCS)




# DO NOT DELETE THIS LINE -- make depend depends on it.

Display.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
Display.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
Display.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
Display.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdio.h
Display.o: /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
Display.o: /vx/h/memLib.h /vx/h/private/objLibP.h /vx/h/string.h
Display.o: /vx/h/stdlib.h /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h
Display.o: /vx/h/regs.h /vx/h/excLib.h /vx/h/private/semLibP.h /vx/h/semLib.h
Display.o: /vx/h/private/classLibP.h Display.hh ../include/Window.hh
Display.o: ../include/GraphicController.hh ../include/point.h
Display.o: ../include/qpdm.h ../include/WdwGbls.h
Display.o: ../include/GraphicControllerP.hh ../include/DataPoint.h
Display.o: /vx/h/math.h /local/include/vx++/iostream.h
Display.o: /local/include/vx++/streambuf.h /local/include/vx++/libio.h
Display.o: ${ELDORA}/rpc/include/DisplayRpc.h /vx/h/rpc/rpc.h
Display.o: /vx/h/rpc/rpctypes.h /vx/h/sys/times.h /vx/h/netinet/in.h
Display.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
Display.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
Display.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
Display.o: ${ELDORA}/rpc/include/DispCommands.h
Display.o: ${ELDORA}/rpc/include/GblCommands.h ${ELDORA}/rpc/include/DispStatus.h
Display.o: ${ELDORA}/rpc/include/GblStatus.h ${ELDORA}/rpc/include/DispAlarm.h
Display.o: /vx/h/rpc/pmap_clnt.h ${ELDORA}/rpc/include/Header.hh
Display.o: ${ELDORA}/rpc/include/Header.h ${ELDORA}/rpc/include/TapeHeader.h
Display.o: ${ELDORA}/rpc/include/CellSpacing.h ${ELDORA}/rpc/include/FieldRadar.h
Display.o: ${ELDORA}/rpc/include/Parameter.h ${ELDORA}/rpc/include/RadarDesc.h
Display.o: ${ELDORA}/rpc/include/Volume.h ${ELDORA}/rpc/include/Waveform.h
Display.o: ${ELDORA}/rpc/include/InSitu.h ${ELDORA}/rpc/include/NavDesc.h
Display.o: ../include/ParamNames.hh ../include/ColorConverter.hh
Display.o: /vx/h/stdioLib.h ../include/DataBeam.hh ${ELDORA}/rpc/include/Ray.h
Display.o: ${ELDORA}/rpc/include/Platform.h ${ELDORA}/rpc/include/FieldParam.h
Display.o: /ELDORA/include/mathVar.h /ELDORA/vx/include/fastMath.h
