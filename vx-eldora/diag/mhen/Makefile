#
#	Makefile for Directory diag/mhen 
#	Author: Eric Loew
#	Date: 4/28/95
#
# GNU C 

CCC= gcc68k -fno-builtin -nostdinc
CC= gcc68k -fno-builtin -nostdinc

OPT= -O2
#
VXDEFINES= -DVX -DELDORA -DTADPL
#
# Drag in includes from here.
INCLUDES= -I/vw/h -I/ELDORA/vx/mhen/include -I/ELDORA/dsp/include \
	-I/ELDORA/rpc/include -I/ELDORA/vx/mhen/src/nu_archit \
	-I/ELDORA/diag/include -I/net/rsg/radar/rel/include \
	-I/ELDORA/vx/hskp/include -I/local/include/vx++
#
CFLAGS= ${OPT} ${VXDEFINES} ${INCLUDES}
#CFLAGS= ${VXDEFINES} ${INCLUDES}
CCFLAGS= ${CFLAGS}
include /local/include/make/sun.cc.mk
#
LIBS=
#
OBJS=DSP32C.o QuadDSP32C.o CollDSP32C.o ELDORA_RP7.o eldrp7_tst.o \
	ran2.o quad_tst.o coll_tst.o ascope.o fft.o getseres.o \
	bus_test.o
#
SRCS=DSP32C.cc QuadDSP32C.cc CollDSP32C.cc ELDORA_RP7.cc eldrp7_tst.cc \
ran2.c quad_tst.c coll_tst.c ascope.c fft.c getseres.c bus_test.c
#
ELD_OBJS=DSP32C.o QuadDSP32C.o CollDSP32C.o ELDORA_RP7.o eldrp7_tst.o \
	ran2.o quad_tst.o coll_tst.o bus_test.o \
	/ELDORA/vx/mhen/src/timmod/latch_daisy.o \
	/ELDORA/vx/mhen/src/timmod/timonb.o \
	/ELDORA/vx/mhen/src/timmod/timoffb.o \
	/ELDORA/vx/mhen/src/ad/prog_digif.o
#
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN=/ELDORA/vx/mhen/bin

.KEEP_STATE:

all: ${OBJS} 

diag:
	ld68k -X -r -o diag.o $(ELD_OBJS)
	install -m 774 diag.o $(BIN)

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

dspgo.o: /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/types/vxANSI.h
dspgo.o: /vw/h/types/vxCpu.h /vw/h/types/vxArch.h /vw/h/types/vxParams.h
dspgo.o: /vw/h/types/vxTypesBase.h /vw/h/classLib.h /vw/h/vxWorks.h
dspgo.o: /vw/h/types/vxTypes.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
dspgo.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
dspgo.o: /vw/h/private/objLibP.h /vw/h/ctype.h /vw/h/stdarg.h
dspgo.o: /ELDORA/vx/mhen/include/ELDRP7.h
dspstop.o: /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/types/vxANSI.h
dspstop.o: /vw/h/types/vxCpu.h /vw/h/types/vxArch.h /vw/h/types/vxParams.h
dspstop.o: /vw/h/types/vxTypesBase.h /vw/h/classLib.h /vw/h/vxWorks.h
dspstop.o: /vw/h/types/vxTypes.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
dspstop.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
dspstop.o: /vw/h/private/objLibP.h /vw/h/ctype.h /vw/h/stdarg.h
dspstop.o: /ELDORA/vx/mhen/include/ELDRP7.h
dsy.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
dsy.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
dsy.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
dsy.o: /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h
dsy.o: /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h
dsy.o: /ELDORA/vx/mhen/include/ELDRP7.h
gates4.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
gates4.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
gates4.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
gates4.o: /vw/h/sys/types.h /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h
gates4.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
gates4.o: /vw/h/private/objLibP.h /vw/h/ctype.h /vw/h/stdarg.h
gates4.o: /ELDORA/vx/mhen/include/ELDRP7.h
sam4.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
sam4.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
sam4.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
sam4.o: /vw/h/sys/types.h /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h
sam4.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
sam4.o: /vw/h/private/objLibP.h /vw/h/stdarg.h /vw/h/ctype.h
sam4.o: /ELDORA/vx/mhen/include/ELDRP7.h
gates8.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
gates8.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
gates8.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
gates8.o: /vw/h/sys/types.h /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h
gates8.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
gates8.o: /vw/h/private/objLibP.h /vw/h/ctype.h /vw/h/stdarg.h
gates8.o: /ELDORA/vx/mhen/include/ELDRP7.h
probe.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
probe.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
probe.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
probe.o: /vw/h/sys/types.h /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h
probe.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
probe.o: /vw/h/private/objLibP.h /vw/h/stdarg.h /vw/h/ctype.h
probe.o: /ELDORA/vx/mhen/include/ELDRP7.h
nu_lddsp.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
nu_lddsp.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
nu_lddsp.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
nu_lddsp.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
nu_lddsp.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
nu_lddsp.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h
nu_lddsp.o: /vw/h/nfsLib.h /vw/h/dirent.h /vw/h/limits.h /vw/h/xdr_nfs.h
nu_lddsp.o: /vw/h/rpc/xdr.h /vw/h/rpc/rpctypes.h /vw/h/sys/times.h
nu_lddsp.o: /vw/h/sys/stat.h /vw/h/stdarg.h /vw/h/iosLib.h /vw/h/ioLib.h
nu_lddsp.o: /vw/h/net/uio.h /vw/h/fcntl.h /vw/h/sys/fcntlcom.h /vw/h/unistd.h
nu_lddsp.o: /vw/h/dllLib.h /vw/h/strLib.h /vw/h/string.h /vw/h/ctype.h
nu_lddsp.o: /ELDORA/vx/mhen/include/ELDRP7.h
opflags.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
opflags.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
opflags.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
opflags.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
opflags.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
opflags.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h /vw/h/ctype.h
opflags.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
opflags.o: /ELDORA/dsp/include/dspaddr.h
v_scale.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
v_scale.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
v_scale.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
v_scale.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
v_scale.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
v_scale.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h /vw/h/ctype.h
v_scale.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
v_scale.o: /ELDORA/dsp/include/dspaddr.h
pkgate.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
pkgate.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
pkgate.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
pkgate.o: /vw/h/sys/types.h /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h
pkgate.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
pkgate.o: /vw/h/private/objLibP.h /vw/h/ctype.h /vw/h/stdarg.h
pkgate.o: /ELDORA/vx/mhen/include/ELDRP7.h
dp_sync.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
dp_sync.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
dp_sync.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
dp_sync.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
dp_sync.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
dp_sync.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h /vw/h/ctype.h
dp_sync.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
dp_sync.o: /ELDORA/dsp/include/dspaddr.h
ppp_opflags.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
ppp_opflags.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
ppp_opflags.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
ppp_opflags.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
ppp_opflags.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
ppp_opflags.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h
ppp_opflags.o: /vw/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
ngates.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
ngates.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
ngates.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
ngates.o: /vw/h/sys/types.h /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h
ngates.o: /vw/h/vwModNum.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
ngates.o: /vw/h/private/objLibP.h /vw/h/ctype.h /vw/h/stdarg.h
ngates.o: /ELDORA/vx/mhen/include/ELDRP7.h
ppp_sync.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
ppp_sync.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
ppp_sync.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
ppp_sync.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
ppp_sync.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
ppp_sync.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h
ppp_sync.o: /vw/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
ppp_sync.o: /ELDORA/dsp/include/dp_ramdefs.h /ELDORA/dsp/include/dspaddr.h
dp_intr.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
dp_intr.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
dp_intr.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
dp_intr.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
dp_intr.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
dp_intr.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h /vw/h/ctype.h
dp_intr.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/dsp/include/dp_ramdefs.h
dp_intr.o: /ELDORA/dsp/include/dspaddr.h
ppp_intr.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
ppp_intr.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
ppp_intr.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
ppp_intr.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/stdioLib.h
ppp_intr.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/vwModNum.h /vw/h/objLib.h
ppp_intr.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h
ppp_intr.o: /vw/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
ppp_intr.o: /ELDORA/dsp/include/dp_ramdefs.h /ELDORA/dsp/include/dspaddr.h
