#
#	Makefile for Directory eth29fc
#	Author: Eric Loew
#	Date: 12/3/91
#
# GNU C 
CC= gcc68k -fno-builtin -nostdinc -w
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions \
	-fomit-frame-pointer
OPT= -O2
#OPT=-O1
#
DEFINES= -DUSE_ANSI -DTADPL -DNEW_DSP_CODE -DVXWORKS
#
#
# Drag in includes from here.
INCLUDES= -I/. -I/vx/h -I/local/include/vx++
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
CXX = ${CCC}
CXXFLAGS = ${CCFLAGS}
#
LIBS=
#
HEADERS=
#
OBJS=eth29.o
#
#
# Include files go here.
INC=
#
SRCS=eth29.c
#
# Binaries go here.
BIN=.

compile: $(OBJS)

install: $(OBJS)	
#	scp eth29.o martinc@dyn80-25:/magnus/dt/eldora/vx/eth29fc
	cp eth29.o  ../../bin

depend:	
	makedepend ${INCLUDES} ${SRCS}

clean:
	rm -f *.o *~ core

# DO NOT DELETE

isockdrv.o: nif.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
isockdrv.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
isockdrv.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
isockdrv.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h neterrno.h sockcons.h
isockdrv.o: hwconfig.h /vx/h/errno.h mbuf.h customize.h param.h machparm.h
isockdrv.o: endian.h
sockstat.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
sockstat.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
sockstat.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
sockstat.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h mbuf.h
nif_c.o: nif.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
nif_c.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
nif_c.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
nif_c.o: /vx/h/sys/types.h mbuf.h hwconfig.h customize.h
nifmisc.o: hwconfig.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
nifmisc.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
nifmisc.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
nifmisc.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
nifmisc.o: mbuf.h nif.h neterr.h customize.h
nifsock.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
nifsock.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
nifsock.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
nifsock.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/errno.h mbuf.h
nifsock.o: socket.h neterrno.h nif.h hwconfig.h customize.h
misc.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
misc.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
misc.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
misc.o: /vx/h/sys/types.h hwconfig.h ms360.h customize.h
customize.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
customize.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
customize.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
customize.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/iv.h
customize.o: /vx/h/semLib.h /vx/h/vwModNum.h customize.h
testNat.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
testNat.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
testNat.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
testNat.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h customize.h
testNat.o: /vx/h/sys/socket.h /vx/h/netinet/in.h
setdebug.o: /vx/h/stdio.h /vx/h/types/vxANSI.h /vx/h/types/vxCpu.h
setdebug.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
setdebug.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
setdebug.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
setdebug.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
setdebug.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
setdebug.o: endian.h /vx/h/sys/socket.h /vx/h/sys/ioctl.h /vx/h/net/if.h
setdebug.o: /vx/h/net/if_arp.h /vx/h/ifLib.h /vx/h/netinet/tcp.h
setdebug.o: /vx/h/netinet/in.h /usr/include/netdb.h
setdebug.o: /usr/include/sys/feature_tests.h /usr/include/sys/isa_defs.h
