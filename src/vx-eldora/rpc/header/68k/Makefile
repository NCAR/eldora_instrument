#
#       Makefile for VxWorks Header code.
#       Author: Rich Neitzel
#       Date:   Tue Aug 30 1994
#
# GNU C++ 
CCC= gcc68k
CC= ${CCC}
CXX= ${CCC}
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
# This where the ELDORA tree is rooted.
ELDORA= /ELDORA
#
OPT= -O2
#
# NOTE ------ We define the default as a 68020, since we still have a
# mv133 kicking around.
DEFINES= -DOK_RPC -Dmc68020
#
INCLUDES= -I/vx/h -I/local/include/vx++ -I..
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CFLAGS= ${CCFLAGS}
CXXFLAGS= ${CCFLAGS}
#
LIBS=
#
OBJS=Header.o
#
SRCS=
#
HEADERS=
#
INC= ${ELDORA}/rpc/include
#
BIN= ${ELDORA}/rpc/bin
#
LIB= ${ELDORA}/rpc/lib
#
LIBNAME= libVxHeader.a
#
.KEEP_STATE:

all: lib update

lib: ${OBJS}
	${AR} crv ${LIBNAME} ${OBJS}

update:
	cp ${LIBNAME} ${LIB}
	${RANLIB} ${LIB}/${LIBNAME}

HeaderRpc_xdr.o: ../HeaderRpc_xdr.cc
	$(CCC) $(CCFLAGS) -c ../HeaderRpc_xdr.cc -o $@

Header.o: ../Header.cc
	$(CCC) $(CCFLAGS) -c ../Header.cc -o $@
asm:
	for i in $(SRCS); \
		do (echo "$(CCC) -S $(CCFLAGS) $$i";$(CCC) -S $(CCFLAGS) $$i); \
	done;

depend:
	makedepend -DCPU=MC68020 ${DEFINES} ${INCLUDES} ${SRCS} \
	../Header.cc ../HeaderRpc_xdr.cc

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
# DO NOT DELETE

HeaderRpc_svc.o: ../HeaderRpc.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
HeaderRpc_svc.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
HeaderRpc_svc.o: /vx/h/arch/mc68k/archMc68k.h /vx/h/types/vxParams.h
HeaderRpc_svc.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
HeaderRpc_svc.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
HeaderRpc_svc.o: /vx/h/sys/types.h /vx/h/sys/times.h /vx/h/netinet/in.h
HeaderRpc_svc.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
HeaderRpc_svc.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
HeaderRpc_svc.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h /vx/h/string.h
HeaderRpc_svc.o: /vx/h/stdlib.h
HeaderRpc_server.o: ../HeaderRpc.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
HeaderRpc_server.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
HeaderRpc_server.o: /vx/h/arch/mc68k/archMc68k.h /vx/h/types/vxParams.h
HeaderRpc_server.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
HeaderRpc_server.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
HeaderRpc_server.o: /vx/h/sys/types.h /vx/h/sys/times.h /vx/h/netinet/in.h
HeaderRpc_server.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
HeaderRpc_server.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
HeaderRpc_server.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
HeaderRpc_server.o: /vx/h/rpc/svc_auth.h /local/include/vx++/fstream.h
HeaderRpc_server.o: /local/include/vx++/iostream.h
HeaderRpc_server.o: /local/include/vx++/streambuf.h
HeaderRpc_server.o: /local/include/vx++/libio.h
HeaderRpc_server.o: /local/include/vx++/_G_config.h
Header.o: ../Header.hh ../Header.h ../TapeHeader.h ../CellSpacing.h
Header.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/vxWorks.h
Header.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
Header.o: /vx/h/arch/mc68k/archMc68k.h /vx/h/types/vxParams.h
Header.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
Header.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
Header.o: /vx/h/sys/times.h /vx/h/netinet/in.h /vx/h/rpc/xdr.h
Header.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h /vx/h/rpc/clnt.h
Header.o: /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
Header.o: /vx/h/rpc/svc_auth.h ../FieldRadar.h ../Parameter.h
Header.o: ../RadarDesc.h ../Volume.h ../Waveform.h ../InSitu.h
Header.o: ../NavDesc.h /local/include/vx++/iostream.h
Header.o: /local/include/vx++/streambuf.h /local/include/vx++/libio.h
Header.o: /local/include/vx++/_G_config.h /vx/h/stdio.h /vx/h/classLib.h
Header.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
Header.o: /vx/h/private/objLibP.h /vx/h/stdarg.h /vx/h/stdlib.h
Header.o: /vx/h/string.h
HeaderRpc_xdr.o: ../HeaderRpc.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
HeaderRpc_xdr.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
HeaderRpc_xdr.o: /vx/h/arch/mc68k/archMc68k.h /vx/h/types/vxParams.h
HeaderRpc_xdr.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
HeaderRpc_xdr.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
HeaderRpc_xdr.o: /vx/h/sys/types.h /vx/h/sys/times.h /vx/h/netinet/in.h
HeaderRpc_xdr.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
HeaderRpc_xdr.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
HeaderRpc_xdr.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
HeaderRpc_xdr.o: /vx/h/rpc/svc_auth.h
