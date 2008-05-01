#
#	Makefile for Directory ELDORA/vx/hskp/src/main
#	Author: Craig Walther
#	Date: 8/19/92
#
# GNU C 
CC= gcc68k
CCC = $(CC)
#
# Totally agressive optimization.
OPT= -O -fvolatile -fno-builtin -nostdinc -m68040
#
DEFINES= -DUSE_ANSI
#
ELDORA = /ELDORA
#
COMMON= /net/projects/common
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I. -I${ELDORA}/rpc/header -I${ELDORA}/vx/hskp/include\
		-I${ELDORA}/vx/c++/include -I${ELDORA}/rpc/include\
		-I/local/include/vx++ -I${COMMON}/include
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= ${ELDORA}/rpc/lib/libVxHeader.a -L/local/lib/gcc-lib/68k/current \
	-lstdc++

#
OBJS= hskp.o startup.o globals.o main_menu.o start_testpulse.o\
	update_testpulse.o hostStartup.o sum_testpulse.o
#
SRCS= hskp.c startup.cc globals.c main_menu.c sum_testpulse.c\
	start_testpulse.c update_testpulse.c hostStartup.c
#    
HEADERS=
#
CXX = ${CCC}
CXXFLAGS = ${CCFLAGS}
#
# Binaries go here.
BIN= ${ELDORA}/vx/hskp/bin

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
	cp *.o ${ELDORA}/vx/hskp/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h


# DO NOT DELETE

hskp.o: ${ELDORA}/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
hskp.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
hskp.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
hskp.o: /vx/h/sys/types.h /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h
hskp.o: /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
hskp.o: /vx/h/memLib.h /vx/h/private/objLibP.h /vx/h/private/classLibP.h
hskp.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
hskp.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
hskp.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
hskp.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
hskp.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
hskp.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
hskp.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
hskp.o: /vx/h/sysLib.h /vx/h/bootLib.h ${ELDORA}/rpc/header/Volume.h
hskp.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h
hskp.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h /vx/h/rpc/clnt.h
hskp.o: /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
hskp.o: /vx/h/rpc/svc_auth.h ${ELDORA}/rpc/header/Waveform.h
hskp.o: ${ELDORA}/rpc/header/RadarDesc.h ${ELDORA}/rpc/header/FieldRadar.h
hskp.o: ${ELDORA}/rpc/header/CellSpacing.h ${ELDORA}/rpc/header/Parameter.h
hskp.o: ${ELDORA}/rpc/header/NavDesc.h ${ELDORA}/rpc/header/InSitu.h
hskp.o: ${ELDORA}/rpc/header/Ray.h ${ELDORA}/rpc/header/Platform.h
hskp.o: ${ELDORA}/rpc/header/FieldParam.h ${ELDORA}/rpc/header/IndFreq.h
hskp.o: ${ELDORA}/rpc/header/TimeSeries.h ${ELDORA}/rpc/header/NavInfo.h
hskp.o: ${ELDORA}/rpc/header/Ins.h ${ELDORA}/rpc/header/MiniRIMS.h
hskp.o: ${ELDORA}/rpc/header/Gps.h ${ELDORA}/rpc/header/InSituData.h
hskp.o: ${ELDORA}/rpc/header/Header.h ${ELDORA}/rpc/header/TapeHeader.h
hskp.o: ${ELDORA}/rpc/header/HeaderRpc.h ${ELDORA}/vx/hskp/include/cntrlDef.h
hskp.o: ${ELDORA}/vx/hskp/include/cntrlFunc.h
hskp.o: ${ELDORA}/vx/hskp/include/cntrlGbl.h ${ELDORA}/vx/hskp/include/ecbAdr.h
hskp.o: ${ELDORA}/vx/hskp/include/ecbErrBound.h
hskp.o: ${ELDORA}/vx/hskp/include/ecbFunc.h ${ELDORA}/vx/hskp/include/ecbMaster.h
hskp.o: ${ELDORA}/vx/hskp/include/ecbSem.h ${ELDORA}/vx/hskp/include/ecbStat.h
hskp.o: ${ELDORA}/vx/hskp/include/gpsDef.h ${ELDORA}/vx/hskp/include/gpsFunc.h
hskp.o: ${ELDORA}/vx/hskp/include/gpsGbl.h ${ELDORA}/vx/hskp/include/hskpDef.h
hskp.o: ${ELDORA}/vx/hskp/include/hskpFunc.h ${ELDORA}/vx/hskp/include/hskpGbl.h
hskp.o: ${ELDORA}/vx/hskp/include/hskpInt.h ${ELDORA}/vx/hskp/include/iruDef.h
hskp.o: ${ELDORA}/vx/hskp/include/iruFunc.h ${ELDORA}/vx/hskp/include/iruGbl.h
hskp.o: ${ELDORA}/vx/hskp/include/pwrDef.h ${ELDORA}/vx/hskp/include/pwrFunc.h
hskp.o: ${ELDORA}/vx/hskp/include/todDef.h ${ELDORA}/vx/hskp/include/todFunc.h
hskp.o: ${ELDORA}/vx/hskp/include/todGbl.h ${ELDORA}/vx/hskp/include/tp41vAdr.h
hskp.o: ${ELDORA}/vx/hskp/include/vme_hndshk.h
hskp.o: ${ELDORA}/vx/hskp/include/vmevmeAdr.h
hskp.o: ${ELDORA}/vx/hskp/include/vmevmeDef.h
hskp.o: ${ELDORA}/vx/hskp/include/vmevmeFunc.h
hskp.o: ${ELDORA}/vx/hskp/include/vmevmeGbl.h ${ELDORA}/rpc/include/HskpCmd.h
hskp.o: ${ELDORA}/rpc/include/GblCommands.h ${ELDORA}/rpc/include/HskpStatus.h
startup.o: ${ELDORA}/rpc/header/Header.hh ${ELDORA}/rpc/header/Header.h
startup.o: ${ELDORA}/rpc/header/TapeHeader.h ${ELDORA}/rpc/header/CellSpacing.h
startup.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/vxWorks.h
startup.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
startup.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
startup.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
startup.o: /vx/h/sys/times.h /vx/h/netinet/in.h /vx/h/rpc/xdr.h
startup.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h /vx/h/rpc/clnt.h
startup.o: /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
startup.o: /vx/h/rpc/svc_auth.h ${ELDORA}/rpc/header/FieldRadar.h
startup.o: ${ELDORA}/rpc/header/Parameter.h ${ELDORA}/rpc/header/RadarDesc.h
startup.o: ${ELDORA}/rpc/header/Volume.h ${ELDORA}/rpc/header/Waveform.h
startup.o: ${ELDORA}/rpc/header/InSitu.h ${ELDORA}/rpc/header/NavDesc.h
startup.o: /local/include/vx++/iostream.h /local/include/vx++/streambuf.h
startup.o: /local/include/vx++/libio.h /local/include/vx++/_G_config.h
startup.o: ${ELDORA}/rpc/include/Hskp.hh /vx/h/string.h
startup.o: ${ELDORA}/rpc/include/HskpCmd.h ${ELDORA}/rpc/include/GblCommands.h
startup.o: ${ELDORA}/rpc/include/HskpStatus.h /vx/h/stdioLib.h /vx/h/stdio.h
startup.o: /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
startup.o: /vx/h/memLib.h /vx/h/private/objLibP.h /vx/h/private/classLibP.h
startup.o: /vx/h/stdarg.h /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h
startup.o: /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
startup.o: /vx/h/private/semLibP.h /vx/h/semLib.h /vx/h/stdlib.h
startup.o: /vx/h/rpcLib.h ${ELDORA}/rpc/header/HeaderRpc.h
startup.o: ${ELDORA}/vx/hskp/include/hskpFunc.h
globals.o: ${ELDORA}/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
globals.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
globals.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
globals.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
globals.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
globals.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
globals.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
globals.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
globals.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
globals.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
globals.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
globals.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
globals.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
globals.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
globals.o: /vx/h/bootLib.h ${ELDORA}/rpc/header/Volume.h /vx/h/rpc/rpc.h
globals.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
globals.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
globals.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
globals.o: ${ELDORA}/rpc/header/Waveform.h ${ELDORA}/rpc/header/RadarDesc.h
globals.o: ${ELDORA}/rpc/header/FieldRadar.h ${ELDORA}/rpc/header/CellSpacing.h
globals.o: ${ELDORA}/rpc/header/Parameter.h ${ELDORA}/rpc/header/NavDesc.h
globals.o: ${ELDORA}/rpc/header/InSitu.h ${ELDORA}/rpc/header/Ray.h
globals.o: ${ELDORA}/rpc/header/Platform.h ${ELDORA}/rpc/header/FieldParam.h
globals.o: ${ELDORA}/rpc/header/IndFreq.h ${ELDORA}/rpc/header/TimeSeries.h
globals.o: ${ELDORA}/rpc/header/NavInfo.h ${ELDORA}/rpc/header/Ins.h
globals.o: ${ELDORA}/rpc/header/MiniRIMS.h ${ELDORA}/rpc/header/Gps.h
globals.o: ${ELDORA}/rpc/header/InSituData.h ${ELDORA}/rpc/header/Header.h
globals.o: ${ELDORA}/rpc/header/TapeHeader.h ${ELDORA}/rpc/header/HeaderRpc.h
globals.o: ${ELDORA}/vx/hskp/include/cntrlDef.h
globals.o: ${ELDORA}/vx/hskp/include/cntrlFunc.h
globals.o: ${ELDORA}/vx/hskp/include/cntrlGbl.h
globals.o: ${ELDORA}/vx/hskp/include/ecbAdr.h
globals.o: ${ELDORA}/vx/hskp/include/ecbErrBound.h
globals.o: ${ELDORA}/vx/hskp/include/ecbFunc.h
globals.o: ${ELDORA}/vx/hskp/include/ecbMaster.h
globals.o: ${ELDORA}/vx/hskp/include/ecbSem.h ${ELDORA}/vx/hskp/include/ecbStat.h
globals.o: ${ELDORA}/vx/hskp/include/gpsDef.h ${ELDORA}/vx/hskp/include/gpsFunc.h
globals.o: ${ELDORA}/vx/hskp/include/gpsGbl.h ${ELDORA}/vx/hskp/include/hskpDef.h
globals.o: ${ELDORA}/vx/hskp/include/hskpFunc.h
globals.o: ${ELDORA}/vx/hskp/include/hskpGbl.h
globals.o: ${ELDORA}/vx/hskp/include/hskpInt.h ${ELDORA}/vx/hskp/include/iruDef.h
globals.o: ${ELDORA}/vx/hskp/include/iruFunc.h ${ELDORA}/vx/hskp/include/iruGbl.h
globals.o: ${ELDORA}/vx/hskp/include/pwrDef.h ${ELDORA}/vx/hskp/include/pwrFunc.h
globals.o: ${ELDORA}/vx/hskp/include/todDef.h ${ELDORA}/vx/hskp/include/todFunc.h
globals.o: ${ELDORA}/vx/hskp/include/todGbl.h
globals.o: ${ELDORA}/vx/hskp/include/tp41vAdr.h
globals.o: ${ELDORA}/vx/hskp/include/vme_hndshk.h
globals.o: ${ELDORA}/vx/hskp/include/vmevmeAdr.h
globals.o: ${ELDORA}/vx/hskp/include/vmevmeDef.h
globals.o: ${ELDORA}/vx/hskp/include/vmevmeFunc.h
globals.o: ${ELDORA}/vx/hskp/include/vmevmeGbl.h ${ELDORA}/rpc/include/HskpCmd.h
globals.o: ${ELDORA}/rpc/include/GblCommands.h ${ELDORA}/rpc/include/HskpStatus.h
main_menu.o: ${ELDORA}/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
main_menu.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
main_menu.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
main_menu.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
main_menu.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
main_menu.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
main_menu.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
main_menu.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
main_menu.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
main_menu.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
main_menu.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
main_menu.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
main_menu.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
main_menu.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
main_menu.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
main_menu.o: ${ELDORA}/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
main_menu.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
main_menu.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
main_menu.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
main_menu.o: ${ELDORA}/rpc/header/Waveform.h ${ELDORA}/rpc/header/RadarDesc.h
main_menu.o: ${ELDORA}/rpc/header/FieldRadar.h ${ELDORA}/rpc/header/CellSpacing.h
main_menu.o: ${ELDORA}/rpc/header/Parameter.h ${ELDORA}/rpc/header/NavDesc.h
main_menu.o: ${ELDORA}/rpc/header/InSitu.h ${ELDORA}/rpc/header/Ray.h
main_menu.o: ${ELDORA}/rpc/header/Platform.h ${ELDORA}/rpc/header/FieldParam.h
main_menu.o: ${ELDORA}/rpc/header/IndFreq.h ${ELDORA}/rpc/header/TimeSeries.h
main_menu.o: ${ELDORA}/rpc/header/NavInfo.h ${ELDORA}/rpc/header/Ins.h
main_menu.o: ${ELDORA}/rpc/header/MiniRIMS.h ${ELDORA}/rpc/header/Gps.h
main_menu.o: ${ELDORA}/rpc/header/InSituData.h ${ELDORA}/rpc/header/Header.h
main_menu.o: ${ELDORA}/rpc/header/TapeHeader.h ${ELDORA}/rpc/header/HeaderRpc.h
main_menu.o: ${ELDORA}/vx/hskp/include/cntrlDef.h
main_menu.o: ${ELDORA}/vx/hskp/include/cntrlFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/cntrlGbl.h
main_menu.o: ${ELDORA}/vx/hskp/include/ecbAdr.h
main_menu.o: ${ELDORA}/vx/hskp/include/ecbErrBound.h
main_menu.o: ${ELDORA}/vx/hskp/include/ecbFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/ecbMaster.h
main_menu.o: ${ELDORA}/vx/hskp/include/ecbSem.h
main_menu.o: ${ELDORA}/vx/hskp/include/ecbStat.h
main_menu.o: ${ELDORA}/vx/hskp/include/gpsDef.h
main_menu.o: ${ELDORA}/vx/hskp/include/gpsFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/gpsGbl.h
main_menu.o: ${ELDORA}/vx/hskp/include/hskpDef.h
main_menu.o: ${ELDORA}/vx/hskp/include/hskpFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/hskpGbl.h
main_menu.o: ${ELDORA}/vx/hskp/include/hskpInt.h
main_menu.o: ${ELDORA}/vx/hskp/include/iruDef.h
main_menu.o: ${ELDORA}/vx/hskp/include/iruFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/iruGbl.h
main_menu.o: ${ELDORA}/vx/hskp/include/pwrDef.h
main_menu.o: ${ELDORA}/vx/hskp/include/pwrFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/todDef.h
main_menu.o: ${ELDORA}/vx/hskp/include/todFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/todGbl.h
main_menu.o: ${ELDORA}/vx/hskp/include/tp41vAdr.h
main_menu.o: ${ELDORA}/vx/hskp/include/vme_hndshk.h
main_menu.o: ${ELDORA}/vx/hskp/include/vmevmeAdr.h
main_menu.o: ${ELDORA}/vx/hskp/include/vmevmeDef.h
main_menu.o: ${ELDORA}/vx/hskp/include/vmevmeFunc.h
main_menu.o: ${ELDORA}/vx/hskp/include/vmevmeGbl.h
main_menu.o: ${ELDORA}/rpc/include/HskpCmd.h ${ELDORA}/rpc/include/GblCommands.h
main_menu.o: ${ELDORA}/rpc/include/HskpStatus.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
sum_testpulse.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
sum_testpulse.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
sum_testpulse.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
sum_testpulse.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
sum_testpulse.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
sum_testpulse.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
sum_testpulse.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
sum_testpulse.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
sum_testpulse.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
sum_testpulse.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h
sum_testpulse.o: /vx/h/stdlib.h /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h
sum_testpulse.o: /vx/h/limits.h /vx/h/net/uio.h /vx/h/fcntl.h
sum_testpulse.o: /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
sum_testpulse.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
sum_testpulse.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
sum_testpulse.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
sum_testpulse.o: ${ELDORA}/rpc/header/Volume.h /vx/h/rpc/rpc.h
sum_testpulse.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
sum_testpulse.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
sum_testpulse.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
sum_testpulse.o: ${ELDORA}/rpc/header/Waveform.h ${ELDORA}/rpc/header/RadarDesc.h
sum_testpulse.o: ${ELDORA}/rpc/header/FieldRadar.h
sum_testpulse.o: ${ELDORA}/rpc/header/CellSpacing.h
sum_testpulse.o: ${ELDORA}/rpc/header/Parameter.h ${ELDORA}/rpc/header/NavDesc.h
sum_testpulse.o: ${ELDORA}/rpc/header/InSitu.h ${ELDORA}/rpc/header/Ray.h
sum_testpulse.o: ${ELDORA}/rpc/header/Platform.h
sum_testpulse.o: ${ELDORA}/rpc/header/FieldParam.h ${ELDORA}/rpc/header/IndFreq.h
sum_testpulse.o: ${ELDORA}/rpc/header/TimeSeries.h ${ELDORA}/rpc/header/NavInfo.h
sum_testpulse.o: ${ELDORA}/rpc/header/Ins.h ${ELDORA}/rpc/header/MiniRIMS.h
sum_testpulse.o: ${ELDORA}/rpc/header/Gps.h ${ELDORA}/rpc/header/InSituData.h
sum_testpulse.o: ${ELDORA}/rpc/header/Header.h ${ELDORA}/rpc/header/TapeHeader.h
sum_testpulse.o: ${ELDORA}/rpc/header/HeaderRpc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlDef.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlGbl.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/ecbAdr.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/ecbErrBound.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/ecbFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/ecbMaster.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/ecbSem.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/ecbStat.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/gpsDef.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/gpsFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/gpsGbl.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/hskpDef.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/hskpFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/hskpGbl.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/hskpInt.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/iruDef.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/iruFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/iruGbl.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/pwrDef.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/pwrFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/todDef.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/todFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/todGbl.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/tp41vAdr.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/vme_hndshk.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeAdr.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeDef.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeFunc.h
sum_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeGbl.h
sum_testpulse.o: ${ELDORA}/rpc/include/HskpCmd.h
sum_testpulse.o: ${ELDORA}/rpc/include/GblCommands.h
sum_testpulse.o: ${ELDORA}/rpc/include/HskpStatus.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
start_testpulse.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
start_testpulse.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
start_testpulse.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
start_testpulse.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
start_testpulse.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
start_testpulse.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
start_testpulse.o: /vx/h/memLib.h /vx/h/private/objLibP.h
start_testpulse.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/intLib.h
start_testpulse.o: /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h
start_testpulse.o: /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
start_testpulse.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
start_testpulse.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
start_testpulse.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
start_testpulse.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h
start_testpulse.o: /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
start_testpulse.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
start_testpulse.o: /vx/h/sysLib.h /vx/h/bootLib.h ${ELDORA}/rpc/header/Volume.h
start_testpulse.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h
start_testpulse.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h /vx/h/rpc/clnt.h
start_testpulse.o: /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
start_testpulse.o: /vx/h/rpc/svc_auth.h ${ELDORA}/rpc/header/Waveform.h
start_testpulse.o: ${ELDORA}/rpc/header/RadarDesc.h
start_testpulse.o: ${ELDORA}/rpc/header/FieldRadar.h
start_testpulse.o: ${ELDORA}/rpc/header/CellSpacing.h
start_testpulse.o: ${ELDORA}/rpc/header/Parameter.h
start_testpulse.o: ${ELDORA}/rpc/header/NavDesc.h ${ELDORA}/rpc/header/InSitu.h
start_testpulse.o: ${ELDORA}/rpc/header/Ray.h ${ELDORA}/rpc/header/Platform.h
start_testpulse.o: ${ELDORA}/rpc/header/FieldParam.h
start_testpulse.o: ${ELDORA}/rpc/header/IndFreq.h
start_testpulse.o: ${ELDORA}/rpc/header/TimeSeries.h
start_testpulse.o: ${ELDORA}/rpc/header/NavInfo.h ${ELDORA}/rpc/header/Ins.h
start_testpulse.o: ${ELDORA}/rpc/header/MiniRIMS.h ${ELDORA}/rpc/header/Gps.h
start_testpulse.o: ${ELDORA}/rpc/header/InSituData.h
start_testpulse.o: ${ELDORA}/rpc/header/Header.h
start_testpulse.o: ${ELDORA}/rpc/header/TapeHeader.h
start_testpulse.o: ${ELDORA}/rpc/header/HeaderRpc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlDef.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlGbl.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/ecbAdr.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/ecbErrBound.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/ecbFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/ecbMaster.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/ecbSem.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/ecbStat.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/gpsDef.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/gpsFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/gpsGbl.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/hskpDef.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/hskpFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/hskpGbl.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/hskpInt.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/iruDef.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/iruFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/iruGbl.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/pwrDef.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/pwrFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/todDef.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/todFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/todGbl.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/tp41vAdr.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/vme_hndshk.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeAdr.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeDef.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeFunc.h
start_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeGbl.h
start_testpulse.o: ${ELDORA}/rpc/include/HskpCmd.h
start_testpulse.o: ${ELDORA}/rpc/include/GblCommands.h
start_testpulse.o: ${ELDORA}/rpc/include/HskpStatus.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
update_testpulse.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
update_testpulse.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
update_testpulse.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
update_testpulse.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
update_testpulse.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
update_testpulse.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
update_testpulse.o: /vx/h/memLib.h /vx/h/private/objLibP.h
update_testpulse.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/intLib.h
update_testpulse.o: /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
update_testpulse.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
update_testpulse.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h
update_testpulse.o: /vx/h/stdlib.h /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h
update_testpulse.o: /vx/h/limits.h /vx/h/net/uio.h /vx/h/fcntl.h
update_testpulse.o: /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
update_testpulse.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
update_testpulse.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
update_testpulse.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
update_testpulse.o: ${ELDORA}/rpc/header/Volume.h /vx/h/rpc/rpc.h
update_testpulse.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
update_testpulse.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
update_testpulse.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
update_testpulse.o: /vx/h/rpc/svc_auth.h ${ELDORA}/rpc/header/Waveform.h
update_testpulse.o: ${ELDORA}/rpc/header/RadarDesc.h
update_testpulse.o: ${ELDORA}/rpc/header/FieldRadar.h
update_testpulse.o: ${ELDORA}/rpc/header/CellSpacing.h
update_testpulse.o: ${ELDORA}/rpc/header/Parameter.h
update_testpulse.o: ${ELDORA}/rpc/header/NavDesc.h ${ELDORA}/rpc/header/InSitu.h
update_testpulse.o: ${ELDORA}/rpc/header/Ray.h ${ELDORA}/rpc/header/Platform.h
update_testpulse.o: ${ELDORA}/rpc/header/FieldParam.h
update_testpulse.o: ${ELDORA}/rpc/header/IndFreq.h
update_testpulse.o: ${ELDORA}/rpc/header/TimeSeries.h
update_testpulse.o: ${ELDORA}/rpc/header/NavInfo.h ${ELDORA}/rpc/header/Ins.h
update_testpulse.o: ${ELDORA}/rpc/header/MiniRIMS.h ${ELDORA}/rpc/header/Gps.h
update_testpulse.o: ${ELDORA}/rpc/header/InSituData.h
update_testpulse.o: ${ELDORA}/rpc/header/Header.h
update_testpulse.o: ${ELDORA}/rpc/header/TapeHeader.h
update_testpulse.o: ${ELDORA}/rpc/header/HeaderRpc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlDef.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/cntrlGbl.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/ecbAdr.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/ecbErrBound.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/ecbFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/ecbMaster.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/ecbSem.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/ecbStat.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/gpsDef.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/gpsFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/gpsGbl.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/hskpDef.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/hskpFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/hskpGbl.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/hskpInt.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/iruDef.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/iruFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/iruGbl.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/pwrDef.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/pwrFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/todDef.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/todFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/todGbl.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/tp41vAdr.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/vme_hndshk.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeAdr.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeDef.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeFunc.h
update_testpulse.o: ${ELDORA}/vx/hskp/include/vmevmeGbl.h
update_testpulse.o: ${ELDORA}/rpc/include/HskpCmd.h
update_testpulse.o: ${ELDORA}/rpc/include/GblCommands.h
update_testpulse.o: ${ELDORA}/rpc/include/HskpStatus.h
hostStartup.o: ${ELDORA}/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
hostStartup.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
hostStartup.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
hostStartup.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
hostStartup.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
hostStartup.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
hostStartup.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
hostStartup.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
hostStartup.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
hostStartup.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
hostStartup.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
hostStartup.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
hostStartup.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
hostStartup.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
hostStartup.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
hostStartup.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
hostStartup.o: ${ELDORA}/rpc/header/Volume.h /vx/h/rpc/rpc.h
hostStartup.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
hostStartup.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
hostStartup.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
hostStartup.o: ${ELDORA}/rpc/header/Waveform.h ${ELDORA}/rpc/header/RadarDesc.h
hostStartup.o: ${ELDORA}/rpc/header/FieldRadar.h
hostStartup.o: ${ELDORA}/rpc/header/CellSpacing.h
hostStartup.o: ${ELDORA}/rpc/header/Parameter.h ${ELDORA}/rpc/header/NavDesc.h
hostStartup.o: ${ELDORA}/rpc/header/InSitu.h ${ELDORA}/rpc/header/Ray.h
hostStartup.o: ${ELDORA}/rpc/header/Platform.h ${ELDORA}/rpc/header/FieldParam.h
hostStartup.o: ${ELDORA}/rpc/header/IndFreq.h ${ELDORA}/rpc/header/TimeSeries.h
hostStartup.o: ${ELDORA}/rpc/header/NavInfo.h ${ELDORA}/rpc/header/Ins.h
hostStartup.o: ${ELDORA}/rpc/header/MiniRIMS.h ${ELDORA}/rpc/header/Gps.h
hostStartup.o: ${ELDORA}/rpc/header/InSituData.h ${ELDORA}/rpc/header/Header.h
hostStartup.o: ${ELDORA}/rpc/header/TapeHeader.h ${ELDORA}/rpc/header/HeaderRpc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/cntrlDef.h
hostStartup.o: ${ELDORA}/vx/hskp/include/cntrlFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/cntrlGbl.h
hostStartup.o: ${ELDORA}/vx/hskp/include/ecbAdr.h
hostStartup.o: ${ELDORA}/vx/hskp/include/ecbErrBound.h
hostStartup.o: ${ELDORA}/vx/hskp/include/ecbFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/ecbMaster.h
hostStartup.o: ${ELDORA}/vx/hskp/include/ecbSem.h
hostStartup.o: ${ELDORA}/vx/hskp/include/ecbStat.h
hostStartup.o: ${ELDORA}/vx/hskp/include/gpsDef.h
hostStartup.o: ${ELDORA}/vx/hskp/include/gpsFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/gpsGbl.h
hostStartup.o: ${ELDORA}/vx/hskp/include/hskpDef.h
hostStartup.o: ${ELDORA}/vx/hskp/include/hskpFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/hskpGbl.h
hostStartup.o: ${ELDORA}/vx/hskp/include/hskpInt.h
hostStartup.o: ${ELDORA}/vx/hskp/include/iruDef.h
hostStartup.o: ${ELDORA}/vx/hskp/include/iruFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/iruGbl.h
hostStartup.o: ${ELDORA}/vx/hskp/include/pwrDef.h
hostStartup.o: ${ELDORA}/vx/hskp/include/pwrFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/todDef.h
hostStartup.o: ${ELDORA}/vx/hskp/include/todFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/todGbl.h
hostStartup.o: ${ELDORA}/vx/hskp/include/tp41vAdr.h
hostStartup.o: ${ELDORA}/vx/hskp/include/vme_hndshk.h
hostStartup.o: ${ELDORA}/vx/hskp/include/vmevmeAdr.h
hostStartup.o: ${ELDORA}/vx/hskp/include/vmevmeDef.h
hostStartup.o: ${ELDORA}/vx/hskp/include/vmevmeFunc.h
hostStartup.o: ${ELDORA}/vx/hskp/include/vmevmeGbl.h
hostStartup.o: ${ELDORA}/rpc/include/HskpCmd.h
hostStartup.o: ${ELDORA}/rpc/include/GblCommands.h
hostStartup.o: ${ELDORA}/rpc/include/HskpStatus.h
