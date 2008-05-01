#
#	Makefile for Directory ELDORA/vx/hskp/src/clock
#	Author: Craig Walther
#	Date: 8/12/92
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
		-I/vx/config/tp41
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS= init_clock.o get_time.o read_data.o set_time.o start_clock.o\
	sync_irig.o tod_menu.o write_data.o view_time.o
#
SRCS= init_clock.c get_time.c read_data.c set_time.c start_clock.c\
	sync_irig.c tod_menu.c write_data.c view_time.c
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

clean:
	rm -f *.o *~ core

copyem:
	cp *.o /ELDORA/vx/hskp/bin/.

real_clean: clean
	rm -f *.c *.h


# DO NOT DELETE

init_clock.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
init_clock.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
init_clock.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
init_clock.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
init_clock.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
init_clock.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_clock.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
init_clock.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
init_clock.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
init_clock.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
init_clock.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
init_clock.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
init_clock.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
init_clock.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
init_clock.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
init_clock.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
init_clock.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
init_clock.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
init_clock.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
init_clock.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
init_clock.o: /ELDORA/rpc/header/FieldRadar.h
init_clock.o: /ELDORA/rpc/header/CellSpacing.h /ELDORA/rpc/header/Parameter.h
init_clock.o: /ELDORA/rpc/header/NavDesc.h /ELDORA/rpc/header/InSitu.h
init_clock.o: /ELDORA/rpc/header/Ray.h /ELDORA/rpc/header/Platform.h
init_clock.o: /ELDORA/rpc/header/FieldParam.h /ELDORA/rpc/header/IndFreq.h
init_clock.o: /ELDORA/rpc/header/TimeSeries.h /ELDORA/rpc/header/NavInfo.h
init_clock.o: /ELDORA/rpc/header/Ins.h /ELDORA/rpc/header/MiniRIMS.h
init_clock.o: /ELDORA/rpc/header/Gps.h /ELDORA/rpc/header/InSituData.h
init_clock.o: /ELDORA/rpc/header/Header.h /ELDORA/rpc/header/TapeHeader.h
init_clock.o: /ELDORA/rpc/header/HeaderRpc.h
init_clock.o: /ELDORA/vx/hskp/include/cntrlDef.h
init_clock.o: /ELDORA/vx/hskp/include/cntrlFunc.h
init_clock.o: /ELDORA/vx/hskp/include/cntrlGbl.h
init_clock.o: /ELDORA/vx/hskp/include/ecbAdr.h
init_clock.o: /ELDORA/vx/hskp/include/ecbErrBound.h
init_clock.o: /ELDORA/vx/hskp/include/ecbFunc.h
init_clock.o: /ELDORA/vx/hskp/include/ecbMaster.h
init_clock.o: /ELDORA/vx/hskp/include/ecbSem.h
init_clock.o: /ELDORA/vx/hskp/include/ecbStat.h
init_clock.o: /ELDORA/vx/hskp/include/gpsDef.h
init_clock.o: /ELDORA/vx/hskp/include/gpsFunc.h
init_clock.o: /ELDORA/vx/hskp/include/gpsGbl.h
init_clock.o: /ELDORA/vx/hskp/include/hskpDef.h
init_clock.o: /ELDORA/vx/hskp/include/hskpFunc.h
init_clock.o: /ELDORA/vx/hskp/include/hskpGbl.h
init_clock.o: /ELDORA/vx/hskp/include/hskpInt.h
init_clock.o: /ELDORA/vx/hskp/include/iruDef.h
init_clock.o: /ELDORA/vx/hskp/include/iruFunc.h
init_clock.o: /ELDORA/vx/hskp/include/iruGbl.h
init_clock.o: /ELDORA/vx/hskp/include/pwrDef.h
init_clock.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_clock.o: /ELDORA/vx/hskp/include/todDef.h
init_clock.o: /ELDORA/vx/hskp/include/todFunc.h
init_clock.o: /ELDORA/vx/hskp/include/todGbl.h
init_clock.o: /ELDORA/vx/hskp/include/tp41vAdr.h
init_clock.o: /ELDORA/vx/hskp/include/vme_hndshk.h
init_clock.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
init_clock.o: /ELDORA/vx/hskp/include/vmevmeDef.h
init_clock.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
init_clock.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
init_clock.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
init_clock.o: /ELDORA/rpc/include/HskpStatus.h
get_time.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
get_time.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
get_time.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
get_time.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
get_time.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
get_time.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
get_time.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
get_time.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
get_time.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
get_time.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
get_time.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
get_time.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
get_time.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
get_time.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
get_time.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
get_time.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
get_time.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
get_time.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
get_time.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
get_time.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
get_time.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
get_time.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
get_time.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
get_time.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
get_time.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
get_time.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
get_time.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
get_time.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
get_time.o: /ELDORA/vx/hskp/include/cntrlDef.h
get_time.o: /ELDORA/vx/hskp/include/cntrlFunc.h
get_time.o: /ELDORA/vx/hskp/include/cntrlGbl.h
get_time.o: /ELDORA/vx/hskp/include/ecbAdr.h
get_time.o: /ELDORA/vx/hskp/include/ecbErrBound.h
get_time.o: /ELDORA/vx/hskp/include/ecbFunc.h
get_time.o: /ELDORA/vx/hskp/include/ecbMaster.h
get_time.o: /ELDORA/vx/hskp/include/ecbSem.h
get_time.o: /ELDORA/vx/hskp/include/ecbStat.h
get_time.o: /ELDORA/vx/hskp/include/gpsDef.h
get_time.o: /ELDORA/vx/hskp/include/gpsFunc.h
get_time.o: /ELDORA/vx/hskp/include/gpsGbl.h
get_time.o: /ELDORA/vx/hskp/include/hskpDef.h
get_time.o: /ELDORA/vx/hskp/include/hskpFunc.h
get_time.o: /ELDORA/vx/hskp/include/hskpGbl.h
get_time.o: /ELDORA/vx/hskp/include/hskpInt.h
get_time.o: /ELDORA/vx/hskp/include/iruDef.h
get_time.o: /ELDORA/vx/hskp/include/iruFunc.h
get_time.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
get_time.o: /ELDORA/vx/hskp/include/pwrFunc.h
get_time.o: /ELDORA/vx/hskp/include/todDef.h
get_time.o: /ELDORA/vx/hskp/include/todFunc.h
get_time.o: /ELDORA/vx/hskp/include/todGbl.h
get_time.o: /ELDORA/vx/hskp/include/tp41vAdr.h
get_time.o: /ELDORA/vx/hskp/include/vme_hndshk.h
get_time.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
get_time.o: /ELDORA/vx/hskp/include/vmevmeDef.h
get_time.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
get_time.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
get_time.o: /ELDORA/rpc/include/GblCommands.h
get_time.o: /ELDORA/rpc/include/HskpStatus.h
read_data.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
read_data.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
read_data.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
read_data.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
read_data.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
read_data.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
read_data.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
read_data.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
read_data.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
read_data.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
read_data.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
read_data.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
read_data.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
read_data.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
read_data.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
read_data.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
read_data.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
read_data.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
read_data.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
read_data.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
read_data.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
read_data.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
read_data.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
read_data.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
read_data.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
read_data.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
read_data.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
read_data.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
read_data.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
read_data.o: /ELDORA/vx/hskp/include/cntrlDef.h
read_data.o: /ELDORA/vx/hskp/include/cntrlFunc.h
read_data.o: /ELDORA/vx/hskp/include/cntrlGbl.h
read_data.o: /ELDORA/vx/hskp/include/ecbAdr.h
read_data.o: /ELDORA/vx/hskp/include/ecbErrBound.h
read_data.o: /ELDORA/vx/hskp/include/ecbFunc.h
read_data.o: /ELDORA/vx/hskp/include/ecbMaster.h
read_data.o: /ELDORA/vx/hskp/include/ecbSem.h
read_data.o: /ELDORA/vx/hskp/include/ecbStat.h
read_data.o: /ELDORA/vx/hskp/include/gpsDef.h
read_data.o: /ELDORA/vx/hskp/include/gpsFunc.h
read_data.o: /ELDORA/vx/hskp/include/gpsGbl.h
read_data.o: /ELDORA/vx/hskp/include/hskpDef.h
read_data.o: /ELDORA/vx/hskp/include/hskpFunc.h
read_data.o: /ELDORA/vx/hskp/include/hskpGbl.h
read_data.o: /ELDORA/vx/hskp/include/hskpInt.h
read_data.o: /ELDORA/vx/hskp/include/iruDef.h
read_data.o: /ELDORA/vx/hskp/include/iruFunc.h
read_data.o: /ELDORA/vx/hskp/include/iruGbl.h
read_data.o: /ELDORA/vx/hskp/include/pwrDef.h
read_data.o: /ELDORA/vx/hskp/include/pwrFunc.h
read_data.o: /ELDORA/vx/hskp/include/todDef.h
read_data.o: /ELDORA/vx/hskp/include/todFunc.h
read_data.o: /ELDORA/vx/hskp/include/todGbl.h
read_data.o: /ELDORA/vx/hskp/include/tp41vAdr.h
read_data.o: /ELDORA/vx/hskp/include/vme_hndshk.h
read_data.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
read_data.o: /ELDORA/vx/hskp/include/vmevmeDef.h
read_data.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
read_data.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
read_data.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
read_data.o: /ELDORA/rpc/include/HskpStatus.h
set_time.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
set_time.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
set_time.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
set_time.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
set_time.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
set_time.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
set_time.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
set_time.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
set_time.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
set_time.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
set_time.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
set_time.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
set_time.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
set_time.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
set_time.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
set_time.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
set_time.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
set_time.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
set_time.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
set_time.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
set_time.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
set_time.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
set_time.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
set_time.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
set_time.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
set_time.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
set_time.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
set_time.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
set_time.o: /ELDORA/vx/hskp/include/cntrlDef.h
set_time.o: /ELDORA/vx/hskp/include/cntrlFunc.h
set_time.o: /ELDORA/vx/hskp/include/cntrlGbl.h
set_time.o: /ELDORA/vx/hskp/include/ecbAdr.h
set_time.o: /ELDORA/vx/hskp/include/ecbErrBound.h
set_time.o: /ELDORA/vx/hskp/include/ecbFunc.h
set_time.o: /ELDORA/vx/hskp/include/ecbMaster.h
set_time.o: /ELDORA/vx/hskp/include/ecbSem.h
set_time.o: /ELDORA/vx/hskp/include/ecbStat.h
set_time.o: /ELDORA/vx/hskp/include/gpsDef.h
set_time.o: /ELDORA/vx/hskp/include/gpsFunc.h
set_time.o: /ELDORA/vx/hskp/include/gpsGbl.h
set_time.o: /ELDORA/vx/hskp/include/hskpDef.h
set_time.o: /ELDORA/vx/hskp/include/hskpFunc.h
set_time.o: /ELDORA/vx/hskp/include/hskpGbl.h
set_time.o: /ELDORA/vx/hskp/include/hskpInt.h
set_time.o: /ELDORA/vx/hskp/include/iruDef.h
set_time.o: /ELDORA/vx/hskp/include/iruFunc.h
set_time.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
set_time.o: /ELDORA/vx/hskp/include/pwrFunc.h
set_time.o: /ELDORA/vx/hskp/include/todDef.h
set_time.o: /ELDORA/vx/hskp/include/todFunc.h
set_time.o: /ELDORA/vx/hskp/include/todGbl.h
set_time.o: /ELDORA/vx/hskp/include/tp41vAdr.h
set_time.o: /ELDORA/vx/hskp/include/vme_hndshk.h
set_time.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
set_time.o: /ELDORA/vx/hskp/include/vmevmeDef.h
set_time.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
set_time.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
set_time.o: /ELDORA/rpc/include/GblCommands.h
set_time.o: /ELDORA/rpc/include/HskpStatus.h
start_clock.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
start_clock.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
start_clock.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
start_clock.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
start_clock.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
start_clock.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
start_clock.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
start_clock.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
start_clock.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
start_clock.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
start_clock.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
start_clock.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
start_clock.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
start_clock.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
start_clock.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
start_clock.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
start_clock.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
start_clock.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
start_clock.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
start_clock.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
start_clock.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
start_clock.o: /ELDORA/rpc/header/FieldRadar.h
start_clock.o: /ELDORA/rpc/header/CellSpacing.h
start_clock.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
start_clock.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
start_clock.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
start_clock.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
start_clock.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
start_clock.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
start_clock.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
start_clock.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
start_clock.o: /ELDORA/vx/hskp/include/cntrlDef.h
start_clock.o: /ELDORA/vx/hskp/include/cntrlFunc.h
start_clock.o: /ELDORA/vx/hskp/include/cntrlGbl.h
start_clock.o: /ELDORA/vx/hskp/include/ecbAdr.h
start_clock.o: /ELDORA/vx/hskp/include/ecbErrBound.h
start_clock.o: /ELDORA/vx/hskp/include/ecbFunc.h
start_clock.o: /ELDORA/vx/hskp/include/ecbMaster.h
start_clock.o: /ELDORA/vx/hskp/include/ecbSem.h
start_clock.o: /ELDORA/vx/hskp/include/ecbStat.h
start_clock.o: /ELDORA/vx/hskp/include/gpsDef.h
start_clock.o: /ELDORA/vx/hskp/include/gpsFunc.h
start_clock.o: /ELDORA/vx/hskp/include/gpsGbl.h
start_clock.o: /ELDORA/vx/hskp/include/hskpDef.h
start_clock.o: /ELDORA/vx/hskp/include/hskpFunc.h
start_clock.o: /ELDORA/vx/hskp/include/hskpGbl.h
start_clock.o: /ELDORA/vx/hskp/include/hskpInt.h
start_clock.o: /ELDORA/vx/hskp/include/iruDef.h
start_clock.o: /ELDORA/vx/hskp/include/iruFunc.h
start_clock.o: /ELDORA/vx/hskp/include/iruGbl.h
start_clock.o: /ELDORA/vx/hskp/include/pwrDef.h
start_clock.o: /ELDORA/vx/hskp/include/pwrFunc.h
start_clock.o: /ELDORA/vx/hskp/include/todDef.h
start_clock.o: /ELDORA/vx/hskp/include/todFunc.h
start_clock.o: /ELDORA/vx/hskp/include/todGbl.h
start_clock.o: /ELDORA/vx/hskp/include/tp41vAdr.h
start_clock.o: /ELDORA/vx/hskp/include/vme_hndshk.h
start_clock.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
start_clock.o: /ELDORA/vx/hskp/include/vmevmeDef.h
start_clock.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
start_clock.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
start_clock.o: /ELDORA/rpc/include/HskpCmd.h
start_clock.o: /ELDORA/rpc/include/GblCommands.h
start_clock.o: /ELDORA/rpc/include/HskpStatus.h
sync_irig.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
sync_irig.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
sync_irig.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
sync_irig.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
sync_irig.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
sync_irig.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
sync_irig.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
sync_irig.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
sync_irig.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
sync_irig.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
sync_irig.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
sync_irig.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
sync_irig.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
sync_irig.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
sync_irig.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
sync_irig.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
sync_irig.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
sync_irig.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
sync_irig.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
sync_irig.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
sync_irig.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
sync_irig.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
sync_irig.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
sync_irig.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
sync_irig.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
sync_irig.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
sync_irig.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
sync_irig.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
sync_irig.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
sync_irig.o: /ELDORA/vx/hskp/include/cntrlDef.h
sync_irig.o: /ELDORA/vx/hskp/include/cntrlFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/cntrlGbl.h
sync_irig.o: /ELDORA/vx/hskp/include/ecbAdr.h
sync_irig.o: /ELDORA/vx/hskp/include/ecbErrBound.h
sync_irig.o: /ELDORA/vx/hskp/include/ecbFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/ecbMaster.h
sync_irig.o: /ELDORA/vx/hskp/include/ecbSem.h
sync_irig.o: /ELDORA/vx/hskp/include/ecbStat.h
sync_irig.o: /ELDORA/vx/hskp/include/gpsDef.h
sync_irig.o: /ELDORA/vx/hskp/include/gpsFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/gpsGbl.h
sync_irig.o: /ELDORA/vx/hskp/include/hskpDef.h
sync_irig.o: /ELDORA/vx/hskp/include/hskpFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/hskpGbl.h
sync_irig.o: /ELDORA/vx/hskp/include/hskpInt.h
sync_irig.o: /ELDORA/vx/hskp/include/iruDef.h
sync_irig.o: /ELDORA/vx/hskp/include/iruFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/iruGbl.h
sync_irig.o: /ELDORA/vx/hskp/include/pwrDef.h
sync_irig.o: /ELDORA/vx/hskp/include/pwrFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/todDef.h
sync_irig.o: /ELDORA/vx/hskp/include/todFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/todGbl.h
sync_irig.o: /ELDORA/vx/hskp/include/tp41vAdr.h
sync_irig.o: /ELDORA/vx/hskp/include/vme_hndshk.h
sync_irig.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
sync_irig.o: /ELDORA/vx/hskp/include/vmevmeDef.h
sync_irig.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
sync_irig.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
sync_irig.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
sync_irig.o: /ELDORA/rpc/include/HskpStatus.h
tod_menu.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
tod_menu.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
tod_menu.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
tod_menu.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
tod_menu.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
tod_menu.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
tod_menu.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
tod_menu.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
tod_menu.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
tod_menu.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
tod_menu.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
tod_menu.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
tod_menu.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
tod_menu.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
tod_menu.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
tod_menu.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
tod_menu.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
tod_menu.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
tod_menu.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
tod_menu.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
tod_menu.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
tod_menu.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
tod_menu.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
tod_menu.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
tod_menu.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
tod_menu.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
tod_menu.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
tod_menu.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
tod_menu.o: /ELDORA/vx/hskp/include/cntrlDef.h
tod_menu.o: /ELDORA/vx/hskp/include/cntrlFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/cntrlGbl.h
tod_menu.o: /ELDORA/vx/hskp/include/ecbAdr.h
tod_menu.o: /ELDORA/vx/hskp/include/ecbErrBound.h
tod_menu.o: /ELDORA/vx/hskp/include/ecbFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/ecbMaster.h
tod_menu.o: /ELDORA/vx/hskp/include/ecbSem.h
tod_menu.o: /ELDORA/vx/hskp/include/ecbStat.h
tod_menu.o: /ELDORA/vx/hskp/include/gpsDef.h
tod_menu.o: /ELDORA/vx/hskp/include/gpsFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/gpsGbl.h
tod_menu.o: /ELDORA/vx/hskp/include/hskpDef.h
tod_menu.o: /ELDORA/vx/hskp/include/hskpFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/hskpGbl.h
tod_menu.o: /ELDORA/vx/hskp/include/hskpInt.h
tod_menu.o: /ELDORA/vx/hskp/include/iruDef.h
tod_menu.o: /ELDORA/vx/hskp/include/iruFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
tod_menu.o: /ELDORA/vx/hskp/include/pwrFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/todDef.h
tod_menu.o: /ELDORA/vx/hskp/include/todFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/todGbl.h
tod_menu.o: /ELDORA/vx/hskp/include/tp41vAdr.h
tod_menu.o: /ELDORA/vx/hskp/include/vme_hndshk.h
tod_menu.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
tod_menu.o: /ELDORA/vx/hskp/include/vmevmeDef.h
tod_menu.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
tod_menu.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
tod_menu.o: /ELDORA/rpc/include/GblCommands.h
tod_menu.o: /ELDORA/rpc/include/HskpStatus.h
write_data.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
write_data.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
write_data.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
write_data.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
write_data.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
write_data.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
write_data.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
write_data.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
write_data.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
write_data.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
write_data.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
write_data.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
write_data.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
write_data.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
write_data.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
write_data.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
write_data.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
write_data.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
write_data.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
write_data.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
write_data.o: /ELDORA/rpc/header/FieldRadar.h
write_data.o: /ELDORA/rpc/header/CellSpacing.h /ELDORA/rpc/header/Parameter.h
write_data.o: /ELDORA/rpc/header/NavDesc.h /ELDORA/rpc/header/InSitu.h
write_data.o: /ELDORA/rpc/header/Ray.h /ELDORA/rpc/header/Platform.h
write_data.o: /ELDORA/rpc/header/FieldParam.h /ELDORA/rpc/header/IndFreq.h
write_data.o: /ELDORA/rpc/header/TimeSeries.h /ELDORA/rpc/header/NavInfo.h
write_data.o: /ELDORA/rpc/header/Ins.h /ELDORA/rpc/header/MiniRIMS.h
write_data.o: /ELDORA/rpc/header/Gps.h /ELDORA/rpc/header/InSituData.h
write_data.o: /ELDORA/rpc/header/Header.h /ELDORA/rpc/header/TapeHeader.h
write_data.o: /ELDORA/rpc/header/HeaderRpc.h
write_data.o: /ELDORA/vx/hskp/include/cntrlDef.h
write_data.o: /ELDORA/vx/hskp/include/cntrlFunc.h
write_data.o: /ELDORA/vx/hskp/include/cntrlGbl.h
write_data.o: /ELDORA/vx/hskp/include/ecbAdr.h
write_data.o: /ELDORA/vx/hskp/include/ecbErrBound.h
write_data.o: /ELDORA/vx/hskp/include/ecbFunc.h
write_data.o: /ELDORA/vx/hskp/include/ecbMaster.h
write_data.o: /ELDORA/vx/hskp/include/ecbSem.h
write_data.o: /ELDORA/vx/hskp/include/ecbStat.h
write_data.o: /ELDORA/vx/hskp/include/gpsDef.h
write_data.o: /ELDORA/vx/hskp/include/gpsFunc.h
write_data.o: /ELDORA/vx/hskp/include/gpsGbl.h
write_data.o: /ELDORA/vx/hskp/include/hskpDef.h
write_data.o: /ELDORA/vx/hskp/include/hskpFunc.h
write_data.o: /ELDORA/vx/hskp/include/hskpGbl.h
write_data.o: /ELDORA/vx/hskp/include/hskpInt.h
write_data.o: /ELDORA/vx/hskp/include/iruDef.h
write_data.o: /ELDORA/vx/hskp/include/iruFunc.h
write_data.o: /ELDORA/vx/hskp/include/iruGbl.h
write_data.o: /ELDORA/vx/hskp/include/pwrDef.h
write_data.o: /ELDORA/vx/hskp/include/pwrFunc.h
write_data.o: /ELDORA/vx/hskp/include/todDef.h
write_data.o: /ELDORA/vx/hskp/include/todFunc.h
write_data.o: /ELDORA/vx/hskp/include/todGbl.h
write_data.o: /ELDORA/vx/hskp/include/tp41vAdr.h
write_data.o: /ELDORA/vx/hskp/include/vme_hndshk.h
write_data.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
write_data.o: /ELDORA/vx/hskp/include/vmevmeDef.h
write_data.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
write_data.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
write_data.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
write_data.o: /ELDORA/rpc/include/HskpStatus.h
view_time.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
view_time.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
view_time.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
view_time.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
view_time.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
view_time.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
view_time.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
view_time.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
view_time.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
view_time.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
view_time.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
view_time.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
view_time.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
view_time.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
view_time.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
view_time.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
view_time.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
view_time.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
view_time.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
view_time.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
view_time.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
view_time.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
view_time.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
view_time.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
view_time.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
view_time.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
view_time.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
view_time.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
view_time.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
view_time.o: /ELDORA/vx/hskp/include/cntrlDef.h
view_time.o: /ELDORA/vx/hskp/include/cntrlFunc.h
view_time.o: /ELDORA/vx/hskp/include/cntrlGbl.h
view_time.o: /ELDORA/vx/hskp/include/ecbAdr.h
view_time.o: /ELDORA/vx/hskp/include/ecbErrBound.h
view_time.o: /ELDORA/vx/hskp/include/ecbFunc.h
view_time.o: /ELDORA/vx/hskp/include/ecbMaster.h
view_time.o: /ELDORA/vx/hskp/include/ecbSem.h
view_time.o: /ELDORA/vx/hskp/include/ecbStat.h
view_time.o: /ELDORA/vx/hskp/include/gpsDef.h
view_time.o: /ELDORA/vx/hskp/include/gpsFunc.h
view_time.o: /ELDORA/vx/hskp/include/gpsGbl.h
view_time.o: /ELDORA/vx/hskp/include/hskpDef.h
view_time.o: /ELDORA/vx/hskp/include/hskpFunc.h
view_time.o: /ELDORA/vx/hskp/include/hskpGbl.h
view_time.o: /ELDORA/vx/hskp/include/hskpInt.h
view_time.o: /ELDORA/vx/hskp/include/iruDef.h
view_time.o: /ELDORA/vx/hskp/include/iruFunc.h
view_time.o: /ELDORA/vx/hskp/include/iruGbl.h
view_time.o: /ELDORA/vx/hskp/include/pwrDef.h
view_time.o: /ELDORA/vx/hskp/include/pwrFunc.h
view_time.o: /ELDORA/vx/hskp/include/todDef.h
view_time.o: /ELDORA/vx/hskp/include/todFunc.h
view_time.o: /ELDORA/vx/hskp/include/todGbl.h
view_time.o: /ELDORA/vx/hskp/include/tp41vAdr.h
view_time.o: /ELDORA/vx/hskp/include/vme_hndshk.h
view_time.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
view_time.o: /ELDORA/vx/hskp/include/vmevmeDef.h
view_time.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
view_time.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
view_time.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
view_time.o: /ELDORA/rpc/include/HskpStatus.h
