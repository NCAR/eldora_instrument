#
#	Makefile for Directory ELDORA/vx/hskp/src/gps
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
OPT= -O -fvolatile -fno-builtin -nostdinc -m68040
#
DEFINES= -DUSE_ANSI
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I. -I/ELDORA/rpc/header -I/ELDORA/vx/hskp/include\
		-I/ELDORA/vx/c++/include -I/ELDORA/rpc/include\
		-I/ELDORA/vx/include -I/vx/h/drv -I/vx/config/eldora.tp41
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS= init_gps.o command_gps.o gps_isr.o  gps_menu.o
#
SRCS= init_gps.c command_gps.c gps_isr.c gps_menu.c
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

#	 bsdinst -m 774 $(OBJS) $(BIN)

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;
		
depend: 
	makedepend ${INCLUDES} ${SRCS}

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
		do (echo "rcs -c\" * \" -L $$i";rcs -aericloew,granger,martinc -c" * " -L $$i); \
	done;

copyem:
	cp *.o /ELDORA/vx/hskp/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h


# DO NOT DELETE

init_gps.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
init_gps.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
init_gps.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
init_gps.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
init_gps.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
init_gps.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_gps.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
init_gps.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
init_gps.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
init_gps.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
init_gps.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
init_gps.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
init_gps.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
init_gps.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
init_gps.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
init_gps.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
init_gps.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
init_gps.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
init_gps.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
init_gps.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
init_gps.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
init_gps.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
init_gps.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
init_gps.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
init_gps.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
init_gps.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
init_gps.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
init_gps.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
init_gps.o: /ELDORA/vx/hskp/include/cntrlDef.h
init_gps.o: /ELDORA/vx/hskp/include/cntrlFunc.h
init_gps.o: /ELDORA/vx/hskp/include/cntrlGbl.h
init_gps.o: /ELDORA/vx/hskp/include/ecbAdr.h
init_gps.o: /ELDORA/vx/hskp/include/ecbErrBound.h
init_gps.o: /ELDORA/vx/hskp/include/ecbFunc.h
init_gps.o: /ELDORA/vx/hskp/include/ecbMaster.h
init_gps.o: /ELDORA/vx/hskp/include/ecbSem.h
init_gps.o: /ELDORA/vx/hskp/include/ecbStat.h
init_gps.o: /ELDORA/vx/hskp/include/gpsDef.h
init_gps.o: /ELDORA/vx/hskp/include/gpsFunc.h
init_gps.o: /ELDORA/vx/hskp/include/gpsGbl.h
init_gps.o: /ELDORA/vx/hskp/include/hskpDef.h
init_gps.o: /ELDORA/vx/hskp/include/hskpFunc.h
init_gps.o: /ELDORA/vx/hskp/include/hskpGbl.h
init_gps.o: /ELDORA/vx/hskp/include/hskpInt.h
init_gps.o: /ELDORA/vx/hskp/include/iruDef.h
init_gps.o: /ELDORA/vx/hskp/include/iruFunc.h
init_gps.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
init_gps.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_gps.o: /ELDORA/vx/hskp/include/todDef.h
init_gps.o: /ELDORA/vx/hskp/include/todFunc.h
init_gps.o: /ELDORA/vx/hskp/include/todGbl.h
init_gps.o: /ELDORA/vx/hskp/include/tp41vAdr.h
init_gps.o: /ELDORA/vx/hskp/include/vme_hndshk.h
init_gps.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
init_gps.o: /ELDORA/vx/hskp/include/vmevmeDef.h
init_gps.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
init_gps.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
init_gps.o: /ELDORA/rpc/include/GblCommands.h
init_gps.o: /ELDORA/rpc/include/HskpStatus.h
command_gps.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
command_gps.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
command_gps.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
command_gps.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
command_gps.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
command_gps.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
command_gps.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
command_gps.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
command_gps.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
command_gps.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
command_gps.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
command_gps.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
command_gps.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
command_gps.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
command_gps.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
command_gps.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
command_gps.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
command_gps.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
command_gps.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
command_gps.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
command_gps.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
command_gps.o: /ELDORA/rpc/header/FieldRadar.h
command_gps.o: /ELDORA/rpc/header/CellSpacing.h
command_gps.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
command_gps.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
command_gps.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
command_gps.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
command_gps.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
command_gps.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
command_gps.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
command_gps.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
command_gps.o: /ELDORA/vx/hskp/include/cntrlDef.h
command_gps.o: /ELDORA/vx/hskp/include/cntrlFunc.h
command_gps.o: /ELDORA/vx/hskp/include/cntrlGbl.h
command_gps.o: /ELDORA/vx/hskp/include/ecbAdr.h
command_gps.o: /ELDORA/vx/hskp/include/ecbErrBound.h
command_gps.o: /ELDORA/vx/hskp/include/ecbFunc.h
command_gps.o: /ELDORA/vx/hskp/include/ecbMaster.h
command_gps.o: /ELDORA/vx/hskp/include/ecbSem.h
command_gps.o: /ELDORA/vx/hskp/include/ecbStat.h
command_gps.o: /ELDORA/vx/hskp/include/gpsDef.h
command_gps.o: /ELDORA/vx/hskp/include/gpsFunc.h
command_gps.o: /ELDORA/vx/hskp/include/gpsGbl.h
command_gps.o: /ELDORA/vx/hskp/include/hskpDef.h
command_gps.o: /ELDORA/vx/hskp/include/hskpFunc.h
command_gps.o: /ELDORA/vx/hskp/include/hskpGbl.h
command_gps.o: /ELDORA/vx/hskp/include/hskpInt.h
command_gps.o: /ELDORA/vx/hskp/include/iruDef.h
command_gps.o: /ELDORA/vx/hskp/include/iruFunc.h
command_gps.o: /ELDORA/vx/hskp/include/iruGbl.h
command_gps.o: /ELDORA/vx/hskp/include/pwrDef.h
command_gps.o: /ELDORA/vx/hskp/include/pwrFunc.h
command_gps.o: /ELDORA/vx/hskp/include/todDef.h
command_gps.o: /ELDORA/vx/hskp/include/todFunc.h
command_gps.o: /ELDORA/vx/hskp/include/todGbl.h
command_gps.o: /ELDORA/vx/hskp/include/tp41vAdr.h
command_gps.o: /ELDORA/vx/hskp/include/vme_hndshk.h
command_gps.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
command_gps.o: /ELDORA/vx/hskp/include/vmevmeDef.h
command_gps.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
command_gps.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
command_gps.o: /ELDORA/rpc/include/HskpCmd.h
command_gps.o: /ELDORA/rpc/include/GblCommands.h
command_gps.o: /ELDORA/rpc/include/HskpStatus.h
gps_isr.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
gps_isr.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
gps_isr.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
gps_isr.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
gps_isr.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
gps_isr.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
gps_isr.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
gps_isr.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
gps_isr.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
gps_isr.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
gps_isr.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
gps_isr.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
gps_isr.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
gps_isr.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
gps_isr.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
gps_isr.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
gps_isr.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
gps_isr.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
gps_isr.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
gps_isr.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
gps_isr.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
gps_isr.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
gps_isr.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
gps_isr.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
gps_isr.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
gps_isr.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
gps_isr.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
gps_isr.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
gps_isr.o: /ELDORA/vx/hskp/include/cntrlDef.h
gps_isr.o: /ELDORA/vx/hskp/include/cntrlFunc.h
gps_isr.o: /ELDORA/vx/hskp/include/cntrlGbl.h
gps_isr.o: /ELDORA/vx/hskp/include/ecbAdr.h
gps_isr.o: /ELDORA/vx/hskp/include/ecbErrBound.h
gps_isr.o: /ELDORA/vx/hskp/include/ecbFunc.h
gps_isr.o: /ELDORA/vx/hskp/include/ecbMaster.h
gps_isr.o: /ELDORA/vx/hskp/include/ecbSem.h /ELDORA/vx/hskp/include/ecbStat.h
gps_isr.o: /ELDORA/vx/hskp/include/gpsDef.h /ELDORA/vx/hskp/include/gpsFunc.h
gps_isr.o: /ELDORA/vx/hskp/include/gpsGbl.h /ELDORA/vx/hskp/include/hskpDef.h
gps_isr.o: /ELDORA/vx/hskp/include/hskpFunc.h
gps_isr.o: /ELDORA/vx/hskp/include/hskpGbl.h
gps_isr.o: /ELDORA/vx/hskp/include/hskpInt.h /ELDORA/vx/hskp/include/iruDef.h
gps_isr.o: /ELDORA/vx/hskp/include/iruFunc.h /ELDORA/vx/hskp/include/iruGbl.h
gps_isr.o: /ELDORA/vx/hskp/include/pwrDef.h /ELDORA/vx/hskp/include/pwrFunc.h
gps_isr.o: /ELDORA/vx/hskp/include/todDef.h /ELDORA/vx/hskp/include/todFunc.h
gps_isr.o: /ELDORA/vx/hskp/include/todGbl.h
gps_isr.o: /ELDORA/vx/hskp/include/tp41vAdr.h
gps_isr.o: /ELDORA/vx/hskp/include/vme_hndshk.h
gps_isr.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
gps_isr.o: /ELDORA/vx/hskp/include/vmevmeDef.h
gps_isr.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
gps_isr.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
gps_isr.o: /ELDORA/rpc/include/GblCommands.h /ELDORA/rpc/include/HskpStatus.h
gps_isr.o: /vx/config/eldora.tp41/tp41.h /vx/h/drv/serial/z8530.h
gps_isr.o: /vx/h/drv/multi/z8536.h
gps_menu.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
gps_menu.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
gps_menu.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
gps_menu.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
gps_menu.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
gps_menu.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
gps_menu.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
gps_menu.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
gps_menu.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
gps_menu.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
gps_menu.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
gps_menu.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
gps_menu.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
gps_menu.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
gps_menu.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
gps_menu.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
gps_menu.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
gps_menu.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
gps_menu.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
gps_menu.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
gps_menu.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
gps_menu.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
gps_menu.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
gps_menu.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
gps_menu.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
gps_menu.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
gps_menu.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
gps_menu.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
gps_menu.o: /ELDORA/vx/hskp/include/cntrlDef.h
gps_menu.o: /ELDORA/vx/hskp/include/cntrlFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/cntrlGbl.h
gps_menu.o: /ELDORA/vx/hskp/include/ecbAdr.h
gps_menu.o: /ELDORA/vx/hskp/include/ecbErrBound.h
gps_menu.o: /ELDORA/vx/hskp/include/ecbFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/ecbMaster.h
gps_menu.o: /ELDORA/vx/hskp/include/ecbSem.h
gps_menu.o: /ELDORA/vx/hskp/include/ecbStat.h
gps_menu.o: /ELDORA/vx/hskp/include/gpsDef.h
gps_menu.o: /ELDORA/vx/hskp/include/gpsFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/gpsGbl.h
gps_menu.o: /ELDORA/vx/hskp/include/hskpDef.h
gps_menu.o: /ELDORA/vx/hskp/include/hskpFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/hskpGbl.h
gps_menu.o: /ELDORA/vx/hskp/include/hskpInt.h
gps_menu.o: /ELDORA/vx/hskp/include/iruDef.h
gps_menu.o: /ELDORA/vx/hskp/include/iruFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
gps_menu.o: /ELDORA/vx/hskp/include/pwrFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/todDef.h
gps_menu.o: /ELDORA/vx/hskp/include/todFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/todGbl.h
gps_menu.o: /ELDORA/vx/hskp/include/tp41vAdr.h
gps_menu.o: /ELDORA/vx/hskp/include/vme_hndshk.h
gps_menu.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
gps_menu.o: /ELDORA/vx/hskp/include/vmevmeDef.h
gps_menu.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
gps_menu.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
gps_menu.o: /ELDORA/rpc/include/GblCommands.h
gps_menu.o: /ELDORA/rpc/include/HskpStatus.h
