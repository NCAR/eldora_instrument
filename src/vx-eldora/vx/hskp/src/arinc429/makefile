#
#	Makefile for Directory ELDORA/vx/hskp/src/arinc429
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
		-I/vx/config/tp41
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS= init_iru.o convert_iru.o start_iru.o iru_menu.o
#
SRCS= init_iru.c convert_iru.c start_iru.c iru_menu.c
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

init_iru.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
init_iru.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
init_iru.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
init_iru.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
init_iru.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
init_iru.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_iru.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
init_iru.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
init_iru.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
init_iru.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
init_iru.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
init_iru.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
init_iru.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
init_iru.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
init_iru.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
init_iru.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
init_iru.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
init_iru.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
init_iru.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
init_iru.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
init_iru.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
init_iru.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
init_iru.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
init_iru.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
init_iru.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
init_iru.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
init_iru.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
init_iru.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
init_iru.o: /ELDORA/vx/hskp/include/cntrlDef.h
init_iru.o: /ELDORA/vx/hskp/include/cntrlFunc.h
init_iru.o: /ELDORA/vx/hskp/include/cntrlGbl.h
init_iru.o: /ELDORA/vx/hskp/include/ecbAdr.h
init_iru.o: /ELDORA/vx/hskp/include/ecbErrBound.h
init_iru.o: /ELDORA/vx/hskp/include/ecbFunc.h
init_iru.o: /ELDORA/vx/hskp/include/ecbMaster.h
init_iru.o: /ELDORA/vx/hskp/include/ecbSem.h
init_iru.o: /ELDORA/vx/hskp/include/ecbStat.h
init_iru.o: /ELDORA/vx/hskp/include/gpsDef.h
init_iru.o: /ELDORA/vx/hskp/include/gpsFunc.h
init_iru.o: /ELDORA/vx/hskp/include/gpsGbl.h
init_iru.o: /ELDORA/vx/hskp/include/hskpDef.h
init_iru.o: /ELDORA/vx/hskp/include/hskpFunc.h
init_iru.o: /ELDORA/vx/hskp/include/hskpGbl.h
init_iru.o: /ELDORA/vx/hskp/include/hskpInt.h
init_iru.o: /ELDORA/vx/hskp/include/iruDef.h
init_iru.o: /ELDORA/vx/hskp/include/iruFunc.h
init_iru.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
init_iru.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_iru.o: /ELDORA/vx/hskp/include/todDef.h
init_iru.o: /ELDORA/vx/hskp/include/todFunc.h
init_iru.o: /ELDORA/vx/hskp/include/todGbl.h
init_iru.o: /ELDORA/vx/hskp/include/tp41vAdr.h
init_iru.o: /ELDORA/vx/hskp/include/vme_hndshk.h
init_iru.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
init_iru.o: /ELDORA/vx/hskp/include/vmevmeDef.h
init_iru.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
init_iru.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
init_iru.o: /ELDORA/rpc/include/GblCommands.h
init_iru.o: /ELDORA/rpc/include/HskpStatus.h
convert_iru.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
convert_iru.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
convert_iru.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
convert_iru.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
convert_iru.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
convert_iru.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
convert_iru.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
convert_iru.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
convert_iru.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
convert_iru.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
convert_iru.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
convert_iru.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
convert_iru.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
convert_iru.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
convert_iru.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
convert_iru.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
convert_iru.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
convert_iru.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
convert_iru.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
convert_iru.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
convert_iru.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
convert_iru.o: /ELDORA/rpc/header/FieldRadar.h
convert_iru.o: /ELDORA/rpc/header/CellSpacing.h
convert_iru.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
convert_iru.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
convert_iru.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
convert_iru.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
convert_iru.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
convert_iru.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
convert_iru.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
convert_iru.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
convert_iru.o: /ELDORA/vx/hskp/include/cntrlDef.h
convert_iru.o: /ELDORA/vx/hskp/include/cntrlFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/cntrlGbl.h
convert_iru.o: /ELDORA/vx/hskp/include/ecbAdr.h
convert_iru.o: /ELDORA/vx/hskp/include/ecbErrBound.h
convert_iru.o: /ELDORA/vx/hskp/include/ecbFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/ecbMaster.h
convert_iru.o: /ELDORA/vx/hskp/include/ecbSem.h
convert_iru.o: /ELDORA/vx/hskp/include/ecbStat.h
convert_iru.o: /ELDORA/vx/hskp/include/gpsDef.h
convert_iru.o: /ELDORA/vx/hskp/include/gpsFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/gpsGbl.h
convert_iru.o: /ELDORA/vx/hskp/include/hskpDef.h
convert_iru.o: /ELDORA/vx/hskp/include/hskpFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/hskpGbl.h
convert_iru.o: /ELDORA/vx/hskp/include/hskpInt.h
convert_iru.o: /ELDORA/vx/hskp/include/iruDef.h
convert_iru.o: /ELDORA/vx/hskp/include/iruFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/iruGbl.h
convert_iru.o: /ELDORA/vx/hskp/include/pwrDef.h
convert_iru.o: /ELDORA/vx/hskp/include/pwrFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/todDef.h
convert_iru.o: /ELDORA/vx/hskp/include/todFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/todGbl.h
convert_iru.o: /ELDORA/vx/hskp/include/tp41vAdr.h
convert_iru.o: /ELDORA/vx/hskp/include/vme_hndshk.h
convert_iru.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
convert_iru.o: /ELDORA/vx/hskp/include/vmevmeDef.h
convert_iru.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
convert_iru.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
convert_iru.o: /ELDORA/rpc/include/HskpCmd.h
convert_iru.o: /ELDORA/rpc/include/GblCommands.h
convert_iru.o: /ELDORA/rpc/include/HskpStatus.h
start_iru.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
start_iru.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
start_iru.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
start_iru.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
start_iru.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
start_iru.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
start_iru.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
start_iru.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
start_iru.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
start_iru.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
start_iru.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
start_iru.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
start_iru.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
start_iru.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
start_iru.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
start_iru.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
start_iru.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
start_iru.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
start_iru.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
start_iru.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
start_iru.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
start_iru.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
start_iru.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
start_iru.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
start_iru.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
start_iru.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
start_iru.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
start_iru.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
start_iru.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
start_iru.o: /ELDORA/vx/hskp/include/cntrlDef.h
start_iru.o: /ELDORA/vx/hskp/include/cntrlFunc.h
start_iru.o: /ELDORA/vx/hskp/include/cntrlGbl.h
start_iru.o: /ELDORA/vx/hskp/include/ecbAdr.h
start_iru.o: /ELDORA/vx/hskp/include/ecbErrBound.h
start_iru.o: /ELDORA/vx/hskp/include/ecbFunc.h
start_iru.o: /ELDORA/vx/hskp/include/ecbMaster.h
start_iru.o: /ELDORA/vx/hskp/include/ecbSem.h
start_iru.o: /ELDORA/vx/hskp/include/ecbStat.h
start_iru.o: /ELDORA/vx/hskp/include/gpsDef.h
start_iru.o: /ELDORA/vx/hskp/include/gpsFunc.h
start_iru.o: /ELDORA/vx/hskp/include/gpsGbl.h
start_iru.o: /ELDORA/vx/hskp/include/hskpDef.h
start_iru.o: /ELDORA/vx/hskp/include/hskpFunc.h
start_iru.o: /ELDORA/vx/hskp/include/hskpGbl.h
start_iru.o: /ELDORA/vx/hskp/include/hskpInt.h
start_iru.o: /ELDORA/vx/hskp/include/iruDef.h
start_iru.o: /ELDORA/vx/hskp/include/iruFunc.h
start_iru.o: /ELDORA/vx/hskp/include/iruGbl.h
start_iru.o: /ELDORA/vx/hskp/include/pwrDef.h
start_iru.o: /ELDORA/vx/hskp/include/pwrFunc.h
start_iru.o: /ELDORA/vx/hskp/include/todDef.h
start_iru.o: /ELDORA/vx/hskp/include/todFunc.h
start_iru.o: /ELDORA/vx/hskp/include/todGbl.h
start_iru.o: /ELDORA/vx/hskp/include/tp41vAdr.h
start_iru.o: /ELDORA/vx/hskp/include/vme_hndshk.h
start_iru.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
start_iru.o: /ELDORA/vx/hskp/include/vmevmeDef.h
start_iru.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
start_iru.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
start_iru.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
start_iru.o: /ELDORA/rpc/include/HskpStatus.h
iru_menu.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
iru_menu.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
iru_menu.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
iru_menu.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
iru_menu.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
iru_menu.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
iru_menu.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
iru_menu.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
iru_menu.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
iru_menu.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
iru_menu.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
iru_menu.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
iru_menu.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
iru_menu.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
iru_menu.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
iru_menu.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
iru_menu.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
iru_menu.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
iru_menu.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
iru_menu.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
iru_menu.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
iru_menu.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
iru_menu.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
iru_menu.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
iru_menu.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
iru_menu.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
iru_menu.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
iru_menu.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
iru_menu.o: /ELDORA/vx/hskp/include/cntrlDef.h
iru_menu.o: /ELDORA/vx/hskp/include/cntrlFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/cntrlGbl.h
iru_menu.o: /ELDORA/vx/hskp/include/ecbAdr.h
iru_menu.o: /ELDORA/vx/hskp/include/ecbErrBound.h
iru_menu.o: /ELDORA/vx/hskp/include/ecbFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/ecbMaster.h
iru_menu.o: /ELDORA/vx/hskp/include/ecbSem.h
iru_menu.o: /ELDORA/vx/hskp/include/ecbStat.h
iru_menu.o: /ELDORA/vx/hskp/include/gpsDef.h
iru_menu.o: /ELDORA/vx/hskp/include/gpsFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/gpsGbl.h
iru_menu.o: /ELDORA/vx/hskp/include/hskpDef.h
iru_menu.o: /ELDORA/vx/hskp/include/hskpFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/hskpGbl.h
iru_menu.o: /ELDORA/vx/hskp/include/hskpInt.h
iru_menu.o: /ELDORA/vx/hskp/include/iruDef.h
iru_menu.o: /ELDORA/vx/hskp/include/iruFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/pwrDef.h
iru_menu.o: /ELDORA/vx/hskp/include/pwrFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/todDef.h
iru_menu.o: /ELDORA/vx/hskp/include/todFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/todGbl.h
iru_menu.o: /ELDORA/vx/hskp/include/tp41vAdr.h
iru_menu.o: /ELDORA/vx/hskp/include/vme_hndshk.h
iru_menu.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
iru_menu.o: /ELDORA/vx/hskp/include/vmevmeDef.h
iru_menu.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
iru_menu.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
iru_menu.o: /ELDORA/rpc/include/GblCommands.h
iru_menu.o: /ELDORA/rpc/include/HskpStatus.h
