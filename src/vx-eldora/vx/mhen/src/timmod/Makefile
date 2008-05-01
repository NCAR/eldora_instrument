#
#	Makefile for Directory timmod
#	Author: Eric Loew
#	Date: 12/3/91
#
# GNU C 
CC= gcc68k -fno-builtin -nostdinc
#
# Totally agressive optimization.
OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
	-fomit-frame-pointer
#
DEFINES= -DUSE_ANSI -DTADPL -DNU_ARCH
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I/ELDORA/vx/mhen/include \
          -I/ELDORA/dsp/include -I/ELDORA/rpc/include -I/ELDORA/rpc/mhen \
          -I/ELDORA/vx/hskp/include


#
CFLAGS= $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= timing.o timingb.o timon.o timonb.o timoff.o timoffb.o\
     timing2.o timingb2.o timing3.o timingb3.o timingb4.o timingb5.o\
     testpulseb.o testpulsecw.o load_waveformb.o call_lw.o\
     load_gatesb.o call_lg.o staggerb3.o write_coll.o timingb3second.o\
     timonbsecond.o tst_timing.o ld_timmod.o latch_daisy.o testpulse_test.o
#
SRCS= timing.c timingb.c timon.c timonb.c timoff.c timoffb.c\
     timing2.c timingb2.c timing3.c timingb3.c timingb4.c timingb5.c\
     testpulseb.c testpulsecw.c load_waveformb.c call_lw.c\
     load_gatesb.c call_lg.c staggerb3.c write_coll.c timingb3second.c\
     timonbsecond.c tst_timing.c ld_timmod.c latch_daisy.c testpulse_test.c

#
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN=/ELDORA/vx/mhen/bin

.KEEP_STATE:

compile: $(OBJS)

timmod:
	ld68k -X -r -o timmod.o $(OBJS)
	install -m 774 timmod.o $(BIN)

depend:
	makedepend ${INCLUDES} ${SRCS}

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

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;

# Modify this to include an access list if desired.
set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,shawn,rich,eric,reif -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






# DO NOT DELETE THIS LINE -- make depend depends on it.

timing.o: /ELDORA/vx/mhen/include/ELDRP7.h
timing.o: /ELDORA/dsp/include/coll_dpramdefs.h /ELDORA/dsp/include/dspaddr.h
timingb.o: /ELDORA/vx/mhen/include/ELDRP7.h
timingb.o: /ELDORA/dsp/include/coll_dpramdefs.h /ELDORA/dsp/include/dspaddr.h
timon.o: /ELDORA/vx/mhen/include/ELDRP7.h
timonb.o: /ELDORA/vx/mhen/include/ELDRP7.h
timoff.o: /ELDORA/vx/mhen/include/ELDRP7.h
timoffb.o: /ELDORA/vx/mhen/include/ELDRP7.h
timing2.o: /ELDORA/vx/mhen/include/ELDRP7.h
timing2.o: /ELDORA/dsp/include/coll_dpramdefs.h /ELDORA/dsp/include/dspaddr.h
timingb2.o: /ELDORA/vx/mhen/include/ELDRP7.h
timingb2.o: /ELDORA/dsp/include/coll_dpramdefs.h
timingb2.o: /ELDORA/dsp/include/dspaddr.h
timing3.o: /ELDORA/vx/mhen/include/ELDRP7.h
timing3.o: /ELDORA/dsp/include/coll_dpramdefs.h /ELDORA/dsp/include/dspaddr.h
timingb3.o: /ELDORA/vx/mhen/include/ELDRP7.h
timingb3.o: /ELDORA/dsp/include/coll_dpramdefs.h
timingb3.o: /ELDORA/dsp/include/dspaddr.h
timingb4.o: /ELDORA/vx/mhen/include/ELDRP7.h
timingb4.o: /ELDORA/dsp/include/coll_dpramdefs.h
timingb4.o: /ELDORA/dsp/include/dspaddr.h
timingb5.o: /ELDORA/vx/mhen/include/ELDRP7.h
timingb5.o: /ELDORA/dsp/include/coll_dpramdefs.h
timingb5.o: /ELDORA/dsp/include/dspaddr.h
testpulseb.o: /ELDORA/vx/mhen/include/ELDRP7.h
testpulsecw.o: /ELDORA/vx/mhen/include/ELDRP7.h
load_waveformb.o: /ELDORA/vx/mhen/include/ELDRP7.h
call_lw.o: /ELDORA/vx/mhen/include/ELDRP7.h
load_gatesb.o: /ELDORA/vx/mhen/include/ELDRP7.h
call_lg.o: /ELDORA/vx/mhen/include/ELDRP7.h
staggerb3.o: /ELDORA/vx/mhen/include/ELDRP7.h
write_coll.o: /ELDORA/vx/mhen/include/ELDRP7.h
write_coll.o: /ELDORA/dsp/include/coll_dpramdefs.h
write_coll.o: /ELDORA/dsp/include/dspaddr.h
timingb3second.o: /ELDORA/vx/mhen/include/ELDRP7.h
timingb3second.o: /ELDORA/dsp/include/coll_dpramdefs.h
timingb3second.o: /ELDORA/dsp/include/dspaddr.h
timonbsecond.o: /ELDORA/vx/mhen/include/ELDRP7.h
tst_timing.o: /ELDORA/vx/mhen/include/ELDRP7.h
tst_timing.o: /ELDORA/dsp/include/coll_dpramdefs.h
tst_timing.o: /ELDORA/dsp/include/dspaddr.h
ld_timmod.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
ld_timmod.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
ld_timmod.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
ld_timmod.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/math.h
ld_timmod.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
ld_timmod.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
ld_timmod.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
ld_timmod.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
ld_timmod.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
ld_timmod.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
ld_timmod.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
ld_timmod.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
ld_timmod.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
ld_timmod.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
ld_timmod.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h
ld_timmod.o: /ELDORA/rpc/include/HeaderRpc.h /vx/h/rpc/rpc.h
ld_timmod.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
ld_timmod.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
ld_timmod.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
ld_timmod.o: /ELDORA/rpc/include/Parameter.h /ELDORA/rpc/include/Header.h
ld_timmod.o: /ELDORA/rpc/include/TapeHeader.h
ld_timmod.o: /ELDORA/rpc/include/CellSpacing.h
ld_timmod.o: /ELDORA/rpc/include/FieldRadar.h /ELDORA/rpc/include/RadarDesc.h
ld_timmod.o: /ELDORA/rpc/include/Volume.h /ELDORA/rpc/include/Waveform.h
ld_timmod.o: /ELDORA/rpc/include/InSitu.h /ELDORA/rpc/include/NavDesc.h
ld_timmod.o: /ELDORA/rpc/include/Ray.h /ELDORA/rpc/include/Platform.h
ld_timmod.o: /ELDORA/rpc/include/FieldParam.h /ELDORA/rpc/include/RadarGbls.h
ld_timmod.o: /ELDORA/vx/mhen/include/RDPGlobals.h
ld_timmod.o: /ELDORA/vx/mhen/include/mcpl_def.h
ld_timmod.o: /ELDORA/vx/mhen/include/ELDRP7.h /ELDORA/rpc/include/raw_data.h
ld_timmod.o: /ELDORA/vx/mhen/include/mcpl_gbl.h
ld_timmod.o: /ELDORA/vx/hskp/include/vme_hndshk.h
ld_timmod.o: /ELDORA/rpc/include/TimeSeries.h /ELDORA/rpc/include/IndFreq.h
ld_timmod.o: /ELDORA/dsp/include/coll_dpramdefs.h
ld_timmod.o: /ELDORA/dsp/include/dspaddr.h /ELDORA/dsp/include/colquadsubs.h
ld_timmod.o: /ELDORA/dsp/include/dspdefs.h /ELDORA/dsp/include/rp7.h
latch_daisy.o: /ELDORA/vx/mhen/include/ELDRP7.h
testpulse_test.o: /vx/h/taskLib.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
testpulse_test.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
testpulse_test.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
testpulse_test.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
testpulse_test.o: /vx/h/sys/types.h /vx/h/vwModNum.h /vx/h/classLib.h
testpulse_test.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h /vx/h/qLib.h
testpulse_test.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
testpulse_test.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h
testpulse_test.o: /vx/h/semLib.h /vx/h/private/classLibP.h
testpulse_test.o: /vx/h/private/objLibP.h /vx/h/stdlib.h /vx/h/stdio.h
testpulse_test.o: /vx/h/stdarg.h
