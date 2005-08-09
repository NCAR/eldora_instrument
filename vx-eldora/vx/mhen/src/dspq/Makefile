#
#	Makefile for Directory dspq 
#	Author: Eric Loew
#	Date: 12/3/91
#
# GNU C 
CC= gcc68k -fno-builtin -nostdinc
#
DEFINES= -DTADPL -DNU_ARCHIT -DELDORA
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I/ELDORA/vx/mhen/include -I/ELDORA/dsp/include \
	-I/ELDORA/vx/mhen/src/nu_archit
#
CFLAGS= $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS=dspgo.o dspstop.o dsy.o gates4.o sam4.o gates8.o probe.o \
     nu_lddsp.o opflags.o v_scale.o dp_sync.o \
     ppp_opflags.o ngates.o ppp_sync.o dp_intr.o ppp_intr.o \
     ppp_ts_gate.o
#
NU_ARCH_OBJS=dspgo.o dspstop.o dsy.o gates4.o sam4.o gates8.o probe.o \
     nu_lddsp.o opflags.o v_scale.o dp_sync.o \
     ppp_opflags.o ngates.o ppp_sync.o dp_intr.o ppp_intr.o ppp_ts_gate.o
#
LAB_OBJS=dspgo.o dspstop.o dsy.o sam4.o gates4.o gates8.o probe.o \
     nu_lddsp.o opflags.o v_scale.o dp_sync.o \
     ppp_opflags.o ngates.o ppp_sync.o dp_intr.o ppp_intr.o
#
SRCS=dspgo.c dspstop.c dsy.c gates4.c sam4.c gates8.c probe.c \
     nu_lddsp.c opflags.c v_scale.c dp_sync.c \
     ppp_opflags.c ngates.c ppp_sync.c dp_intr.c ppp_intr.c \
     ppp_ts_gate.c
#
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN=/ELDORA/vx/mhen/bin

.KEEP_STATE:

compile: $(OBJS)

dspq:
	ld68k -X -r -o dspq.o $(NU_ARCH_OBJS)
	install -m 774 dspq.o $(BIN)

lab_dspq: $(LAB_OBJS)
	ld68k -X -r -o lab_dspq.o $(LAB_OBJS)
	install -m 774 lab_dspq.o $(BIN)

depend:	
	makedepend ${INCLUDES} ${SRCS}

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;

asm:
	for i in $(SRCS); \
		do (echo "$(CC) -S $(CFLAGS) $$i";$(CC) -S $(CFLAGS) $$i); \
	done;

get_rcs:
	for i in $(SRCS); \
		do (echo "co $$i";co $$i); \
	done;

get_rcs_locked:
	for i in $(SRCS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;

# Modify this to include an access list if desired.
set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,shawn,rich,eric,reif -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






# DO NOT DELETE THIS LINE -- make depend depends on it.

dspgo.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/types/vxANSI.h
dspgo.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
dspgo.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
dspgo.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
dspgo.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
dspgo.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
dspgo.o: /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
dspstop.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/types/vxANSI.h
dspstop.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
dspstop.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
dspstop.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
dspstop.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
dspstop.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
dspstop.o: /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
dsy.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
dsy.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
dsy.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
dsy.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
dsy.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
dsy.o: /vx/h/private/classLibP.h /vx/h/stdarg.h
dsy.o: /ELDORA/vx/mhen/include/ELDRP7.h
gates4.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
gates4.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
gates4.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
gates4.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
gates4.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
gates4.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
gates4.o: /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
sam4.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
sam4.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
sam4.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
sam4.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
sam4.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
sam4.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
sam4.o: /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
gates8.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
gates8.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
gates8.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
gates8.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
gates8.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
gates8.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
gates8.o: /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
probe.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
probe.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
probe.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
probe.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
probe.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
probe.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
probe.o: /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
nu_lddsp.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
nu_lddsp.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
nu_lddsp.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
nu_lddsp.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
nu_lddsp.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
nu_lddsp.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
nu_lddsp.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/nfsLib.h
nu_lddsp.o: /vx/h/dirent.h /vx/h/limits.h /vx/h/sys/stat.h
nu_lddsp.o: /vx/h/xdr_nfsserv.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
nu_lddsp.o: /vx/h/xdr_nfs.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/net/uio.h
nu_lddsp.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
nu_lddsp.o: /vx/h/strLib.h /vx/h/string.h /vx/h/ctype.h
nu_lddsp.o: /ELDORA/vx/mhen/include/ELDRP7.h
opflags.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
opflags.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
opflags.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
opflags.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
opflags.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
opflags.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
opflags.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
opflags.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
opflags.o: /ELDORA/dsp/include/dspaddr.h
v_scale.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
v_scale.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
v_scale.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
v_scale.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
v_scale.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
v_scale.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
v_scale.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
v_scale.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
v_scale.o: /ELDORA/dsp/include/dspaddr.h
dp_sync.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
dp_sync.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
dp_sync.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
dp_sync.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
dp_sync.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
dp_sync.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
dp_sync.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
dp_sync.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
dp_sync.o: /ELDORA/dsp/include/dspaddr.h
ppp_opflags.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
ppp_opflags.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
ppp_opflags.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
ppp_opflags.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
ppp_opflags.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
ppp_opflags.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
ppp_opflags.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
ppp_opflags.o: /ELDORA/vx/mhen/include/ELDRP7.h
ngates.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
ngates.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
ngates.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
ngates.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
ngates.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
ngates.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
ngates.o: /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
ppp_sync.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
ppp_sync.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
ppp_sync.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
ppp_sync.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
ppp_sync.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
ppp_sync.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
ppp_sync.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
ppp_sync.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
ppp_sync.o: /ELDORA/dsp/include/dspaddr.h
dp_intr.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
dp_intr.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
dp_intr.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
dp_intr.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
dp_intr.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
dp_intr.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
dp_intr.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
dp_intr.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
dp_intr.o: /ELDORA/dsp/include/dspaddr.h
ppp_intr.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
ppp_intr.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
ppp_intr.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
ppp_intr.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
ppp_intr.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
ppp_intr.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
ppp_intr.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
ppp_intr.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
ppp_intr.o: /ELDORA/dsp/include/dspaddr.h
ppp_ts_gate.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
ppp_ts_gate.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
ppp_ts_gate.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
ppp_ts_gate.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
ppp_ts_gate.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
ppp_ts_gate.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
ppp_ts_gate.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
ppp_ts_gate.o: /ELDORA/vx/mhen/include/ELDRP7.h
