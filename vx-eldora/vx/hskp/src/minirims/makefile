.KEEP_STATE:		# let make worry about it, Sun make dependent.
#
#	Makefile for Directory ELDORA/vx/hskp/src/minirims
#	Author: Craig Walther
#	Date: 8/19/92
#
# GNU C 
CC= gcc68k
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
#	-fomit-frame-pointer
#OPT= -O -fvolatile -fno-builtin -nostdinc -m68040
OPT= -O -fno-builtin -nostdinc
#
#DEFINES= -DUSE_ANSI -DMV133 -DVxWorks -m68020
DEFINES= -DUSE_ANSI -DVxWorks
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I. -I/ELDORA/rpc/header -I/ELDORA/vx/hskp/include\
	-I/ELDORA/vx/c++/include -I/ELDORA/rpc/include -I/ELDORA/vx/c/include \
	-I/vx/config/eldora.tp41  -I/ELDORA/vx/minirims/include
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a \
	/ELDORA/vx/tp41/tp41Lib.o \
	/ELDORA/vx/c/lib/libtermlib.a \
	-L/local/lib/gcc-lib/68k/current -lstdc++ -lgcc
#
OBJS= init_mini.o mini_isr.o mini_menu.o command_mini.o 
#
SRCS= init_mini.c mini_isr.c mini_menu.c command_mini.c
#    
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN= /ELDORA/vx/hskp/bin

.KEEP_STATE:

compile: $(OBJS)
#	ld68k -X -r -o minirims.o $(OBJS) $(LIBS)
#	 bsdinst -m 774 minirims.o $(BIN)

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

depend: 
	makedepend ${INCLUDES} ${SRCS}

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;

# Modify this to include an access list if desired.
set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -aericloew,granger,martinc -c" * " -L $$i); \
	done;

copyem:
	cp *.o /ELDORA/vx/hskp/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h



# DO NOT DELETE THIS LINE -- make depend depends on it.

init_mini.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
init_mini.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
init_mini.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
init_mini.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
init_mini.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
init_mini.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_mini.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
init_mini.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
init_mini.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
init_mini.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
init_mini.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
init_mini.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
init_mini.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
init_mini.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
init_mini.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
init_mini.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
init_mini.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
init_mini.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
init_mini.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
init_mini.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
init_mini.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
init_mini.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
init_mini.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
init_mini.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
init_mini.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
init_mini.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
init_mini.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
init_mini.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
init_mini.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
init_mini.o: /ELDORA/vx/hskp/include/cntrlDef.h
init_mini.o: /ELDORA/vx/hskp/include/cntrlFunc.h
init_mini.o: /ELDORA/vx/hskp/include/cntrlGbl.h
init_mini.o: /ELDORA/vx/hskp/include/ecbAdr.h
init_mini.o: /ELDORA/vx/hskp/include/ecbErrBound.h
init_mini.o: /ELDORA/vx/hskp/include/ecbFunc.h
init_mini.o: /ELDORA/vx/hskp/include/ecbMaster.h
init_mini.o: /ELDORA/vx/hskp/include/ecbSem.h
init_mini.o: /ELDORA/vx/hskp/include/ecbStat.h
init_mini.o: /ELDORA/vx/hskp/include/gpsDef.h
init_mini.o: /ELDORA/vx/hskp/include/gpsFunc.h
init_mini.o: /ELDORA/vx/hskp/include/gpsGbl.h
init_mini.o: /ELDORA/vx/hskp/include/hskpDef.h
init_mini.o: /ELDORA/vx/hskp/include/hskpFunc.h
init_mini.o: /ELDORA/vx/hskp/include/hskpGbl.h
init_mini.o: /ELDORA/vx/hskp/include/hskpInt.h
init_mini.o: /ELDORA/vx/hskp/include/iruDef.h
init_mini.o: /ELDORA/vx/hskp/include/iruFunc.h
init_mini.o: /ELDORA/vx/hskp/include/iruGbl.h
init_mini.o: /ELDORA/vx/hskp/include/pwrDef.h
init_mini.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_mini.o: /ELDORA/vx/hskp/include/todDef.h
init_mini.o: /ELDORA/vx/hskp/include/todFunc.h
init_mini.o: /ELDORA/vx/hskp/include/todGbl.h
init_mini.o: /ELDORA/vx/hskp/include/tp41vAdr.h
init_mini.o: /ELDORA/vx/hskp/include/vme_hndshk.h
init_mini.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
init_mini.o: /ELDORA/vx/hskp/include/vmevmeDef.h
init_mini.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
init_mini.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
init_mini.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
init_mini.o: /ELDORA/rpc/include/HskpStatus.h
init_mini.o: /ELDORA/vx/minirims/include/numinDef.h
mini_isr.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
mini_isr.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_isr.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
mini_isr.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_isr.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
mini_isr.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mini_isr.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
mini_isr.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
mini_isr.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
mini_isr.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
mini_isr.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
mini_isr.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
mini_isr.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
mini_isr.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
mini_isr.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
mini_isr.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
mini_isr.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
mini_isr.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mini_isr.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
mini_isr.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
mini_isr.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
mini_isr.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
mini_isr.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
mini_isr.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
mini_isr.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
mini_isr.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
mini_isr.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
mini_isr.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
mini_isr.o: /ELDORA/vx/hskp/include/cntrlDef.h
mini_isr.o: /ELDORA/vx/hskp/include/cntrlFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/cntrlGbl.h
mini_isr.o: /ELDORA/vx/hskp/include/ecbAdr.h
mini_isr.o: /ELDORA/vx/hskp/include/ecbErrBound.h
mini_isr.o: /ELDORA/vx/hskp/include/ecbFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/ecbMaster.h
mini_isr.o: /ELDORA/vx/hskp/include/ecbSem.h
mini_isr.o: /ELDORA/vx/hskp/include/ecbStat.h
mini_isr.o: /ELDORA/vx/hskp/include/gpsDef.h
mini_isr.o: /ELDORA/vx/hskp/include/gpsFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/gpsGbl.h
mini_isr.o: /ELDORA/vx/hskp/include/hskpDef.h
mini_isr.o: /ELDORA/vx/hskp/include/hskpFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/hskpGbl.h
mini_isr.o: /ELDORA/vx/hskp/include/hskpInt.h
mini_isr.o: /ELDORA/vx/hskp/include/iruDef.h
mini_isr.o: /ELDORA/vx/hskp/include/iruFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
mini_isr.o: /ELDORA/vx/hskp/include/pwrFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/todDef.h
mini_isr.o: /ELDORA/vx/hskp/include/todFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/todGbl.h
mini_isr.o: /ELDORA/vx/hskp/include/tp41vAdr.h
mini_isr.o: /ELDORA/vx/hskp/include/vme_hndshk.h
mini_isr.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
mini_isr.o: /ELDORA/vx/hskp/include/vmevmeDef.h
mini_isr.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
mini_isr.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
mini_isr.o: /ELDORA/rpc/include/GblCommands.h
mini_isr.o: /ELDORA/rpc/include/HskpStatus.h
mini_isr.o: /ELDORA/vx/minirims/include/numinDef.h
mini_isr.o: /vx/config/eldora.tp41/tp41.h /vx/h/drv/serial/z8530.h
mini_isr.o: /vx/h/drv/multi/z8536.h
mini_menu.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
mini_menu.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_menu.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
mini_menu.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_menu.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
mini_menu.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mini_menu.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
mini_menu.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
mini_menu.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
mini_menu.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
mini_menu.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
mini_menu.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
mini_menu.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
mini_menu.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
mini_menu.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
mini_menu.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
mini_menu.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
mini_menu.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
mini_menu.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mini_menu.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
mini_menu.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
mini_menu.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
mini_menu.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
mini_menu.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
mini_menu.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
mini_menu.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
mini_menu.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
mini_menu.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
mini_menu.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
mini_menu.o: /ELDORA/vx/hskp/include/cntrlDef.h
mini_menu.o: /ELDORA/vx/hskp/include/cntrlFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/cntrlGbl.h
mini_menu.o: /ELDORA/vx/hskp/include/ecbAdr.h
mini_menu.o: /ELDORA/vx/hskp/include/ecbErrBound.h
mini_menu.o: /ELDORA/vx/hskp/include/ecbFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/ecbMaster.h
mini_menu.o: /ELDORA/vx/hskp/include/ecbSem.h
mini_menu.o: /ELDORA/vx/hskp/include/ecbStat.h
mini_menu.o: /ELDORA/vx/hskp/include/gpsDef.h
mini_menu.o: /ELDORA/vx/hskp/include/gpsFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/gpsGbl.h
mini_menu.o: /ELDORA/vx/hskp/include/hskpDef.h
mini_menu.o: /ELDORA/vx/hskp/include/hskpFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/hskpGbl.h
mini_menu.o: /ELDORA/vx/hskp/include/hskpInt.h
mini_menu.o: /ELDORA/vx/hskp/include/iruDef.h
mini_menu.o: /ELDORA/vx/hskp/include/iruFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/iruGbl.h
mini_menu.o: /ELDORA/vx/hskp/include/pwrDef.h
mini_menu.o: /ELDORA/vx/hskp/include/pwrFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/todDef.h
mini_menu.o: /ELDORA/vx/hskp/include/todFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/todGbl.h
mini_menu.o: /ELDORA/vx/hskp/include/tp41vAdr.h
mini_menu.o: /ELDORA/vx/hskp/include/vme_hndshk.h
mini_menu.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
mini_menu.o: /ELDORA/vx/hskp/include/vmevmeDef.h
mini_menu.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
mini_menu.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
mini_menu.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
mini_menu.o: /ELDORA/rpc/include/HskpStatus.h
command_mini.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
command_mini.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
command_mini.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
command_mini.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
command_mini.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
command_mini.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
command_mini.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
command_mini.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
command_mini.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
command_mini.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
command_mini.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
command_mini.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
command_mini.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
command_mini.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h
command_mini.o: /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h
command_mini.o: /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
command_mini.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
command_mini.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
command_mini.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
command_mini.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
command_mini.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
command_mini.o: /ELDORA/rpc/header/FieldRadar.h
command_mini.o: /ELDORA/rpc/header/CellSpacing.h
command_mini.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
command_mini.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
command_mini.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
command_mini.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
command_mini.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
command_mini.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
command_mini.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
command_mini.o: /ELDORA/rpc/header/TapeHeader.h
command_mini.o: /ELDORA/rpc/header/HeaderRpc.h
command_mini.o: /ELDORA/vx/hskp/include/cntrlDef.h
command_mini.o: /ELDORA/vx/hskp/include/cntrlFunc.h
command_mini.o: /ELDORA/vx/hskp/include/cntrlGbl.h
command_mini.o: /ELDORA/vx/hskp/include/ecbAdr.h
command_mini.o: /ELDORA/vx/hskp/include/ecbErrBound.h
command_mini.o: /ELDORA/vx/hskp/include/ecbFunc.h
command_mini.o: /ELDORA/vx/hskp/include/ecbMaster.h
command_mini.o: /ELDORA/vx/hskp/include/ecbSem.h
command_mini.o: /ELDORA/vx/hskp/include/ecbStat.h
command_mini.o: /ELDORA/vx/hskp/include/gpsDef.h
command_mini.o: /ELDORA/vx/hskp/include/gpsFunc.h
command_mini.o: /ELDORA/vx/hskp/include/gpsGbl.h
command_mini.o: /ELDORA/vx/hskp/include/hskpDef.h
command_mini.o: /ELDORA/vx/hskp/include/hskpFunc.h
command_mini.o: /ELDORA/vx/hskp/include/hskpGbl.h
command_mini.o: /ELDORA/vx/hskp/include/hskpInt.h
command_mini.o: /ELDORA/vx/hskp/include/iruDef.h
command_mini.o: /ELDORA/vx/hskp/include/iruFunc.h
command_mini.o: /ELDORA/vx/hskp/include/iruGbl.h
command_mini.o: /ELDORA/vx/hskp/include/pwrDef.h
command_mini.o: /ELDORA/vx/hskp/include/pwrFunc.h
command_mini.o: /ELDORA/vx/hskp/include/todDef.h
command_mini.o: /ELDORA/vx/hskp/include/todFunc.h
command_mini.o: /ELDORA/vx/hskp/include/todGbl.h
command_mini.o: /ELDORA/vx/hskp/include/tp41vAdr.h
command_mini.o: /ELDORA/vx/hskp/include/vme_hndshk.h
command_mini.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
command_mini.o: /ELDORA/vx/hskp/include/vmevmeDef.h
command_mini.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
command_mini.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
command_mini.o: /ELDORA/rpc/include/HskpCmd.h
command_mini.o: /ELDORA/rpc/include/GblCommands.h
command_mini.o: /ELDORA/rpc/include/HskpStatus.h
command_mini.o: /ELDORA/vx/minirims/include/numinDef.h
