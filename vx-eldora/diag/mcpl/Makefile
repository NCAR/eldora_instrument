#
#	Makefile for Directory home/craig/mcpl/diag
#	Author: Craig Walther
#	Date: 8/27/92
#
# GNU C 
CC= gcc68k
LD= ld68k
CCC = $(CC)
INSTALL= /usr/ucb/install
#
OPT= -O2 -fvolatile
#
DEFINES=
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I. -I/home/thor/craig/mcpl/include
#
CFLAGS= $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/bin/vxheader.o
#
OBJS= init_pntrs.o mcpl_reset.o mcpl_init.o stand_xfer.o bcast_xfer.o \
	mcpl_menu.o print_tpbs.o ran2.o
#
SRCS= init_pntrs.c mcpl_reset.c mcpl_init.c stand_xfer.c bcast_xfer.c \
	mcpl_menu.c print_tpbs.c ran2.c
#
#    
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN= /ELDORA/diag/bin

.KEEP_STATE:

mcpl_diag.o: $(OBJS)
	${LD} -r -X -o $@ ${OBJS}
	 ${INSTALL} -m 664 $@ $(BIN)

asm:
	for i in $(SRCS); \
		do (echo "$(CC) -S $(CFLAGS) $$i";$(CC) -S $(CFLAGS) $$i); \
	done;

depend:
	makedepend ${INCLUDES} ${SRCS}

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
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,eric -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h


# DO NOT DELETE THIS LINE -- make depend depends on it.

init_pntrs.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
init_pntrs.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
init_pntrs.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
init_pntrs.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
init_pntrs.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
init_pntrs.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_pntrs.o: /vx/h/private/objLibP.h /vx/h/intLib.h /vx/h/semLib.h
init_pntrs.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
init_pntrs.o: /vx/h/excLib.h /vx/h/private/semLibP.h
init_pntrs.o: /vx/h/private/classLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
init_pntrs.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
init_pntrs.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
init_pntrs.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
init_pntrs.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
init_pntrs.o: /vx/h/sysLib.h /vx/h/bootLib.h
init_pntrs.o: /home/thor/craig/mcpl/include/mcplDef.h
init_pntrs.o: /home/thor/craig/mcpl/include/mcplGbl.h
mcpl_reset.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
mcpl_reset.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
mcpl_reset.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
mcpl_reset.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
mcpl_reset.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
mcpl_reset.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mcpl_reset.o: /vx/h/private/objLibP.h /vx/h/intLib.h /vx/h/semLib.h
mcpl_reset.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
mcpl_reset.o: /vx/h/excLib.h /vx/h/private/semLibP.h
mcpl_reset.o: /vx/h/private/classLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
mcpl_reset.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
mcpl_reset.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
mcpl_reset.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
mcpl_reset.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
mcpl_reset.o: /vx/h/sysLib.h /vx/h/bootLib.h
mcpl_reset.o: /home/thor/craig/mcpl/include/mcplDef.h
mcpl_reset.o: /home/thor/craig/mcpl/include/mcplGbl.h
mcpl_init.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
mcpl_init.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
mcpl_init.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
mcpl_init.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
mcpl_init.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
mcpl_init.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mcpl_init.o: /vx/h/private/objLibP.h /vx/h/intLib.h /vx/h/semLib.h
mcpl_init.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
mcpl_init.o: /vx/h/excLib.h /vx/h/private/semLibP.h /vx/h/private/classLibP.h
mcpl_init.o: /vx/h/stdlib.h /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h
mcpl_init.o: /vx/h/limits.h /vx/h/net/uio.h /vx/h/fcntl.h
mcpl_init.o: /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
mcpl_init.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
mcpl_init.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
mcpl_init.o: /vx/h/sysLib.h /vx/h/bootLib.h
mcpl_init.o: /home/thor/craig/mcpl/include/mcplDef.h
mcpl_init.o: /home/thor/craig/mcpl/include/mcplGbl.h
stand_xfer.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
stand_xfer.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
stand_xfer.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
stand_xfer.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
stand_xfer.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
stand_xfer.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
stand_xfer.o: /vx/h/private/objLibP.h /vx/h/intLib.h /vx/h/semLib.h
stand_xfer.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
stand_xfer.o: /vx/h/excLib.h /vx/h/private/semLibP.h
stand_xfer.o: /vx/h/private/classLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
stand_xfer.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
stand_xfer.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
stand_xfer.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
stand_xfer.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
stand_xfer.o: /vx/h/sysLib.h /vx/h/bootLib.h
stand_xfer.o: /home/thor/craig/mcpl/include/mcplDef.h
stand_xfer.o: /home/thor/craig/mcpl/include/mcplGbl.h
bcast_xfer.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
bcast_xfer.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
bcast_xfer.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
bcast_xfer.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
bcast_xfer.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
bcast_xfer.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
bcast_xfer.o: /vx/h/private/objLibP.h /vx/h/intLib.h /vx/h/semLib.h
bcast_xfer.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
bcast_xfer.o: /vx/h/excLib.h /vx/h/private/semLibP.h
bcast_xfer.o: /vx/h/private/classLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
bcast_xfer.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
bcast_xfer.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
bcast_xfer.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
bcast_xfer.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
bcast_xfer.o: /vx/h/sysLib.h /vx/h/bootLib.h
bcast_xfer.o: /home/thor/craig/mcpl/include/mcplDef.h
bcast_xfer.o: /home/thor/craig/mcpl/include/mcplGbl.h
mcpl_menu.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
mcpl_menu.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
mcpl_menu.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
mcpl_menu.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
mcpl_menu.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
mcpl_menu.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mcpl_menu.o: /vx/h/private/objLibP.h /vx/h/intLib.h /vx/h/semLib.h
mcpl_menu.o: /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
mcpl_menu.o: /vx/h/excLib.h /vx/h/private/semLibP.h /vx/h/private/classLibP.h
mcpl_menu.o: /vx/h/stdlib.h /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h
mcpl_menu.o: /vx/h/limits.h /vx/h/net/uio.h /vx/h/fcntl.h
mcpl_menu.o: /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
mcpl_menu.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
mcpl_menu.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
mcpl_menu.o: /vx/h/sysLib.h /vx/h/bootLib.h
mcpl_menu.o: /home/thor/craig/mcpl/include/mcplDef.h
mcpl_menu.o: /home/thor/craig/mcpl/include/mcplGbl.h
