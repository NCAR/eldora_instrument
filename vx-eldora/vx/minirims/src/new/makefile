#
#	Makefile for Directory ELDORA/vx/hskp/src/minirims/new
#	Author: Eric Loew
#	Date: 9/24/96
#
# GNU C 
CC= gcc68k
CCC = $(CC)
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
	-I/ELDORA/vx/c++/include -I/ELDORA/rpc/include -I/ELDORA/vx/c/include \
	-I/ELDORA/vx/tp41
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/hskp/bin/get_time.o \
	/ELDORA/vx/c/lib/libcurses.a /ELDORA/vx/tp41/tp41Lib.o \
	/ELDORA/vx/c/lib/libtermlib.a /ELDORA/rpc/lib/libVxMinirimsRpc.a \
	/ELDORA/vx/hskp/bin/globals.o \
	-L/local/lib/gcc-lib/68k/current -lstdc++ -lgcc
#
OBJS= mininit.o mini_vme.o align_mini.o init_disp.o mini_rd.o mini_start.o \
      mini_dport.o mini_rpc.o mini_rt_rd.o read_adc.o
#
SRCS= mininit.c mini_vme.c align_mini.c init_disp.c mini_rd.c mini_start.c \
      mini_dport.c mini_rpc.c mini_rt_rd.c read_adc.c
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
	bsdinst -m 774 mini.o $(BIN)

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

mininit.o: /ELDORA/vx/minirims/include/numinDef.h
mininit.o: /ELDORA/rpc/header/MiniRIMS.h
mini_vme.o: /ELDORA/vx/minirims/include/numinDef.h
mini_vme.o: /ELDORA/rpc/header/MiniRIMS.h /vx/h/stdio.h /vx/h/types/vxANSI.h
mini_vme.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_vme.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
mini_vme.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_vme.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mini_vme.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
mini_vme.o: /vx/h/stdlib.h /ELDORA/vx/c/include/curses.h /usr/include/sgtty.h
mini_vme.o: /ELDORA/vx/minirims/include/minGbl.h
mini_vme.o: /ELDORA/vx/minirims/include/minDisp.h /vx/h/semLib.h
mini_vme.o: /ELDORA/rpc/include/MinirimsRpc.hh /vx/h/string.h /vx/h/rpc/rpc.h
mini_vme.o: /vx/h/rpc/rpctypes.h /vx/h/sys/times.h /vx/h/netinet/in.h
mini_vme.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
mini_vme.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
mini_vme.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h /vx/h/taskLib.h /vx/h/qLib.h
mini_vme.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
mini_vme.o: /vx/h/private/semLibP.h
align_mini.o: /ELDORA/vx/minirims/include/minGbl.h
align_mini.o: /ELDORA/vx/minirims/include/minDisp.h /vx/h/semLib.h
align_mini.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
align_mini.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
align_mini.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
align_mini.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/vwModNum.h
align_mini.o: /ELDORA/rpc/include/MinirimsRpc.hh /vx/h/string.h
align_mini.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
align_mini.o: /vx/h/netinet/in.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
align_mini.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
align_mini.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
align_mini.o: /vx/h/stdlib.h /ELDORA/vx/minirims/include/numinDef.h
align_mini.o: /ELDORA/rpc/header/MiniRIMS.h
init_disp.o: /vx/h/stdio.h /vx/h/types/vxANSI.h /vx/h/types/vxCpu.h
init_disp.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
init_disp.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
init_disp.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
init_disp.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
init_disp.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
init_disp.o: /vx/h/stdlib.h /ELDORA/vx/c/include/curses.h
init_disp.o: /usr/include/sgtty.h /ELDORA/vx/minirims/include/minDisp.h
init_disp.o: /vx/h/string.h /ELDORA/vx/minirims/include/minGbl.h
init_disp.o: /vx/h/semLib.h /ELDORA/rpc/include/MinirimsRpc.hh
init_disp.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
init_disp.o: /vx/h/netinet/in.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
init_disp.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
init_disp.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mini_rd.o: /ELDORA/vx/minirims/include/numinDef.h
mini_rd.o: /ELDORA/rpc/header/MiniRIMS.h /vx/h/stdio.h /vx/h/types/vxANSI.h
mini_rd.o: /vx/h/types/vxCpu.h /vx/h/types/vxArch.h /vx/h/types/vxParams.h
mini_rd.o: /vx/h/types/vxTypesBase.h /vx/h/classLib.h /vx/h/vxWorks.h
mini_rd.o: /vx/h/types/vxTypes.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
mini_rd.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mini_rd.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
mini_rd.o: /vx/h/stdlib.h /ELDORA/vx/c/include/curses.h /usr/include/sgtty.h
mini_rd.o: /ELDORA/vx/minirims/include/minGbl.h
mini_rd.o: /ELDORA/vx/minirims/include/minDisp.h /vx/h/semLib.h
mini_rd.o: /ELDORA/rpc/include/MinirimsRpc.hh /vx/h/string.h /vx/h/rpc/rpc.h
mini_rd.o: /vx/h/rpc/rpctypes.h /vx/h/sys/times.h /vx/h/netinet/in.h
mini_rd.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
mini_rd.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
mini_rd.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mini_start.o: /ELDORA/vx/minirims/include/minGbl.h
mini_start.o: /ELDORA/vx/minirims/include/minDisp.h /vx/h/semLib.h
mini_start.o: /vx/h/vxWorks.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
mini_start.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
mini_start.o: /vx/h/types/vxTypes.h /vx/h/types/vxANSI.h
mini_start.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/vwModNum.h
mini_start.o: /ELDORA/rpc/include/MinirimsRpc.hh /vx/h/string.h
mini_start.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
mini_start.o: /vx/h/netinet/in.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
mini_start.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
mini_start.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
mini_start.o: /vx/h/stdlib.h /vx/h/taskLib.h /vx/h/classLib.h /vx/h/objLib.h
mini_start.o: /vx/h/errno.h /vx/h/memLib.h /vx/h/qLib.h /vx/h/qClass.h
mini_start.o: /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
mini_start.o: /vx/h/private/semLibP.h /vx/h/private/classLibP.h
mini_start.o: /vx/h/private/objLibP.h
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
mini_rpc.o: /ELDORA/vx/minirims/include/minGbl.h
mini_rpc.o: /ELDORA/vx/minirims/include/minDisp.h
mini_rpc.o: /ELDORA/vx/minirims/include/numinDef.h
mini_rpc.o: /ELDORA/rpc/header/MiniRIMS.h /vx/h/stdioLib.h /vx/h/stdio.h
mini_rpc.o: /vx/h/stdarg.h
mini_rt_rd.o: /ELDORA/vx/minirims/include/numinDef.h
mini_rt_rd.o: /ELDORA/rpc/header/MiniRIMS.h /vx/h/stdio.h
mini_rt_rd.o: /vx/h/types/vxANSI.h /vx/h/types/vxCpu.h /vx/h/types/vxArch.h
mini_rt_rd.o: /vx/h/types/vxParams.h /vx/h/types/vxTypesBase.h
mini_rt_rd.o: /vx/h/classLib.h /vx/h/vxWorks.h /vx/h/types/vxTypes.h
mini_rt_rd.o: /vx/h/types/vxTypesOld.h /vx/h/sys/types.h /vx/h/vwModNum.h
mini_rt_rd.o: /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
mini_rt_rd.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
mini_rt_rd.o: /vx/h/stdarg.h /vx/h/stdlib.h /ELDORA/vx/c/include/curses.h
mini_rt_rd.o: /usr/include/sgtty.h /ELDORA/vx/minirims/include/minGbl.h
mini_rt_rd.o: /ELDORA/vx/minirims/include/minDisp.h /vx/h/semLib.h
mini_rt_rd.o: /ELDORA/rpc/include/MinirimsRpc.hh /vx/h/string.h
mini_rt_rd.o: /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h /vx/h/sys/times.h
mini_rt_rd.o: /vx/h/netinet/in.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
mini_rt_rd.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
mini_rt_rd.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
