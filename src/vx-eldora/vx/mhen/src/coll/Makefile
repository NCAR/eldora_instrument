#
#	Makefile for Directory coll
#	Author: Eric Loew
#	Date: 12/3/91
#
# GNU C 
CC= gcc68k -fno-builtin -nostdinc
#
DEFINES= -DTADPL -DNU_ARCHIT
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I/ELDORA/vx/mhen/include -I/ELDORA/dsp/include \
	-I/ELDORA/vx/mhen/src/nu_archit -I/ELDORA/rpc/include
#
CFLAGS= $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS=coll_opflags.o gate_avg.o coll_probe.o col_sync.o coll_diag.o
#
SRCS=coll_opflags.c gate_avg.c coll_probe.c col_sync.c coll_diag.c
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

coll:
	ld68k -X -r -o coll.o $(OBJS)
	install -m 774 coll.o $(BIN)

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

dsy.o:
	$(CC) -c $(INCLUDES) $(DEFINES) -O $<		

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

coll_opflags.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
coll_opflags.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
coll_opflags.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
coll_opflags.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
coll_opflags.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
coll_opflags.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
coll_opflags.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
coll_opflags.o: /vx/h/stdarg.h /vx/h/ctype.h /ELDORA/vx/mhen/include/ELDRP7.h
coll_opflags.o: /ELDORA/dsp/include/coll_dpramdefs.h
coll_opflags.o: /ELDORA/dsp/include/dspaddr.h
gate_avg.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
gate_avg.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
gate_avg.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
gate_avg.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
gate_avg.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
gate_avg.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
gate_avg.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
gate_avg.o: /ELDORA/vx/mhen/include/ELDRP7.h
gate_avg.o: /ELDORA/dsp/include/coll_dpramdefs.h
gate_avg.o: /ELDORA/dsp/include/dspaddr.h
coll_probe.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
coll_probe.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
coll_probe.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
coll_probe.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
coll_probe.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
coll_probe.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
coll_probe.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
coll_probe.o: /ELDORA/vx/mhen/include/ELDRP7.h
col_sync.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
col_sync.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
col_sync.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
col_sync.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
col_sync.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
col_sync.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
col_sync.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
col_sync.o: /ELDORA/vx/mhen/include/ELDRP7.h
col_sync.o: /ELDORA/dsp/include/coll_dpramdefs.h
col_sync.o: /ELDORA/dsp/include/dspaddr.h
coll_diag.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
coll_diag.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
coll_diag.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
coll_diag.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
coll_diag.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
coll_diag.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
coll_diag.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/ctype.h
coll_diag.o: /vx/h/semLib.h /ELDORA/rpc/include/RadarGbls.h
coll_diag.o: /ELDORA/rpc/include/Header.h /ELDORA/rpc/include/TapeHeader.h
coll_diag.o: /ELDORA/rpc/include/CellSpacing.h
coll_diag.o: /ELDORA/rpc/include/FieldRadar.h /ELDORA/rpc/include/Parameter.h
coll_diag.o: /ELDORA/rpc/include/RadarDesc.h /ELDORA/rpc/include/Volume.h
coll_diag.o: /ELDORA/rpc/include/Waveform.h /ELDORA/rpc/include/InSitu.h
coll_diag.o: /ELDORA/rpc/include/NavDesc.h /ELDORA/vx/mhen/include/ELDRP7.h
coll_diag.o: /ELDORA/dsp/include/coll_dpramdefs.h
coll_diag.o: /ELDORA/dsp/include/dspaddr.h
