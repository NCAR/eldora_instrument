#
#	Makefile for 
#	Author:
#	Date:
#
CCC= gcc
#
#
ELDORA=/ELDORA
#
OPT= -O2
#
UDEFINES= -DUNIX -DCLIENT_SIDE
VDEFINES=
#
UINCLUDES= -I../include -I/ELDORA/include
VINCLUDES= -I../include -I/ELDORA/include -I/vx/h -I/ELDORA/vx/hskp/include\
		-I../header -I/local/include/vx++
#
CCFLAGS= $(OPT) $(UDEFINES) $(UINCLUDES)
VFLAGS= $(OPT) $(VDEFINES) $(VINCLUDES)
#
LIBS=
#
UOBJS= HskpClient.o xdr_Hskp.o
VOBJS= HskpSvr.o xdr_Hskp68k.o
#
USRCS= HskpClient.cc xdr_Hskp.cc
VSRCS= HskpSvr.cc 
SRCS= $(USRCS) $(VSRCS)
#
HEADERS= Hskp.hh HskpCmd.h HskpStatus.h
#
INC= ../include
#
BIN= ../lib
#
# This include cuts around the GNU make use of CXX.
include /local/include/make/sun.cc.mk

.KEEP_STATE:
.MAIN: all

all: libHskpRpc.a libVxHskpRpc.a
	-for i in $(HEADERS); \
		do (echo "diffing $$i"; diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;
	cp libHskpRpc.a $(BIN)
	cp libVxHskpRpc.a $(BIN)
	ranlib68k ${BIN}/libVxHskpRpc.a

libHskpRpc.a: $(UOBJS)
	ar crv $@ $(UOBJS)

${VOBJS}: ${VSRCS}

libVxHskpRpc.a: $(VOBJS)
	ar68k crv $@ $(VOBJS)

hskp: hskp.o
	${CCC} -o $@ $@.o -L. -lHskpRpc -lnsl -lrpcsvc -lg++

tst:
	gcc68k ${VFLAGS} -DDEBUG_ONLY -c -o tst.o HskpSvr.cc
	ld68k -Ur -X -o hskptst.o tst.o -L. -lVxHskpRpc \
	-L/local/lib/gcc-lib/68k/current -lg++ -lgcc

HskpSvr.o:
	gcc68k $(VFLAGS) -c HskpSvr.cc

xdr_Hskp68k.o:
	gcc68k $(VFLAGS) -c -o $@ xdr_Hskp.cc


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
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L -acraig -ashawn\
	$$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L -acraig -ashawn\
	$$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h

depend:
	makedepend $(UINCLUDES) $(UDEFINES) $(USRCS) 
# DO NOT DELETE THIS LINE -- make depend depends on it.

HskpClient.o: Hskp.hh /usr/include/rpc/rpc.h /usr/include/rpc/types.h
HskpClient.o: /usr/include/sys/types.h /usr/include/sys/feature_tests.h
HskpClient.o: /usr/include/sys/machtypes.h /usr/include/sys/time.h
HskpClient.o: /usr/include/tiuser.h /usr/include/sys/tiuser.h
HskpClient.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
HskpClient.o: /usr/include/memory.h /usr/include/rpc/xdr.h
HskpClient.o: /usr/include/sys/byteorder.h /usr/include/stdio.h
HskpClient.o: /usr/include/rpc/auth.h /usr/include/sys/cred.h
HskpClient.o: /usr/include/sys/t_lock.h /usr/include/sys/machlock.h
HskpClient.o: /usr/include/sys/dki_lkinfo.h /usr/include/sys/dl.h
HskpClient.o: /usr/include/sys/sleepq.h /usr/include/sys/turnstile.h
HskpClient.o: /usr/include/sys/param.h /usr/include/sys/unistd.h
HskpClient.o: /usr/include/sys/pirec.h /usr/include/sys/mutex.h
HskpClient.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_com.h
HskpClient.o: /usr/include/sys/netconfig.h /usr/include/rpc/rpc_msg.h
HskpClient.o: /usr/include/rpc/auth_sys.h /usr/include/rpc/auth_des.h
HskpClient.o: /usr/include/rpc/auth_kerb.h /usr/include/kerberos/krb.h
HskpClient.o: /usr/include/kerberos/mit-copyright.h
HskpClient.o: /usr/include/kerberos/des.h /usr/include/sys/socket.h
HskpClient.o: /usr/include/netinet/in.h /usr/include/sys/stream.h
HskpClient.o: /usr/include/sys/vnode.h /usr/include/sys/uio.h
HskpClient.o: /usr/include/vm/seg_enum.h /usr/include/sys/poll.h
HskpClient.o: /usr/include/sys/strmdep.h /usr/include/rpc/svc.h
HskpClient.o: /usr/include/rpc/svc_auth.h /usr/include/rpc/rpcb_clnt.h
HskpClient.o: /usr/include/rpc/rpcb_prot.h HskpCmd.h ../include/GblCommands.h
HskpClient.o: HskpStatus.h
xdr_Hskp.o: Hskp.hh /usr/include/rpc/rpc.h /usr/include/rpc/types.h
xdr_Hskp.o: /usr/include/sys/types.h /usr/include/sys/feature_tests.h
xdr_Hskp.o: /usr/include/sys/machtypes.h /usr/include/sys/time.h
xdr_Hskp.o: /usr/include/tiuser.h /usr/include/sys/tiuser.h
xdr_Hskp.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
xdr_Hskp.o: /usr/include/memory.h /usr/include/rpc/xdr.h
xdr_Hskp.o: /usr/include/sys/byteorder.h /usr/include/stdio.h
xdr_Hskp.o: /usr/include/rpc/auth.h /usr/include/sys/cred.h
xdr_Hskp.o: /usr/include/sys/t_lock.h /usr/include/sys/machlock.h
xdr_Hskp.o: /usr/include/sys/dki_lkinfo.h /usr/include/sys/dl.h
xdr_Hskp.o: /usr/include/sys/sleepq.h /usr/include/sys/turnstile.h
xdr_Hskp.o: /usr/include/sys/param.h /usr/include/sys/unistd.h
xdr_Hskp.o: /usr/include/sys/pirec.h /usr/include/sys/mutex.h
xdr_Hskp.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_com.h
xdr_Hskp.o: /usr/include/sys/netconfig.h /usr/include/rpc/rpc_msg.h
xdr_Hskp.o: /usr/include/rpc/auth_sys.h /usr/include/rpc/auth_des.h
xdr_Hskp.o: /usr/include/rpc/auth_kerb.h /usr/include/kerberos/krb.h
xdr_Hskp.o: /usr/include/kerberos/mit-copyright.h /usr/include/kerberos/des.h
xdr_Hskp.o: /usr/include/sys/socket.h /usr/include/netinet/in.h
xdr_Hskp.o: /usr/include/sys/stream.h /usr/include/sys/vnode.h
xdr_Hskp.o: /usr/include/sys/uio.h /usr/include/vm/seg_enum.h
xdr_Hskp.o: /usr/include/sys/poll.h /usr/include/sys/strmdep.h
xdr_Hskp.o: /usr/include/rpc/svc.h /usr/include/rpc/svc_auth.h
xdr_Hskp.o: /usr/include/rpc/rpcb_clnt.h /usr/include/rpc/rpcb_prot.h
xdr_Hskp.o: HskpCmd.h ../include/GblCommands.h HskpStatus.h
