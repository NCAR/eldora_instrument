#
#	Makefile for Tape Control routines.
#	Author: R. Neitzel
#	Date: 19 Aug. 91
#
# GNU C 
CC68= gcc68k
CC= gcc
CCC= gcc
LD= ld68k
#
#
ELDORA= /ELDORA
#
OPT= -O2
#
#
DEFINES=
#
INCLUDES= -I${ELDORA}/rpc/include
#
VX_INCLUDES= -I/vx/h
#
VFLAGS= $(OPT) $(VX_INCLUDES) $(INCLUDES) -m68020
CFLAGS = $(OPT) $(INCLUDES)
CCFLAGS=${CFLAGS}
#
# This include cuts around the GNU make use of CXX.
include /local/include/make/sun.cc.mk
#
LIBS=
#
UOBJS= tapeCtrlClnt.o tapeCtrlXdr.o
VXOBJS= tapeCtrlSvr.o tapeCtrlXdr68.o
#
SRCS= tapeCtrlSvr.c tapeCtrlXdr.c tapeCtrlClnt.c
#
HEADERS= tapeControl.h tapeCommands.h tapeStatus.h
#
HDRDIR= ${ELDORA}/rpc/include
#
LIBDIR= ${ELDORA}/rpc/lib
#
.KEEP_STATE:

all: libTapeRpc.a libVxTapeRpc.a record
	-for i in $(HEADERS); \
		do (diff $$i $(HDRDIR)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i"; cp $$i $(HDRDIR)) \
			fi;); \
		rm tmpdiff; \
	done;


libTapeRpc.a: $(UOBJS)
	ar crv $@ $(UOBJS)
	cp $@ $(LIBDIR)

	

libVxTapeRpc.a: $(VXOBJS)
	ar68k crv $@ ${VXOBJS}
	cp $@ $(LIBDIR)
	ranlib68k ${LIBDIR}/$@

record: record.o
	${CCC} -o $@ $@.o -L. -lTapeRpc -lnsl -lrpcsvc -lg++

tst:
	${CC68} ${VFLAGS} ${VX_INCLUDES} -DDEBUG_ONLY -o tst.o -c tapeCtrlSvr.c
	ld68k -Ur -X -o tapetst.o tst.o -L. libVxTapeRpc.a

tapeCtrlSvr.o:
	$(CC68) $(VFLAGS) $(VX_INCLUDES) -c tapeCtrlSvr.c

tapeCtrlHand.o:
	$(CC68) $(VFLAGS) $(VX_INCLUDES) -c tapeCtrlHand.c

tapeCtrlXdr68.o:
	$(CC68) $(VFLAGS) $(VX_INCLUDES) -c tapeCtrlXdr.c -o $@

tapeCtrlInit.o:
	$(CC68) $(VFLAGS) $(VXINCLUDES) -c tapeCtrlInit.c

asm:
	for i in $(SRCS); \
		do (echo "$(CC) -S $(CFLAGS) $$i";$(CC) -S $(CFLAGS) $$i); \
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
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h

depend:
	makedepend ${INCLUDES} ${SRCS}# DO NOT DELETE

tapeCtrlSvr.o: tapeControl.h /usr/include/rpc/rpc.h /usr/include/rpc/types.h
tapeCtrlSvr.o: /usr/include/sys/types.h /usr/include/sys/feature_tests.h
tapeCtrlSvr.o: /usr/include/sys/machtypes.h /usr/include/sys/select.h
tapeCtrlSvr.o: /usr/include/sys/time.h /usr/include/sys/time.h
tapeCtrlSvr.o: /usr/include/tiuser.h /usr/include/sys/tiuser.h
tapeCtrlSvr.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
tapeCtrlSvr.o: /usr/include/memory.h /usr/include/rpc/xdr.h
tapeCtrlSvr.o: /usr/include/sys/byteorder.h /usr/include/stdio.h
tapeCtrlSvr.o: /usr/include/rpc/auth.h /usr/include/sys/cred.h
tapeCtrlSvr.o: /usr/include/sys/t_lock.h /usr/include/sys/machlock.h
tapeCtrlSvr.o: /usr/include/sys/dki_lkinfo.h /usr/include/sys/dl.h
tapeCtrlSvr.o: /usr/include/sys/sleepq.h /usr/include/sys/turnstile.h
tapeCtrlSvr.o: /usr/include/sys/param.h /usr/include/sys/unistd.h
tapeCtrlSvr.o: /usr/include/sys/pirec.h /usr/include/sys/mutex.h
tapeCtrlSvr.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_com.h
tapeCtrlSvr.o: /usr/include/sys/netconfig.h /usr/include/rpc/rpc_msg.h
tapeCtrlSvr.o: /usr/include/rpc/auth_sys.h /usr/include/rpc/auth_des.h
tapeCtrlSvr.o: /usr/include/rpc/auth_kerb.h /usr/include/kerberos/krb.h
tapeCtrlSvr.o: /usr/include/kerberos/mit-copyright.h
tapeCtrlSvr.o: /usr/include/kerberos/des.h /usr/include/sys/socket.h
tapeCtrlSvr.o: /usr/include/netinet/in.h /usr/include/sys/stream.h
tapeCtrlSvr.o: /usr/include/sys/vnode.h /usr/include/sys/uio.h
tapeCtrlSvr.o: /usr/include/vm/seg_enum.h /usr/include/sys/poll.h
tapeCtrlSvr.o: /usr/include/sys/strmdep.h /usr/include/rpc/svc.h
tapeCtrlSvr.o: /usr/include/rpc/svc_auth.h /usr/include/rpc/rpcb_clnt.h
tapeCtrlSvr.o: /usr/include/rpc/rpcb_prot.h tapeCommands.h
tapeCtrlSvr.o: /ELDORA/rpc/include/GblCommands.h tapeStatus.h
tapeCtrlSvr.o: /ELDORA/rpc/include/GblStatus.h
tapeCtrlXdr.o: tapeControl.h /usr/include/rpc/rpc.h /usr/include/rpc/types.h
tapeCtrlXdr.o: /usr/include/sys/types.h /usr/include/sys/feature_tests.h
tapeCtrlXdr.o: /usr/include/sys/machtypes.h /usr/include/sys/select.h
tapeCtrlXdr.o: /usr/include/sys/time.h /usr/include/sys/time.h
tapeCtrlXdr.o: /usr/include/tiuser.h /usr/include/sys/tiuser.h
tapeCtrlXdr.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
tapeCtrlXdr.o: /usr/include/memory.h /usr/include/rpc/xdr.h
tapeCtrlXdr.o: /usr/include/sys/byteorder.h /usr/include/stdio.h
tapeCtrlXdr.o: /usr/include/rpc/auth.h /usr/include/sys/cred.h
tapeCtrlXdr.o: /usr/include/sys/t_lock.h /usr/include/sys/machlock.h
tapeCtrlXdr.o: /usr/include/sys/dki_lkinfo.h /usr/include/sys/dl.h
tapeCtrlXdr.o: /usr/include/sys/sleepq.h /usr/include/sys/turnstile.h
tapeCtrlXdr.o: /usr/include/sys/param.h /usr/include/sys/unistd.h
tapeCtrlXdr.o: /usr/include/sys/pirec.h /usr/include/sys/mutex.h
tapeCtrlXdr.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_com.h
tapeCtrlXdr.o: /usr/include/sys/netconfig.h /usr/include/rpc/rpc_msg.h
tapeCtrlXdr.o: /usr/include/rpc/auth_sys.h /usr/include/rpc/auth_des.h
tapeCtrlXdr.o: /usr/include/rpc/auth_kerb.h /usr/include/kerberos/krb.h
tapeCtrlXdr.o: /usr/include/kerberos/mit-copyright.h
tapeCtrlXdr.o: /usr/include/kerberos/des.h /usr/include/sys/socket.h
tapeCtrlXdr.o: /usr/include/netinet/in.h /usr/include/sys/stream.h
tapeCtrlXdr.o: /usr/include/sys/vnode.h /usr/include/sys/uio.h
tapeCtrlXdr.o: /usr/include/vm/seg_enum.h /usr/include/sys/poll.h
tapeCtrlXdr.o: /usr/include/sys/strmdep.h /usr/include/rpc/svc.h
tapeCtrlXdr.o: /usr/include/rpc/svc_auth.h /usr/include/rpc/rpcb_clnt.h
tapeCtrlXdr.o: /usr/include/rpc/rpcb_prot.h tapeCommands.h
tapeCtrlXdr.o: /ELDORA/rpc/include/GblCommands.h tapeStatus.h
tapeCtrlXdr.o: /ELDORA/rpc/include/GblStatus.h
tapeCtrlClnt.o: tapeControl.h /usr/include/rpc/rpc.h /usr/include/rpc/types.h
tapeCtrlClnt.o: /usr/include/sys/types.h /usr/include/sys/feature_tests.h
tapeCtrlClnt.o: /usr/include/sys/machtypes.h /usr/include/sys/select.h
tapeCtrlClnt.o: /usr/include/sys/time.h /usr/include/sys/time.h
tapeCtrlClnt.o: /usr/include/tiuser.h /usr/include/sys/tiuser.h
tapeCtrlClnt.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
tapeCtrlClnt.o: /usr/include/memory.h /usr/include/rpc/xdr.h
tapeCtrlClnt.o: /usr/include/sys/byteorder.h /usr/include/stdio.h
tapeCtrlClnt.o: /usr/include/rpc/auth.h /usr/include/sys/cred.h
tapeCtrlClnt.o: /usr/include/sys/t_lock.h /usr/include/sys/machlock.h
tapeCtrlClnt.o: /usr/include/sys/dki_lkinfo.h /usr/include/sys/dl.h
tapeCtrlClnt.o: /usr/include/sys/sleepq.h /usr/include/sys/turnstile.h
tapeCtrlClnt.o: /usr/include/sys/param.h /usr/include/sys/unistd.h
tapeCtrlClnt.o: /usr/include/sys/pirec.h /usr/include/sys/mutex.h
tapeCtrlClnt.o: /usr/include/rpc/clnt.h /usr/include/rpc/rpc_com.h
tapeCtrlClnt.o: /usr/include/sys/netconfig.h /usr/include/rpc/rpc_msg.h
tapeCtrlClnt.o: /usr/include/rpc/auth_sys.h /usr/include/rpc/auth_des.h
tapeCtrlClnt.o: /usr/include/rpc/auth_kerb.h /usr/include/kerberos/krb.h
tapeCtrlClnt.o: /usr/include/kerberos/mit-copyright.h
tapeCtrlClnt.o: /usr/include/kerberos/des.h /usr/include/sys/socket.h
tapeCtrlClnt.o: /usr/include/netinet/in.h /usr/include/sys/stream.h
tapeCtrlClnt.o: /usr/include/sys/vnode.h /usr/include/sys/uio.h
tapeCtrlClnt.o: /usr/include/vm/seg_enum.h /usr/include/sys/poll.h
tapeCtrlClnt.o: /usr/include/sys/strmdep.h /usr/include/rpc/svc.h
tapeCtrlClnt.o: /usr/include/rpc/svc_auth.h /usr/include/rpc/rpcb_clnt.h
tapeCtrlClnt.o: /usr/include/rpc/rpcb_prot.h tapeCommands.h
tapeCtrlClnt.o: /ELDORA/rpc/include/GblCommands.h tapeStatus.h
tapeCtrlClnt.o: /ELDORA/rpc/include/GblStatus.h
