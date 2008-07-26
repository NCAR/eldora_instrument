#
#	Makefile for Directory ELDORA/vx/hskp/src/power
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
INCLUDES= -I/vx/h -I. -I/ELDORA/vx/hskp/include\
	-I/ELDORA/vx/c++/include -I/ELDORA/rpc/header -I/ELDORA/rpc/include \
	-I/local/include/vx++
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS=  fltpt.o init_dma.o init_gpib.o init_isr_vecs.o init_ptrs.o\
	listen.o xmit_rt.o pwrmenu.o send_cmnd_string.o start_ieee.o\
	testp_err_isr.o testp_norm_isr.o xmit_err_isr.o xmit_norm_isr.o \
	talk.o stop_ieee.o init_ieee.o testp_rt.o test_pwr.o pgm_rt.o \
	lab_test.o ReadStatusByte.o ReceiveData.o Return_to_Local.o
#
SRCS=  fltpt.c init_dma.c init_gpib.c init_isr_vecs.c init_ptrs.c\
	listen.c xmit_rt.c pwrmenu.c send_cmnd_string.c start_ieee.c\
	testp_err_isr.c testp_norm_isr.c xmit_err_isr.c xmit_norm_isr.c \
	talk.c stop_ieee.c init_ieee.c testp_rt.c test_pwr.c pgm_rt.c \
	lab_test.c ReadStatusByte.c ReceiveData.c Return_to_Local.c
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

copyem:
	cp *.o /ELDORA/vx/hskp/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h


# DO NOT DELETE

fltpt.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
fltpt.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
fltpt.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
fltpt.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
fltpt.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
fltpt.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
fltpt.o: /vx/h/math.h /ELDORA/vx/hskp/include/pwrDef.h /vx/h/semLib.h
fltpt.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_dma.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
init_dma.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
init_dma.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
init_dma.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
init_dma.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
init_dma.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
init_dma.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/math.h
init_dma.o: /vx/h/string.h /ELDORA/vx/hskp/include/pwrDef.h /vx/h/semLib.h
init_dma.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_gpib.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
init_gpib.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
init_gpib.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
init_gpib.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
init_gpib.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
init_gpib.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
init_gpib.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/math.h
init_gpib.o: /vx/h/string.h /ELDORA/vx/hskp/include/pwrDef.h /vx/h/semLib.h
init_gpib.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_isr_vecs.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
init_isr_vecs.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
init_isr_vecs.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
init_isr_vecs.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
init_isr_vecs.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
init_isr_vecs.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_isr_vecs.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
init_isr_vecs.o: /vx/h/stdarg.h /vx/h/math.h /vx/h/string.h
init_isr_vecs.o: /ELDORA/vx/hskp/include/pwrDef.h /vx/h/semLib.h
init_isr_vecs.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_ptrs.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
init_ptrs.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
init_ptrs.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
init_ptrs.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
init_ptrs.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
init_ptrs.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
init_ptrs.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/string.h
init_ptrs.o: /ELDORA/vx/hskp/include/pwrDef.h /vx/h/semLib.h
init_ptrs.o: /ELDORA/vx/hskp/include/pwrFunc.h
listen.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
listen.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
listen.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
listen.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
listen.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
listen.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
listen.o: /vx/h/math.h /vx/h/string.h /ELDORA/vx/hskp/include/pwrDef.h
listen.o: /vx/h/semLib.h /ELDORA/vx/hskp/include/pwrFunc.h
pwr.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
pwr.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
pwr.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
pwr.o: /vx/h/sys/types.h /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h
pwr.o: /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
pwr.o: /vx/h/memLib.h /vx/h/private/objLibP.h /vx/h/private/classLibP.h
pwr.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
pwr.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
pwr.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
pwr.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
pwr.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
pwr.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
pwr.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h
pwr.o: /vx/h/sysLib.h /vx/h/bootLib.h /ELDORA/rpc/header/Volume.h
pwr.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h
pwr.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h /vx/h/rpc/clnt.h
pwr.o: /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h
pwr.o: /vx/h/rpc/svc_auth.h /ELDORA/rpc/header/Waveform.h
pwr.o: /ELDORA/rpc/header/RadarDesc.h /ELDORA/rpc/header/FieldRadar.h
pwr.o: /ELDORA/rpc/header/CellSpacing.h /ELDORA/rpc/header/Parameter.h
pwr.o: /ELDORA/rpc/header/NavDesc.h /ELDORA/rpc/header/InSitu.h
pwr.o: /ELDORA/rpc/header/Ray.h /ELDORA/rpc/header/Platform.h
pwr.o: /ELDORA/rpc/header/FieldParam.h /ELDORA/rpc/header/IndFreq.h
pwr.o: /ELDORA/rpc/header/TimeSeries.h /ELDORA/rpc/header/NavInfo.h
pwr.o: /ELDORA/rpc/header/Ins.h /ELDORA/rpc/header/MiniRIMS.h
pwr.o: /ELDORA/rpc/header/Gps.h /ELDORA/rpc/header/InSituData.h
pwr.o: /ELDORA/rpc/header/Header.h /ELDORA/rpc/header/TapeHeader.h
pwr.o: /ELDORA/rpc/header/HeaderRpc.h /ELDORA/vx/hskp/include/cntrlDef.h
pwr.o: /ELDORA/vx/hskp/include/cntrlFunc.h /ELDORA/vx/hskp/include/cntrlGbl.h
pwr.o: /ELDORA/vx/hskp/include/ecbAdr.h /ELDORA/vx/hskp/include/ecbErrBound.h
pwr.o: /ELDORA/vx/hskp/include/ecbFunc.h /ELDORA/vx/hskp/include/ecbMaster.h
pwr.o: /ELDORA/vx/hskp/include/ecbSem.h /ELDORA/vx/hskp/include/ecbStat.h
pwr.o: /ELDORA/vx/hskp/include/gpsDef.h /ELDORA/vx/hskp/include/gpsFunc.h
pwr.o: /ELDORA/vx/hskp/include/gpsGbl.h /ELDORA/vx/hskp/include/hskpDef.h
pwr.o: /ELDORA/vx/hskp/include/hskpFunc.h /ELDORA/vx/hskp/include/hskpGbl.h
pwr.o: /ELDORA/vx/hskp/include/hskpInt.h /ELDORA/vx/hskp/include/iruDef.h
pwr.o: /ELDORA/vx/hskp/include/iruFunc.h /ELDORA/vx/hskp/include/iruGbl.h
pwr.o: /ELDORA/vx/hskp/include/pwrDef.h /ELDORA/vx/hskp/include/pwrFunc.h
pwr.o: /ELDORA/vx/hskp/include/todDef.h /ELDORA/vx/hskp/include/todFunc.h
pwr.o: /ELDORA/vx/hskp/include/todGbl.h /ELDORA/vx/hskp/include/tp41vAdr.h
pwr.o: /ELDORA/vx/hskp/include/vme_hndshk.h
pwr.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
pwr.o: /ELDORA/vx/hskp/include/vmevmeDef.h
pwr.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
pwr.o: /ELDORA/vx/hskp/include/vmevmeGbl.h /ELDORA/rpc/include/HskpCmd.h
pwr.o: /ELDORA/rpc/include/GblCommands.h /ELDORA/rpc/include/HskpStatus.h
pwrmenu.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
pwrmenu.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
pwrmenu.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
pwrmenu.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
pwrmenu.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
pwrmenu.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
pwrmenu.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/math.h
pwrmenu.o: /vx/h/string.h /ELDORA/vx/hskp/include/pwrDef.h /vx/h/semLib.h
pwrmenu.o: /ELDORA/vx/hskp/include/pwrFunc.h
send_cmnd_string.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
send_cmnd_string.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
send_cmnd_string.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
send_cmnd_string.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
send_cmnd_string.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
send_cmnd_string.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
send_cmnd_string.o: /vx/h/memLib.h /vx/h/private/objLibP.h
send_cmnd_string.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/math.h
send_cmnd_string.o: /vx/h/string.h /ELDORA/vx/hskp/include/pwrDef.h
send_cmnd_string.o: /vx/h/semLib.h /ELDORA/vx/hskp/include/pwrFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
start_ieee.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
start_ieee.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
start_ieee.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
start_ieee.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
start_ieee.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
start_ieee.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
start_ieee.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
start_ieee.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
start_ieee.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
start_ieee.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
start_ieee.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
start_ieee.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
start_ieee.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
start_ieee.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
start_ieee.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h
start_ieee.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
start_ieee.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
start_ieee.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
start_ieee.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
start_ieee.o: /ELDORA/rpc/header/FieldRadar.h
start_ieee.o: /ELDORA/rpc/header/CellSpacing.h /ELDORA/rpc/header/Parameter.h
start_ieee.o: /ELDORA/rpc/header/NavDesc.h /ELDORA/rpc/header/InSitu.h
start_ieee.o: /ELDORA/rpc/header/Ray.h /ELDORA/rpc/header/Platform.h
start_ieee.o: /ELDORA/rpc/header/FieldParam.h /ELDORA/rpc/header/IndFreq.h
start_ieee.o: /ELDORA/rpc/header/TimeSeries.h /ELDORA/rpc/header/NavInfo.h
start_ieee.o: /ELDORA/rpc/header/Ins.h /ELDORA/rpc/header/MiniRIMS.h
start_ieee.o: /ELDORA/rpc/header/Gps.h /ELDORA/rpc/header/InSituData.h
start_ieee.o: /ELDORA/rpc/header/Header.h /ELDORA/rpc/header/TapeHeader.h
start_ieee.o: /ELDORA/rpc/header/HeaderRpc.h
start_ieee.o: /ELDORA/vx/hskp/include/cntrlDef.h
start_ieee.o: /ELDORA/vx/hskp/include/cntrlFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/cntrlGbl.h
start_ieee.o: /ELDORA/vx/hskp/include/ecbAdr.h
start_ieee.o: /ELDORA/vx/hskp/include/ecbErrBound.h
start_ieee.o: /ELDORA/vx/hskp/include/ecbFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/ecbMaster.h
start_ieee.o: /ELDORA/vx/hskp/include/ecbSem.h
start_ieee.o: /ELDORA/vx/hskp/include/ecbStat.h
start_ieee.o: /ELDORA/vx/hskp/include/gpsDef.h
start_ieee.o: /ELDORA/vx/hskp/include/gpsFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/gpsGbl.h
start_ieee.o: /ELDORA/vx/hskp/include/hskpDef.h
start_ieee.o: /ELDORA/vx/hskp/include/hskpFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/hskpGbl.h
start_ieee.o: /ELDORA/vx/hskp/include/hskpInt.h
start_ieee.o: /ELDORA/vx/hskp/include/iruDef.h
start_ieee.o: /ELDORA/vx/hskp/include/iruFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/iruGbl.h
start_ieee.o: /ELDORA/vx/hskp/include/pwrDef.h
start_ieee.o: /ELDORA/vx/hskp/include/pwrFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/todDef.h
start_ieee.o: /ELDORA/vx/hskp/include/todFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/todGbl.h
start_ieee.o: /ELDORA/vx/hskp/include/tp41vAdr.h
start_ieee.o: /ELDORA/vx/hskp/include/vme_hndshk.h
start_ieee.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
start_ieee.o: /ELDORA/vx/hskp/include/vmevmeDef.h
start_ieee.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
start_ieee.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
start_ieee.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
start_ieee.o: /ELDORA/rpc/include/HskpStatus.h
testp_err_isr.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
testp_err_isr.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
testp_err_isr.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
testp_err_isr.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
testp_err_isr.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
testp_err_isr.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
testp_err_isr.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
testp_err_isr.o: /vx/h/stdarg.h /vx/h/math.h /ELDORA/vx/hskp/include/pwrDef.h
testp_err_isr.o: /vx/h/semLib.h /ELDORA/vx/hskp/include/pwrFunc.h
testp_norm_isr.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
testp_norm_isr.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
testp_norm_isr.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
testp_norm_isr.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
testp_norm_isr.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
testp_norm_isr.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
testp_norm_isr.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
testp_norm_isr.o: /vx/h/stdarg.h /vx/h/math.h
testp_norm_isr.o: /ELDORA/vx/hskp/include/pwrDef.h /vx/h/semLib.h
testp_norm_isr.o: /ELDORA/vx/hskp/include/pwrFunc.h
xmit_err_isr.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
xmit_err_isr.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
xmit_err_isr.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
xmit_err_isr.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
xmit_err_isr.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
xmit_err_isr.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
xmit_err_isr.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
xmit_err_isr.o: /vx/h/stdarg.h /vx/h/math.h /ELDORA/vx/hskp/include/pwrDef.h
xmit_err_isr.o: /vx/h/semLib.h /ELDORA/vx/hskp/include/pwrFunc.h
xmit_norm_isr.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
xmit_norm_isr.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
xmit_norm_isr.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
xmit_norm_isr.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
xmit_norm_isr.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
xmit_norm_isr.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
xmit_norm_isr.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
xmit_norm_isr.o: /vx/h/stdarg.h /vx/h/math.h /ELDORA/vx/hskp/include/pwrDef.h
xmit_norm_isr.o: /vx/h/semLib.h /ELDORA/vx/hskp/include/pwrFunc.h
talk.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
talk.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
talk.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
talk.o: /vx/h/sys/types.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
talk.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
talk.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
talk.o: /vx/h/math.h /vx/h/string.h /ELDORA/vx/hskp/include/pwrDef.h
talk.o: /vx/h/semLib.h /ELDORA/vx/hskp/include/pwrFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
stop_ieee.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
stop_ieee.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
stop_ieee.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
stop_ieee.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
stop_ieee.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
stop_ieee.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
stop_ieee.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
stop_ieee.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
stop_ieee.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
stop_ieee.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
stop_ieee.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
stop_ieee.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
stop_ieee.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
stop_ieee.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
stop_ieee.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
stop_ieee.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
stop_ieee.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
stop_ieee.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
stop_ieee.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
stop_ieee.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
stop_ieee.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
stop_ieee.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
stop_ieee.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
stop_ieee.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
stop_ieee.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
stop_ieee.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
stop_ieee.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
stop_ieee.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
stop_ieee.o: /ELDORA/vx/hskp/include/cntrlDef.h
stop_ieee.o: /ELDORA/vx/hskp/include/cntrlFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/cntrlGbl.h
stop_ieee.o: /ELDORA/vx/hskp/include/ecbAdr.h
stop_ieee.o: /ELDORA/vx/hskp/include/ecbErrBound.h
stop_ieee.o: /ELDORA/vx/hskp/include/ecbFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/ecbMaster.h
stop_ieee.o: /ELDORA/vx/hskp/include/ecbSem.h
stop_ieee.o: /ELDORA/vx/hskp/include/ecbStat.h
stop_ieee.o: /ELDORA/vx/hskp/include/gpsDef.h
stop_ieee.o: /ELDORA/vx/hskp/include/gpsFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/gpsGbl.h
stop_ieee.o: /ELDORA/vx/hskp/include/hskpDef.h
stop_ieee.o: /ELDORA/vx/hskp/include/hskpFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/hskpGbl.h
stop_ieee.o: /ELDORA/vx/hskp/include/hskpInt.h
stop_ieee.o: /ELDORA/vx/hskp/include/iruDef.h
stop_ieee.o: /ELDORA/vx/hskp/include/iruFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/iruGbl.h
stop_ieee.o: /ELDORA/vx/hskp/include/pwrDef.h
stop_ieee.o: /ELDORA/vx/hskp/include/pwrFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/todDef.h
stop_ieee.o: /ELDORA/vx/hskp/include/todFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/todGbl.h
stop_ieee.o: /ELDORA/vx/hskp/include/tp41vAdr.h
stop_ieee.o: /ELDORA/vx/hskp/include/vme_hndshk.h
stop_ieee.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
stop_ieee.o: /ELDORA/vx/hskp/include/vmevmeDef.h
stop_ieee.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
stop_ieee.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
stop_ieee.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
stop_ieee.o: /ELDORA/rpc/include/HskpStatus.h
init_ieee.o: /ELDORA/vx/hskp/include/hskpAll.h /vx/h/vxWorks.h
init_ieee.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
init_ieee.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
init_ieee.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
init_ieee.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
init_ieee.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_ieee.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
init_ieee.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
init_ieee.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
init_ieee.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
init_ieee.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
init_ieee.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
init_ieee.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
init_ieee.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
init_ieee.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
init_ieee.o: /ELDORA/rpc/header/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
init_ieee.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
init_ieee.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
init_ieee.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
init_ieee.o: /ELDORA/rpc/header/Waveform.h /ELDORA/rpc/header/RadarDesc.h
init_ieee.o: /ELDORA/rpc/header/FieldRadar.h /ELDORA/rpc/header/CellSpacing.h
init_ieee.o: /ELDORA/rpc/header/Parameter.h /ELDORA/rpc/header/NavDesc.h
init_ieee.o: /ELDORA/rpc/header/InSitu.h /ELDORA/rpc/header/Ray.h
init_ieee.o: /ELDORA/rpc/header/Platform.h /ELDORA/rpc/header/FieldParam.h
init_ieee.o: /ELDORA/rpc/header/IndFreq.h /ELDORA/rpc/header/TimeSeries.h
init_ieee.o: /ELDORA/rpc/header/NavInfo.h /ELDORA/rpc/header/Ins.h
init_ieee.o: /ELDORA/rpc/header/MiniRIMS.h /ELDORA/rpc/header/Gps.h
init_ieee.o: /ELDORA/rpc/header/InSituData.h /ELDORA/rpc/header/Header.h
init_ieee.o: /ELDORA/rpc/header/TapeHeader.h /ELDORA/rpc/header/HeaderRpc.h
init_ieee.o: /ELDORA/vx/hskp/include/cntrlDef.h
init_ieee.o: /ELDORA/vx/hskp/include/cntrlFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/cntrlGbl.h
init_ieee.o: /ELDORA/vx/hskp/include/ecbAdr.h
init_ieee.o: /ELDORA/vx/hskp/include/ecbErrBound.h
init_ieee.o: /ELDORA/vx/hskp/include/ecbFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/ecbMaster.h
init_ieee.o: /ELDORA/vx/hskp/include/ecbSem.h
init_ieee.o: /ELDORA/vx/hskp/include/ecbStat.h
init_ieee.o: /ELDORA/vx/hskp/include/gpsDef.h
init_ieee.o: /ELDORA/vx/hskp/include/gpsFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/gpsGbl.h
init_ieee.o: /ELDORA/vx/hskp/include/hskpDef.h
init_ieee.o: /ELDORA/vx/hskp/include/hskpFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/hskpGbl.h
init_ieee.o: /ELDORA/vx/hskp/include/hskpInt.h
init_ieee.o: /ELDORA/vx/hskp/include/iruDef.h
init_ieee.o: /ELDORA/vx/hskp/include/iruFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/iruGbl.h
init_ieee.o: /ELDORA/vx/hskp/include/pwrDef.h
init_ieee.o: /ELDORA/vx/hskp/include/pwrFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/todDef.h
init_ieee.o: /ELDORA/vx/hskp/include/todFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/todGbl.h
init_ieee.o: /ELDORA/vx/hskp/include/tp41vAdr.h
init_ieee.o: /ELDORA/vx/hskp/include/vme_hndshk.h
init_ieee.o: /ELDORA/vx/hskp/include/vmevmeAdr.h
init_ieee.o: /ELDORA/vx/hskp/include/vmevmeDef.h
init_ieee.o: /ELDORA/vx/hskp/include/vmevmeFunc.h
init_ieee.o: /ELDORA/vx/hskp/include/vmevmeGbl.h
init_ieee.o: /ELDORA/rpc/include/HskpCmd.h /ELDORA/rpc/include/GblCommands.h
init_ieee.o: /ELDORA/rpc/include/HskpStatus.h
