#
#	Makefile for Directory /net/eldora/eldora/vx/hskp/src/vmevme
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
DEFINES= -DUSE_ANSI -DUSE_INT_HNDSHK
#
# Drag in includes from here.
INCLUDES= -I/net/vx/h -I. -I/net/eldora/eldora/rpc/include -I/net/eldora/eldora/vx/hskp/include\
	-I/net/eldora/eldora/vx/c++/include -I/net/eldora/eldora/rpc/hskp\
	-I/net/eldora/eldora/rpc/header -I/net/eldora/eldora/vx/vmevme_int
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /net/eldora/eldora/rpc/lib/libVxHeader.a /net/eldora/eldora/vx/c++/bin/builtin++.o\
	/net/eldora/eldora/vx/c++/ctor_dummy.o
#
OBJS= init_vmevme.o stop_vmevme.o start_vmevme.o fill_nav_info.o\
	vmevme_menu.o vmevme_isr.o fill_platform.o midbeam.o
#
SRCS= init_vmevme.c stop_vmevme.c start_vmevme.c fill_nav_info.c\
	vmevme_menu.c vmevme_isr.c fill_platform.c midbeam.c
#    
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN= /net/eldora/eldora/vx/hskp/bin

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
		do (echo "rcs -c\" * \" -L $$i";rcs -aericloew,granger,martinc -c" * " -L $$i); \
	done;

copyem:
	cp *.o /net/eldora/eldora/vx/hskp/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h


# DO NOT DELETE

init_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h
init_vmevme.o: /net/vx/h/vxWorks.h /net/vx/h/types/vxCpu.h
init_vmevme.o: /net/vx/h/types/vxArch.h /net/vx/h/types/vxParams.h
init_vmevme.o: /net/vx/h/types/vxTypesBase.h /net/vx/h/types/vxTypes.h
init_vmevme.o: /net/vx/h/types/vxANSI.h /net/vx/h/types/vxTypesOld.h
init_vmevme.o: /net/vx/h/sys/types.h /net/vx/h/math.h /net/vx/h/stdioLib.h
init_vmevme.o: /net/vx/h/stdio.h /net/vx/h/classLib.h /net/vx/h/vwModNum.h
init_vmevme.o: /net/vx/h/objLib.h /net/vx/h/errno.h /net/vx/h/memLib.h
init_vmevme.o: /net/vx/h/private/objLibP.h /net/vx/h/private/classLibP.h
init_vmevme.o: /net/vx/h/stdarg.h /net/vx/h/intLib.h /net/vx/h/semLib.h
init_vmevme.o: /net/vx/h/taskLib.h /net/vx/h/qLib.h /net/vx/h/qClass.h
init_vmevme.o: /net/vx/h/regs.h /net/vx/h/excLib.h /net/vx/h/private/eventP.h
init_vmevme.o: /net/vx/h/private/semLibP.h /net/vx/h/stdlib.h
init_vmevme.o: /net/vx/h/tyLib.h /net/vx/h/iosLib.h /net/vx/h/ioLib.h
init_vmevme.o: /net/vx/h/limits.h /net/vx/h/net/uio.h /net/vx/h/fcntl.h
init_vmevme.o: /net/vx/h/sys/fcntlcom.h /net/vx/h/unistd.h /net/vx/h/dllLib.h
init_vmevme.o: /net/vx/h/rngLib.h /net/vx/h/selectLib.h /net/vx/h/lstLib.h
init_vmevme.o: /net/vx/h/sys/times.h /net/vx/h/in.h /net/vx/h/netinet/in.h
init_vmevme.o: /net/vx/h/systime.h /net/vx/h/sysLib.h /net/vx/h/bootLib.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Volume.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Waveform.h
init_vmevme.o: /net/eldora/eldora/rpc/include/RadarDesc.h
init_vmevme.o: /net/eldora/eldora/rpc/include/FieldRadar.h
init_vmevme.o: /net/eldora/eldora/rpc/include/CellSpacing.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Parameter.h
init_vmevme.o: /net/eldora/eldora/rpc/include/NavDesc.h
init_vmevme.o: /net/eldora/eldora/rpc/include/InSitu.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Ray.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Platform.h
init_vmevme.o: /net/eldora/eldora/rpc/include/FieldParam.h
init_vmevme.o: /net/eldora/eldora/rpc/include/IndFreq.h
init_vmevme.o: /net/eldora/eldora/rpc/include/TimeSeries.h
init_vmevme.o: /net/eldora/eldora/rpc/include/NavInfo.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Ins.h
init_vmevme.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
init_vmevme.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
init_vmevme.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h
init_vmevme.o: /net/vx/h/rpc/clnt.h /net/vx/h/rpc/rpc_msg.h
init_vmevme.o: /net/vx/h/rpc/auth_unix.h /net/vx/h/rpc/svc.h
init_vmevme.o: /net/vx/h/rpc/svc_auth.h
init_vmevme.o: /net/eldora/eldora/rpc/include/InSituData.h
init_vmevme.o: /net/eldora/eldora/rpc/include/Header.h
init_vmevme.o: /net/eldora/eldora/rpc/include/TapeHeader.h
init_vmevme.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
init_vmevme.o: /net/eldora/eldora/rpc/include/HskpCmd.h
init_vmevme.o: /net/eldora/eldora/rpc/include/GblCommands.h
init_vmevme.o: /net/eldora/eldora/rpc/include/HskpStatus.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
init_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h
stop_vmevme.o: /net/vx/h/vxWorks.h /net/vx/h/types/vxCpu.h
stop_vmevme.o: /net/vx/h/types/vxArch.h /net/vx/h/types/vxParams.h
stop_vmevme.o: /net/vx/h/types/vxTypesBase.h /net/vx/h/types/vxTypes.h
stop_vmevme.o: /net/vx/h/types/vxANSI.h /net/vx/h/types/vxTypesOld.h
stop_vmevme.o: /net/vx/h/sys/types.h /net/vx/h/math.h /net/vx/h/stdioLib.h
stop_vmevme.o: /net/vx/h/stdio.h /net/vx/h/classLib.h /net/vx/h/vwModNum.h
stop_vmevme.o: /net/vx/h/objLib.h /net/vx/h/errno.h /net/vx/h/memLib.h
stop_vmevme.o: /net/vx/h/private/objLibP.h /net/vx/h/private/classLibP.h
stop_vmevme.o: /net/vx/h/stdarg.h /net/vx/h/intLib.h /net/vx/h/semLib.h
stop_vmevme.o: /net/vx/h/taskLib.h /net/vx/h/qLib.h /net/vx/h/qClass.h
stop_vmevme.o: /net/vx/h/regs.h /net/vx/h/excLib.h /net/vx/h/private/eventP.h
stop_vmevme.o: /net/vx/h/private/semLibP.h /net/vx/h/stdlib.h
stop_vmevme.o: /net/vx/h/tyLib.h /net/vx/h/iosLib.h /net/vx/h/ioLib.h
stop_vmevme.o: /net/vx/h/limits.h /net/vx/h/net/uio.h /net/vx/h/fcntl.h
stop_vmevme.o: /net/vx/h/sys/fcntlcom.h /net/vx/h/unistd.h /net/vx/h/dllLib.h
stop_vmevme.o: /net/vx/h/rngLib.h /net/vx/h/selectLib.h /net/vx/h/lstLib.h
stop_vmevme.o: /net/vx/h/sys/times.h /net/vx/h/in.h /net/vx/h/netinet/in.h
stop_vmevme.o: /net/vx/h/systime.h /net/vx/h/sysLib.h /net/vx/h/bootLib.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Volume.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Waveform.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/RadarDesc.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/FieldRadar.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/CellSpacing.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Parameter.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/NavDesc.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/InSitu.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Ray.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Platform.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/FieldParam.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/IndFreq.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/TimeSeries.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/NavInfo.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Ins.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
stop_vmevme.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
stop_vmevme.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h
stop_vmevme.o: /net/vx/h/rpc/clnt.h /net/vx/h/rpc/rpc_msg.h
stop_vmevme.o: /net/vx/h/rpc/auth_unix.h /net/vx/h/rpc/svc.h
stop_vmevme.o: /net/vx/h/rpc/svc_auth.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/InSituData.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/Header.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/TapeHeader.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/HskpCmd.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/GblCommands.h
stop_vmevme.o: /net/eldora/eldora/rpc/include/HskpStatus.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
stop_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h
start_vmevme.o: /net/vx/h/vxWorks.h /net/vx/h/types/vxCpu.h
start_vmevme.o: /net/vx/h/types/vxArch.h /net/vx/h/types/vxParams.h
start_vmevme.o: /net/vx/h/types/vxTypesBase.h /net/vx/h/types/vxTypes.h
start_vmevme.o: /net/vx/h/types/vxANSI.h /net/vx/h/types/vxTypesOld.h
start_vmevme.o: /net/vx/h/sys/types.h /net/vx/h/math.h /net/vx/h/stdioLib.h
start_vmevme.o: /net/vx/h/stdio.h /net/vx/h/classLib.h /net/vx/h/vwModNum.h
start_vmevme.o: /net/vx/h/objLib.h /net/vx/h/errno.h /net/vx/h/memLib.h
start_vmevme.o: /net/vx/h/private/objLibP.h /net/vx/h/private/classLibP.h
start_vmevme.o: /net/vx/h/stdarg.h /net/vx/h/intLib.h /net/vx/h/semLib.h
start_vmevme.o: /net/vx/h/taskLib.h /net/vx/h/qLib.h /net/vx/h/qClass.h
start_vmevme.o: /net/vx/h/regs.h /net/vx/h/excLib.h
start_vmevme.o: /net/vx/h/private/eventP.h /net/vx/h/private/semLibP.h
start_vmevme.o: /net/vx/h/stdlib.h /net/vx/h/tyLib.h /net/vx/h/iosLib.h
start_vmevme.o: /net/vx/h/ioLib.h /net/vx/h/limits.h /net/vx/h/net/uio.h
start_vmevme.o: /net/vx/h/fcntl.h /net/vx/h/sys/fcntlcom.h /net/vx/h/unistd.h
start_vmevme.o: /net/vx/h/dllLib.h /net/vx/h/rngLib.h /net/vx/h/selectLib.h
start_vmevme.o: /net/vx/h/lstLib.h /net/vx/h/sys/times.h /net/vx/h/in.h
start_vmevme.o: /net/vx/h/netinet/in.h /net/vx/h/systime.h /net/vx/h/sysLib.h
start_vmevme.o: /net/vx/h/bootLib.h /net/eldora/eldora/rpc/include/Volume.h
start_vmevme.o: /net/eldora/eldora/rpc/include/Waveform.h
start_vmevme.o: /net/eldora/eldora/rpc/include/RadarDesc.h
start_vmevme.o: /net/eldora/eldora/rpc/include/FieldRadar.h
start_vmevme.o: /net/eldora/eldora/rpc/include/CellSpacing.h
start_vmevme.o: /net/eldora/eldora/rpc/include/Parameter.h
start_vmevme.o: /net/eldora/eldora/rpc/include/NavDesc.h
start_vmevme.o: /net/eldora/eldora/rpc/include/InSitu.h
start_vmevme.o: /net/eldora/eldora/rpc/include/Ray.h
start_vmevme.o: /net/eldora/eldora/rpc/include/Platform.h
start_vmevme.o: /net/eldora/eldora/rpc/include/FieldParam.h
start_vmevme.o: /net/eldora/eldora/rpc/include/IndFreq.h
start_vmevme.o: /net/eldora/eldora/rpc/include/TimeSeries.h
start_vmevme.o: /net/eldora/eldora/rpc/include/NavInfo.h
start_vmevme.o: /net/eldora/eldora/rpc/include/Ins.h
start_vmevme.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
start_vmevme.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
start_vmevme.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
start_vmevme.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h
start_vmevme.o: /net/vx/h/rpc/clnt.h /net/vx/h/rpc/rpc_msg.h
start_vmevme.o: /net/vx/h/rpc/auth_unix.h /net/vx/h/rpc/svc.h
start_vmevme.o: /net/vx/h/rpc/svc_auth.h
start_vmevme.o: /net/eldora/eldora/rpc/include/InSituData.h
start_vmevme.o: /net/eldora/eldora/rpc/include/Header.h
start_vmevme.o: /net/eldora/eldora/rpc/include/TapeHeader.h
start_vmevme.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
start_vmevme.o: /net/eldora/eldora/rpc/include/HskpCmd.h
start_vmevme.o: /net/eldora/eldora/rpc/include/GblCommands.h
start_vmevme.o: /net/eldora/eldora/rpc/include/HskpStatus.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
start_vmevme.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h
fill_nav_info.o: /net/vx/h/vxWorks.h /net/vx/h/types/vxCpu.h
fill_nav_info.o: /net/vx/h/types/vxArch.h /net/vx/h/types/vxParams.h
fill_nav_info.o: /net/vx/h/types/vxTypesBase.h /net/vx/h/types/vxTypes.h
fill_nav_info.o: /net/vx/h/types/vxANSI.h /net/vx/h/types/vxTypesOld.h
fill_nav_info.o: /net/vx/h/sys/types.h /net/vx/h/math.h /net/vx/h/stdioLib.h
fill_nav_info.o: /net/vx/h/stdio.h /net/vx/h/classLib.h /net/vx/h/vwModNum.h
fill_nav_info.o: /net/vx/h/objLib.h /net/vx/h/errno.h /net/vx/h/memLib.h
fill_nav_info.o: /net/vx/h/private/objLibP.h /net/vx/h/private/classLibP.h
fill_nav_info.o: /net/vx/h/stdarg.h /net/vx/h/intLib.h /net/vx/h/semLib.h
fill_nav_info.o: /net/vx/h/taskLib.h /net/vx/h/qLib.h /net/vx/h/qClass.h
fill_nav_info.o: /net/vx/h/regs.h /net/vx/h/excLib.h
fill_nav_info.o: /net/vx/h/private/eventP.h /net/vx/h/private/semLibP.h
fill_nav_info.o: /net/vx/h/stdlib.h /net/vx/h/tyLib.h /net/vx/h/iosLib.h
fill_nav_info.o: /net/vx/h/ioLib.h /net/vx/h/limits.h /net/vx/h/net/uio.h
fill_nav_info.o: /net/vx/h/fcntl.h /net/vx/h/sys/fcntlcom.h
fill_nav_info.o: /net/vx/h/unistd.h /net/vx/h/dllLib.h /net/vx/h/rngLib.h
fill_nav_info.o: /net/vx/h/selectLib.h /net/vx/h/lstLib.h
fill_nav_info.o: /net/vx/h/sys/times.h /net/vx/h/in.h /net/vx/h/netinet/in.h
fill_nav_info.o: /net/vx/h/systime.h /net/vx/h/sysLib.h /net/vx/h/bootLib.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Volume.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Waveform.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/RadarDesc.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/FieldRadar.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/CellSpacing.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Parameter.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/NavDesc.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/InSitu.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Ray.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Platform.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/FieldParam.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/IndFreq.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/TimeSeries.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/NavInfo.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Ins.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
fill_nav_info.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
fill_nav_info.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h
fill_nav_info.o: /net/vx/h/rpc/clnt.h /net/vx/h/rpc/rpc_msg.h
fill_nav_info.o: /net/vx/h/rpc/auth_unix.h /net/vx/h/rpc/svc.h
fill_nav_info.o: /net/vx/h/rpc/svc_auth.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/InSituData.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/Header.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/TapeHeader.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/HskpCmd.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/GblCommands.h
fill_nav_info.o: /net/eldora/eldora/rpc/include/HskpStatus.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
fill_nav_info.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h
vmevme_menu.o: /net/vx/h/vxWorks.h /net/vx/h/types/vxCpu.h
vmevme_menu.o: /net/vx/h/types/vxArch.h /net/vx/h/types/vxParams.h
vmevme_menu.o: /net/vx/h/types/vxTypesBase.h /net/vx/h/types/vxTypes.h
vmevme_menu.o: /net/vx/h/types/vxANSI.h /net/vx/h/types/vxTypesOld.h
vmevme_menu.o: /net/vx/h/sys/types.h /net/vx/h/math.h /net/vx/h/stdioLib.h
vmevme_menu.o: /net/vx/h/stdio.h /net/vx/h/classLib.h /net/vx/h/vwModNum.h
vmevme_menu.o: /net/vx/h/objLib.h /net/vx/h/errno.h /net/vx/h/memLib.h
vmevme_menu.o: /net/vx/h/private/objLibP.h /net/vx/h/private/classLibP.h
vmevme_menu.o: /net/vx/h/stdarg.h /net/vx/h/intLib.h /net/vx/h/semLib.h
vmevme_menu.o: /net/vx/h/taskLib.h /net/vx/h/qLib.h /net/vx/h/qClass.h
vmevme_menu.o: /net/vx/h/regs.h /net/vx/h/excLib.h /net/vx/h/private/eventP.h
vmevme_menu.o: /net/vx/h/private/semLibP.h /net/vx/h/stdlib.h
vmevme_menu.o: /net/vx/h/tyLib.h /net/vx/h/iosLib.h /net/vx/h/ioLib.h
vmevme_menu.o: /net/vx/h/limits.h /net/vx/h/net/uio.h /net/vx/h/fcntl.h
vmevme_menu.o: /net/vx/h/sys/fcntlcom.h /net/vx/h/unistd.h /net/vx/h/dllLib.h
vmevme_menu.o: /net/vx/h/rngLib.h /net/vx/h/selectLib.h /net/vx/h/lstLib.h
vmevme_menu.o: /net/vx/h/sys/times.h /net/vx/h/in.h /net/vx/h/netinet/in.h
vmevme_menu.o: /net/vx/h/systime.h /net/vx/h/sysLib.h /net/vx/h/bootLib.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Volume.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Waveform.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/RadarDesc.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/FieldRadar.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/CellSpacing.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Parameter.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/NavDesc.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/InSitu.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Ray.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Platform.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/FieldParam.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/IndFreq.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/TimeSeries.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/NavInfo.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Ins.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
vmevme_menu.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
vmevme_menu.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h
vmevme_menu.o: /net/vx/h/rpc/clnt.h /net/vx/h/rpc/rpc_msg.h
vmevme_menu.o: /net/vx/h/rpc/auth_unix.h /net/vx/h/rpc/svc.h
vmevme_menu.o: /net/vx/h/rpc/svc_auth.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/InSituData.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/Header.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/TapeHeader.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/HskpCmd.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/GblCommands.h
vmevme_menu.o: /net/eldora/eldora/rpc/include/HskpStatus.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
vmevme_menu.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h
vmevme_isr.o: /net/vx/h/vxWorks.h /net/vx/h/types/vxCpu.h
vmevme_isr.o: /net/vx/h/types/vxArch.h /net/vx/h/types/vxParams.h
vmevme_isr.o: /net/vx/h/types/vxTypesBase.h /net/vx/h/types/vxTypes.h
vmevme_isr.o: /net/vx/h/types/vxANSI.h /net/vx/h/types/vxTypesOld.h
vmevme_isr.o: /net/vx/h/sys/types.h /net/vx/h/math.h /net/vx/h/stdioLib.h
vmevme_isr.o: /net/vx/h/stdio.h /net/vx/h/classLib.h /net/vx/h/vwModNum.h
vmevme_isr.o: /net/vx/h/objLib.h /net/vx/h/errno.h /net/vx/h/memLib.h
vmevme_isr.o: /net/vx/h/private/objLibP.h /net/vx/h/private/classLibP.h
vmevme_isr.o: /net/vx/h/stdarg.h /net/vx/h/intLib.h /net/vx/h/semLib.h
vmevme_isr.o: /net/vx/h/taskLib.h /net/vx/h/qLib.h /net/vx/h/qClass.h
vmevme_isr.o: /net/vx/h/regs.h /net/vx/h/excLib.h /net/vx/h/private/eventP.h
vmevme_isr.o: /net/vx/h/private/semLibP.h /net/vx/h/stdlib.h
vmevme_isr.o: /net/vx/h/tyLib.h /net/vx/h/iosLib.h /net/vx/h/ioLib.h
vmevme_isr.o: /net/vx/h/limits.h /net/vx/h/net/uio.h /net/vx/h/fcntl.h
vmevme_isr.o: /net/vx/h/sys/fcntlcom.h /net/vx/h/unistd.h /net/vx/h/dllLib.h
vmevme_isr.o: /net/vx/h/rngLib.h /net/vx/h/selectLib.h /net/vx/h/lstLib.h
vmevme_isr.o: /net/vx/h/sys/times.h /net/vx/h/in.h /net/vx/h/netinet/in.h
vmevme_isr.o: /net/vx/h/systime.h /net/vx/h/sysLib.h /net/vx/h/bootLib.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Volume.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Waveform.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/RadarDesc.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/FieldRadar.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/CellSpacing.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Parameter.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/NavDesc.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/InSitu.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Ray.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Platform.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/FieldParam.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/IndFreq.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/TimeSeries.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/NavInfo.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Ins.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
vmevme_isr.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
vmevme_isr.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h
vmevme_isr.o: /net/vx/h/rpc/clnt.h /net/vx/h/rpc/rpc_msg.h
vmevme_isr.o: /net/vx/h/rpc/auth_unix.h /net/vx/h/rpc/svc.h
vmevme_isr.o: /net/vx/h/rpc/svc_auth.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/InSituData.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/Header.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/TapeHeader.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/HskpCmd.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/GblCommands.h
vmevme_isr.o: /net/eldora/eldora/rpc/include/HskpStatus.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
vmevme_isr.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h
fill_platform.o: /net/vx/h/vxWorks.h /net/vx/h/types/vxCpu.h
fill_platform.o: /net/vx/h/types/vxArch.h /net/vx/h/types/vxParams.h
fill_platform.o: /net/vx/h/types/vxTypesBase.h /net/vx/h/types/vxTypes.h
fill_platform.o: /net/vx/h/types/vxANSI.h /net/vx/h/types/vxTypesOld.h
fill_platform.o: /net/vx/h/sys/types.h /net/vx/h/math.h /net/vx/h/stdioLib.h
fill_platform.o: /net/vx/h/stdio.h /net/vx/h/classLib.h /net/vx/h/vwModNum.h
fill_platform.o: /net/vx/h/objLib.h /net/vx/h/errno.h /net/vx/h/memLib.h
fill_platform.o: /net/vx/h/private/objLibP.h /net/vx/h/private/classLibP.h
fill_platform.o: /net/vx/h/stdarg.h /net/vx/h/intLib.h /net/vx/h/semLib.h
fill_platform.o: /net/vx/h/taskLib.h /net/vx/h/qLib.h /net/vx/h/qClass.h
fill_platform.o: /net/vx/h/regs.h /net/vx/h/excLib.h
fill_platform.o: /net/vx/h/private/eventP.h /net/vx/h/private/semLibP.h
fill_platform.o: /net/vx/h/stdlib.h /net/vx/h/tyLib.h /net/vx/h/iosLib.h
fill_platform.o: /net/vx/h/ioLib.h /net/vx/h/limits.h /net/vx/h/net/uio.h
fill_platform.o: /net/vx/h/fcntl.h /net/vx/h/sys/fcntlcom.h
fill_platform.o: /net/vx/h/unistd.h /net/vx/h/dllLib.h /net/vx/h/rngLib.h
fill_platform.o: /net/vx/h/selectLib.h /net/vx/h/lstLib.h
fill_platform.o: /net/vx/h/sys/times.h /net/vx/h/in.h /net/vx/h/netinet/in.h
fill_platform.o: /net/vx/h/systime.h /net/vx/h/sysLib.h /net/vx/h/bootLib.h
fill_platform.o: /net/eldora/eldora/rpc/include/Volume.h
fill_platform.o: /net/eldora/eldora/rpc/include/Waveform.h
fill_platform.o: /net/eldora/eldora/rpc/include/RadarDesc.h
fill_platform.o: /net/eldora/eldora/rpc/include/FieldRadar.h
fill_platform.o: /net/eldora/eldora/rpc/include/CellSpacing.h
fill_platform.o: /net/eldora/eldora/rpc/include/Parameter.h
fill_platform.o: /net/eldora/eldora/rpc/include/NavDesc.h
fill_platform.o: /net/eldora/eldora/rpc/include/InSitu.h
fill_platform.o: /net/eldora/eldora/rpc/include/Ray.h
fill_platform.o: /net/eldora/eldora/rpc/include/Platform.h
fill_platform.o: /net/eldora/eldora/rpc/include/FieldParam.h
fill_platform.o: /net/eldora/eldora/rpc/include/IndFreq.h
fill_platform.o: /net/eldora/eldora/rpc/include/TimeSeries.h
fill_platform.o: /net/eldora/eldora/rpc/include/NavInfo.h
fill_platform.o: /net/eldora/eldora/rpc/include/Ins.h
fill_platform.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
fill_platform.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
fill_platform.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
fill_platform.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h
fill_platform.o: /net/vx/h/rpc/clnt.h /net/vx/h/rpc/rpc_msg.h
fill_platform.o: /net/vx/h/rpc/auth_unix.h /net/vx/h/rpc/svc.h
fill_platform.o: /net/vx/h/rpc/svc_auth.h
fill_platform.o: /net/eldora/eldora/rpc/include/InSituData.h
fill_platform.o: /net/eldora/eldora/rpc/include/Header.h
fill_platform.o: /net/eldora/eldora/rpc/include/TapeHeader.h
fill_platform.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
fill_platform.o: /net/eldora/eldora/rpc/include/HskpCmd.h
fill_platform.o: /net/eldora/eldora/rpc/include/GblCommands.h
fill_platform.o: /net/eldora/eldora/rpc/include/HskpStatus.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
fill_platform.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/hskpAll.h /net/vx/h/vxWorks.h
midbeam.o: /net/vx/h/types/vxCpu.h /net/vx/h/types/vxArch.h
midbeam.o: /net/vx/h/types/vxParams.h /net/vx/h/types/vxTypesBase.h
midbeam.o: /net/vx/h/types/vxTypes.h /net/vx/h/types/vxANSI.h
midbeam.o: /net/vx/h/types/vxTypesOld.h /net/vx/h/sys/types.h
midbeam.o: /net/vx/h/math.h /net/vx/h/stdioLib.h /net/vx/h/stdio.h
midbeam.o: /net/vx/h/classLib.h /net/vx/h/vwModNum.h /net/vx/h/objLib.h
midbeam.o: /net/vx/h/errno.h /net/vx/h/memLib.h /net/vx/h/private/objLibP.h
midbeam.o: /net/vx/h/private/classLibP.h /net/vx/h/stdarg.h
midbeam.o: /net/vx/h/intLib.h /net/vx/h/semLib.h /net/vx/h/taskLib.h
midbeam.o: /net/vx/h/qLib.h /net/vx/h/qClass.h /net/vx/h/regs.h
midbeam.o: /net/vx/h/excLib.h /net/vx/h/private/eventP.h
midbeam.o: /net/vx/h/private/semLibP.h /net/vx/h/stdlib.h /net/vx/h/tyLib.h
midbeam.o: /net/vx/h/iosLib.h /net/vx/h/ioLib.h /net/vx/h/limits.h
midbeam.o: /net/vx/h/net/uio.h /net/vx/h/fcntl.h /net/vx/h/sys/fcntlcom.h
midbeam.o: /net/vx/h/unistd.h /net/vx/h/dllLib.h /net/vx/h/rngLib.h
midbeam.o: /net/vx/h/selectLib.h /net/vx/h/lstLib.h /net/vx/h/sys/times.h
midbeam.o: /net/vx/h/in.h /net/vx/h/netinet/in.h /net/vx/h/systime.h
midbeam.o: /net/vx/h/sysLib.h /net/vx/h/bootLib.h
midbeam.o: /net/eldora/eldora/rpc/include/Volume.h
midbeam.o: /net/eldora/eldora/rpc/include/Waveform.h
midbeam.o: /net/eldora/eldora/rpc/include/RadarDesc.h
midbeam.o: /net/eldora/eldora/rpc/include/FieldRadar.h
midbeam.o: /net/eldora/eldora/rpc/include/CellSpacing.h
midbeam.o: /net/eldora/eldora/rpc/include/Parameter.h
midbeam.o: /net/eldora/eldora/rpc/include/NavDesc.h
midbeam.o: /net/eldora/eldora/rpc/include/InSitu.h
midbeam.o: /net/eldora/eldora/rpc/include/Ray.h
midbeam.o: /net/eldora/eldora/rpc/include/Platform.h
midbeam.o: /net/eldora/eldora/rpc/include/FieldParam.h
midbeam.o: /net/eldora/eldora/rpc/include/IndFreq.h
midbeam.o: /net/eldora/eldora/rpc/include/TimeSeries.h
midbeam.o: /net/eldora/eldora/rpc/include/NavInfo.h
midbeam.o: /net/eldora/eldora/rpc/include/Ins.h
midbeam.o: /net/eldora/eldora/rpc/include/MiniRIMS.h
midbeam.o: /net/eldora/eldora/rpc/include/Gps.h /net/vx/h/rpc/rpc.h
midbeam.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/xdr.h
midbeam.o: /net/vx/h/rpc/rpctypes.h /net/vx/h/rpc/auth.h /net/vx/h/rpc/clnt.h
midbeam.o: /net/vx/h/rpc/rpc_msg.h /net/vx/h/rpc/auth_unix.h
midbeam.o: /net/vx/h/rpc/svc.h /net/vx/h/rpc/svc_auth.h
midbeam.o: /net/eldora/eldora/rpc/include/InSituData.h
midbeam.o: /net/eldora/eldora/rpc/include/Header.h
midbeam.o: /net/eldora/eldora/rpc/include/TapeHeader.h
midbeam.o: /net/eldora/eldora/rpc/include/HeaderRpc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/cntrlDef.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/cntrlFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/cntrlGbl.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/ecbAdr.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/ecbErrBound.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/ecbFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/ecbMaster.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/ecbSem.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/ecbStat.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/gpsDef.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/gpsFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/gpsGbl.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/hskpDef.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/hskpFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/hskpGbl.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/hskpInt.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/iruDef.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/iruFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/iruGbl.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/pwrDef.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/pwrFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/tp41vAdr.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/vme_hndshk.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/vmevmeAdr.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/vmevmeDef.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/vmevmeFunc.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/vmevmeGbl.h
midbeam.o: /net/eldora/eldora/rpc/include/HskpCmd.h
midbeam.o: /net/eldora/eldora/rpc/include/GblCommands.h
midbeam.o: /net/eldora/eldora/rpc/include/HskpStatus.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/clockDef.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/clockGbl.h
midbeam.o: /net/eldora/eldora/vx/hskp/include/clockFunc.h
