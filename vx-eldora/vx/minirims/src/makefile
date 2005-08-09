#
#	Makefile for Directory ELDORA/vx/hskp/src/minirims
#	Author: Craig Walther
#	Date: 8/19/92
#
# GNU C 
CC= gcc68k
CCC = $(CC)
CXX = ${CC}
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
INCLUDES= -I/vx/h -I. -I/ELDORA/rpc/header -I/ELDORA/vx/minirims/include\
	-I/local/include/vx++ -I/ELDORA/rpc/include -I/ELDORA/vx/c/include \
	-I/ELDORA/vx/tp41 -I/net/projects/common/include \
	-I/ELDORA/vx/c++/include
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
CXXFLAGS= ${CFLAGS}
#
COMM_DIR= /net/projects
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a \
	/ELDORA/vx/c/lib/libcurses.a /ELDORA/vx/tp41/tp41Lib.o \
	/ELDORA/vx/c/lib/libtermlib.a /ELDORA/rpc/lib/libVxMinirimsRpc.a \
        -L${COMM_DIR}/common/lib/vxworks -lNet \
        -L/local/lib/gcc-lib/m68k-wrs-vxworks/current -lstdc++  -lgcc

#
OBJS= mininit.o mini_vme.o align_mini.o init_disp.o mini_rd.o mini_start.o \
      mini_dport.o mini_rpc.o dpram.o
#
SRCS= mininit.c mini_vme.c align_mini.c init_disp.c mini_rd.c mini_start.c \
      mini_dport.c mini_rpc.c dpram.c
#    
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN= /ELDORA/vx/minirims/bin

.KEEP_STATE:

compile: $(OBJS)
	ld68k -X -r -o mini.o $(OBJS) $(LIBS)
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
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,shawn,rich,eric,reif -c" * " -L $$i); \
	done;

copyem:
	cp *.o /ELDORA/vx/hskp/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h



# DO NOT DELETE THIS LINE -- make depend depends on it.

mininit.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
mininit.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mininit.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
mininit.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mininit.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
mininit.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mininit.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
mininit.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
mininit.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
mininit.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
mininit.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
mininit.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
mininit.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
mininit.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
mininit.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
mininit.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
mininit.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
mininit.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mininit.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
mininit.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
mininit.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
mininit.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
mininit.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
mininit.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
mininit.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
mininit.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
mininit.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
mininit.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
mininit.o: /ELDORA/vx/hskp/include/cntrlDef.h
mininit.o: /ELDORA/vx/hskp/include/cntrlFunc.h
mininit.o: /ELDORA/vx/hskp/include/cntrlGbl.h
mininit.o: /ELDORA/vx/hskp/include/ecbAdr.h
mininit.o: /ELDORA/vx/hskp/include/ecbErrBound.h
mininit.o: /ELDORA/vx/hskp/include/ecbFunc.h
mininit.o: /ELDORA/vx/hskp/include/ecbMaster.h
mininit.o: /ELDORA/vx/hskp/include/ecbSem.h /ELDORA/vx/hskp/include/ecbStat.h
mininit.o: /ELDORA/vx/hskp/include/gpsDef.h /ELDORA/vx/hskp/include/gpsFunc.h
mininit.o: /ELDORA/vx/hskp/include/gpsGbl.h /ELDORA/vx/hskp/include/hskpDef.h
mininit.o: /ELDORA/vx/hskp/include/hskpFunc.h
mininit.o: /ELDORA/vx/hskp/include/hskpGbl.h
mininit.o: /ELDORA/vx/hskp/include/hskpInt.h /ELDORA/vx/hskp/include/iruDef.h
mininit.o: /ELDORA/vx/hskp/include/iruFunc.h /ELDORA/vx/hskp/include/iruGbl.h
mininit.o: /ELDORA/vx/hskp/include/minDef.h /ELDORA/vx/hskp/include/minFunc.h
mininit.o: /ELDORA/vx/hskp/include/pwrDef.h /ELDORA/vx/hskp/include/pwrFunc.h
mininit.o: /ELDORA/vx/hskp/include/todDef.h /ELDORA/vx/hskp/include/todFunc.h
mininit.o: /ELDORA/vx/hskp/include/todGbl.h
mininit.o: /ELDORA/vx/hskp/include/tp41vAdr.h
mininit.o: /ELDORA/vx/hskp/include/vme_hndshk.h
mininit.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
mininit.o: /ELDORA/vx/hskp/include/vmevmeDef.h
mininit.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
mininit.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
mininit.o: /ELDORA/rpc/include/GblCommands.h /ELDORA/rpc/include/HskpStatus.h
mini_vme.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
mini_vme.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_vme.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
mini_vme.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_vme.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
mini_vme.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mini_vme.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
mini_vme.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
mini_vme.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
mini_vme.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
mini_vme.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
mini_vme.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
mini_vme.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
mini_vme.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
mini_vme.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
mini_vme.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
mini_vme.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
mini_vme.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mini_vme.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
mini_vme.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
mini_vme.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
mini_vme.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
mini_vme.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
mini_vme.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
mini_vme.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
mini_vme.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
mini_vme.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
mini_vme.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
mini_vme.o: /ELDORA/vx/hskp/include/cntrlDef.h
mini_vme.o: /ELDORA/vx/hskp/include/cntrlFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/cntrlGbl.h
mini_vme.o: /ELDORA/vx/hskp/include/ecbAdr.h
mini_vme.o: /ELDORA/vx/hskp/include/ecbErrBound.h
mini_vme.o: /ELDORA/vx/hskp/include/ecbFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/ecbMaster.h
mini_vme.o: /ELDORA/vx/hskp/include/ecbSem.h
mini_vme.o: /ELDORA/vx/hskp/include/ecbStat.h
mini_vme.o: /ELDORA/vx/hskp/include/gpsDef.h
mini_vme.o: /ELDORA/vx/hskp/include/gpsFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/gpsGbl.h
mini_vme.o: /ELDORA/vx/hskp/include/hskpDef.h
mini_vme.o: /ELDORA/vx/hskp/include/hskpFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/hskpGbl.h
mini_vme.o: /ELDORA/vx/hskp/include/hskpInt.h
mini_vme.o: /ELDORA/vx/hskp/include/iruDef.h
mini_vme.o: /ELDORA/vx/hskp/include/iruFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/iruGbl.h /ELDORA/vx/hskp/include/minDef.h
mini_vme.o: /ELDORA/vx/hskp/include/minFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/pwrDef.h
mini_vme.o: /ELDORA/vx/hskp/include/pwrFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/todDef.h
mini_vme.o: /ELDORA/vx/hskp/include/todFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/todGbl.h
mini_vme.o: /ELDORA/vx/hskp/include/tp41vAdr.h
mini_vme.o: /ELDORA/vx/hskp/include/vme_hndshk.h
mini_vme.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
mini_vme.o: /ELDORA/vx/hskp/include/vmevmeDef.h
mini_vme.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
mini_vme.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
mini_vme.o: /ELDORA/rpc/include/GblCommands.h
mini_vme.o: /ELDORA/rpc/include/HskpStatus.h /ELDORA/vx/c/include/curses.h
mini_vme.o: /usr/include/sgtty.h /ELDORA/vx/hskp/include/minGbl.h
mini_vme.o: /ELDORA/vx/hskp/include/minDisp.h /vx/h/string.h
align_mini.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
align_mini.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
align_mini.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
align_mini.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
align_mini.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
align_mini.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
align_mini.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
align_mini.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
align_mini.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
align_mini.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
align_mini.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
align_mini.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
align_mini.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
align_mini.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
align_mini.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
align_mini.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
align_mini.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
align_mini.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
align_mini.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
align_mini.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
align_mini.o: /ELDORA/rpc/header/FieldRadar.h
align_mini.o: /ELDORA/rpc/header/CellSpacing.h /ELDORA/rpc/header/Parameter.h
align_mini.o: /ELDORA/rpc/header/NavDesc.h /ELDORA/rpc/header/InSitu.h
align_mini.o: /ELDORA/rpc/header/Ray.h /ELDORA/rpc/header/Platform.h
align_mini.o: /ELDORA/rpc/header/FieldParam.h /ELDORA/rpc/header/IndFreq.h
align_mini.o: /ELDORA/rpc/header/TimeSeries.h /ELDORA/rpc/header/NavInfo.h
align_mini.o: /ELDORA/rpc/header/Ins.h /ELDORA/rpc/header/MiniRIMS.h
align_mini.o: /ELDORA/rpc/header/Gps.h /ELDORA/rpc/header/InSituData.h
align_mini.o: /ELDORA/rpc/header/Header.h /ELDORA/rpc/header/TapeHeader.h
align_mini.o: /ELDORA/rpc/header/HeaderRpc.h
align_mini.o: /ELDORA/vx/hskp/include/cntrlDef.h
align_mini.o: /ELDORA/vx/hskp/include/cntrlFunc.h
align_mini.o: /ELDORA/vx/hskp/include/cntrlGbl.h
align_mini.o: /ELDORA/vx/hskp/include/ecbAdr.h
align_mini.o: /ELDORA/vx/hskp/include/ecbErrBound.h
align_mini.o: /ELDORA/vx/hskp/include/ecbFunc.h
align_mini.o: /ELDORA/vx/hskp/include/ecbMaster.h
align_mini.o: /ELDORA/vx/hskp/include/ecbSem.h
align_mini.o: /ELDORA/vx/hskp/include/ecbStat.h
align_mini.o: /ELDORA/vx/hskp/include/gpsDef.h
align_mini.o: /ELDORA/vx/hskp/include/gpsFunc.h
align_mini.o: /ELDORA/vx/hskp/include/gpsGbl.h
align_mini.o: /ELDORA/vx/hskp/include/hskpDef.h
align_mini.o: /ELDORA/vx/hskp/include/hskpFunc.h
align_mini.o: /ELDORA/vx/hskp/include/hskpGbl.h
align_mini.o: /ELDORA/vx/hskp/include/hskpInt.h
align_mini.o: /ELDORA/vx/hskp/include/iruDef.h
align_mini.o: /ELDORA/vx/hskp/include/iruFunc.h
align_mini.o: /ELDORA/vx/hskp/include/iruGbl.h
align_mini.o: /ELDORA/vx/hskp/include/minDef.h
align_mini.o: /ELDORA/vx/hskp/include/minFunc.h
align_mini.o: /ELDORA/vx/hskp/include/pwrDef.h
align_mini.o: /ELDORA/vx/hskp/include/pwrFunc.h
align_mini.o: /ELDORA/vx/hskp/include/todDef.h
align_mini.o: /ELDORA/vx/hskp/include/todFunc.h
align_mini.o: /ELDORA/vx/hskp/include/todGbl.h
align_mini.o: /ELDORA/vx/hskp/include/tp41vAdr.h
align_mini.o: /ELDORA/vx/hskp/include/vme_hndshk.h
align_mini.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
align_mini.o: /ELDORA/vx/hskp/include/vmevmeDef.h
align_mini.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
align_mini.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
align_mini.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
align_mini.o: /ELDORA/rpc/include/HskpStatus.h
init_disp.o: /vx/h/stdio.h /vx/h/types/vxANSI.h /vx/h/types/vxCpu.h
init_disp.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
init_disp.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
init_disp.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
init_disp.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_disp.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
init_disp.o: /vx/h/stdlib.h /ELDORA/vx/c/include/curses.h
init_disp.o: /usr/include/sgtty.h /ELDORA/vx/hskp/include/minDisp.h
init_disp.o: /vx/h/string.h /ELDORA/vx/hskp/include/minGbl.h /vx/h/semLib.h
mini_rd.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
mini_rd.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_rd.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
mini_rd.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_rd.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
mini_rd.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mini_rd.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
mini_rd.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
mini_rd.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
mini_rd.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
mini_rd.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
mini_rd.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
mini_rd.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
mini_rd.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
mini_rd.o: /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
mini_rd.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
mini_rd.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
mini_rd.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mini_rd.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
mini_rd.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
mini_rd.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
mini_rd.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
mini_rd.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
mini_rd.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
mini_rd.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
mini_rd.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
mini_rd.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
mini_rd.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
mini_rd.o: /ELDORA/vx/hskp/include/cntrlDef.h
mini_rd.o: /ELDORA/vx/hskp/include/cntrlFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/cntrlGbl.h
mini_rd.o: /ELDORA/vx/hskp/include/ecbAdr.h
mini_rd.o: /ELDORA/vx/hskp/include/ecbErrBound.h
mini_rd.o: /ELDORA/vx/hskp/include/ecbFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/ecbMaster.h
mini_rd.o: /ELDORA/vx/hskp/include/ecbSem.h /ELDORA/vx/hskp/include/ecbStat.h
mini_rd.o: /ELDORA/vx/hskp/include/gpsDef.h /ELDORA/vx/hskp/include/gpsFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/gpsGbl.h /ELDORA/vx/hskp/include/hskpDef.h
mini_rd.o: /ELDORA/vx/hskp/include/hskpFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/hskpGbl.h
mini_rd.o: /ELDORA/vx/hskp/include/hskpInt.h /ELDORA/vx/hskp/include/iruDef.h
mini_rd.o: /ELDORA/vx/hskp/include/iruFunc.h /ELDORA/vx/hskp/include/iruGbl.h
mini_rd.o: /ELDORA/vx/hskp/include/minDef.h /ELDORA/vx/hskp/include/minFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/pwrDef.h /ELDORA/vx/hskp/include/pwrFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/todDef.h /ELDORA/vx/hskp/include/todFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/todGbl.h
mini_rd.o: /ELDORA/vx/hskp/include/tp41vAdr.h
mini_rd.o: /ELDORA/vx/hskp/include/vme_hndshk.h
mini_rd.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
mini_rd.o: /ELDORA/vx/hskp/include/vmevmeDef.h
mini_rd.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
mini_rd.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
mini_rd.o: /ELDORA/rpc/include/GblCommands.h /ELDORA/rpc/include/HskpStatus.h
mini_rd.o: /ELDORA/vx/c/include/curses.h /usr/include/sgtty.h
mini_rd.o: /ELDORA/vx/hskp/include/minGbl.h /ELDORA/vx/hskp/include/minDisp.h
mini_rd.o: /vx/h/string.h
mini_start.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
mini_start.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
mini_start.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
mini_start.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/taskLib.h
mini_start.o: /vx/h/vwModNum.h /vx/h/classLib.h /vx/h/objLib.h /vx/h/errno.h
mini_start.o: /vx/h/memLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
mini_start.o: /vx/h/excLib.h /vx/h/private/eventP.h /vx/h/private/semLibP.h
mini_start.o: /vx/h/semLib.h /vx/h/private/classLibP.h
mini_start.o: /vx/h/private/objLibP.h /vx/h/stdlib.h
mini_dport.o: /ELDORA/vx/tp41/tp41Lib.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
mini_dport.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_dport.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
mini_dport.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_rpc.o: /ELDORA/rpc/include/MinirimsRpc.hh /vx/h/vxWorks.h
mini_rpc.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_rpc.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
mini_rpc.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_rpc.o: /vx/h/string.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
mini_rpc.o: /vx/h/sys/times.h /vx/h/netinet/in.h /vx/h/rpc/xdr.h
mini_rpc.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h /vx/h/rpc/clnt.h
mini_rpc.o: /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
mini_rpc.o: /vx/h/rpc/svc_auth.h /vx/h/stdlib.h /vx/h/taskLib.h
mini_rpc.o: /vx/h/vwModNum.h /vx/h/classLib.h /vx/h/objLib.h /vx/h/errno.h
mini_rpc.o: /vx/h/memLib.h /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h
mini_rpc.o: /vx/h/excLib.h /vx/h/private/eventP.h /vx/h/private/semLibP.h
mini_rpc.o: /vx/h/semLib.h /vx/h/private/classLibP.h /vx/h/private/objLibP.h
mini_rpc.o: /ELDORA/rpc/include/GblCommands.h
mini_rpc.o: /ELDORA/vx/hskp/include/minGbl.h
mini_rpc.o: /ELDORA/vx/hskp/include/minDisp.h /vx/h/stdioLib.h /vx/h/stdio.h
mini_rpc.o: /vx/h/stdarg.h
