#
#	Makefile for Directory ad 
#	Author: Eric Loew
#	Date: 12/2/91
#
# GNU C 
CC= gcc68k -fno-builtin -nostdinc
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
	-fomit-frame-pointer
OPT= -O2
#
DEFINES= -DTADPL -DELDORA
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I/ELDORA/vx/mhen/include -I/ELDORA/dsp/include \
          -I/ELDORA/vx/mhen/src/nu_archit
#
CFLAGS= $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS=fsetcal.o i3e2dsp.o atod_hexd.o prog_admisc.o prog_calram.o \
     prog_choiceram.o mysetcal.o prog_digif.o
#
NU_ARCH_OBJS=fsetcal.o i3e2dsp.o atod_hexd.o prog_admisc.o prog_calram.o \
     prog_choiceram.o mysetcal.o prog_digif.o
#
SRCS=fsetcal.c i3e2dsp.c atod_hexd.c prog_admisc.c prog_calram.c \
     prog_choiceram.c mysetcal.c prog_digif.c
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

ad:
	ld68k -X -r -o ad.o $(NU_ARCH_OBJS)
	install -m 774 ad.o $(BIN)
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

fsetcal.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
fsetcal.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
fsetcal.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
fsetcal.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
fsetcal.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
fsetcal.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
fsetcal.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/math.h
fsetcal.o: /ELDORA/vx/mhen/include/ELDRP7.h
atod_hexd.o: /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/types/vxANSI.h
atod_hexd.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
atod_hexd.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
atod_hexd.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
atod_hexd.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
atod_hexd.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
atod_hexd.o: /ELDORA/vx/mhen/include/ELDRP7.h
prog_admisc.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
prog_admisc.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
prog_admisc.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
prog_admisc.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
prog_admisc.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
prog_admisc.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
prog_admisc.o: /vx/h/private/classLibP.h /vx/h/stdarg.h
prog_admisc.o: /ELDORA/vx/mhen/include/ELDRP7.h
prog_calram.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
prog_calram.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
prog_calram.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
prog_calram.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
prog_calram.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
prog_calram.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
prog_calram.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/math.h
prog_calram.o: /ELDORA/vx/mhen/include/ELDRP7.h
prog_choiceram.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
prog_choiceram.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
prog_choiceram.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
prog_choiceram.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
prog_choiceram.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h
prog_choiceram.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
prog_choiceram.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
prog_choiceram.o: /vx/h/stdarg.h /vx/h/math.h
prog_choiceram.o: /ELDORA/vx/mhen/include/ELDRP7.h
mysetcal.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
mysetcal.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
mysetcal.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
mysetcal.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
mysetcal.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
mysetcal.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
mysetcal.o: /vx/h/private/classLibP.h /vx/h/stdarg.h
mysetcal.o: /ELDORA/vx/mhen/include/ELDRP7.h /vx/h/math.h
prog_digif.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
prog_digif.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
prog_digif.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
prog_digif.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/stdioLib.h
prog_digif.o: /vx/h/stdio.h /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h
prog_digif.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/private/objLibP.h
prog_digif.o: /vx/h/private/classLibP.h /vx/h/stdarg.h
prog_digif.o: /ELDORA/vx/mhen/src/nu_archit/NU_ELDRP7.h /vx/h/math.h
