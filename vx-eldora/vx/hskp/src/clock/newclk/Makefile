#
# New makefile for SABL.
# 
# 22 Jan 2001 - 1st edition, R. Neitzel
#

include ../../../sabl.conf
include ../../../vx_sabl.conf

SRCS = get_time.c init_clock.c clock_menu.c set_time.c start_clock.c
SRCS += send_packet.c get_year.c

OBJS = $(SRCS:.c=.o)

NAME = libclock.a

all: ${OBJS}
	${AR} crv ${NAME} ${OBJS} 

install:
	@if [ -d ${VXDEST}/${BINDIR} ]; then set +x; \
	else (set -x; ${MKDIRHIER} ${VXDEST}/${BINDIR}); fi

	${INSTALL_PROG} ${VXDEST}/${BINDIR} ${NAME}
	${RANLIB} ${VXDEST}/${BINDIR}/${NAME}

depend:
	${MAKEDEPEND} ${INCLUDES} ${SRCS}
# DO NOT DELETE

get_time.o: ../include/sablIncs.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
get_time.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
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
get_time.o: /vx/h/bootLib.h /vx/h/sockLib.h /vx/h/sys/socket.h
get_time.o: ../../../include/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
get_time.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
get_time.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
get_time.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
get_time.o: ../../../include/CellSpacing.h ../../../include/Parameter.h
get_time.o: ../../../include/Ray.h ../../../include/Platform.h
get_time.o: ../../../include/LidarDesc.h ../../../include/FieldLidar.h
get_time.o: ../../../include/LidarParam.h ../include/mvme162Def.h
get_time.o: ../include/sablDef.h ../include/sablGbl.h ../include/dataStruct.h
get_time.o: ../include/sablFuncs.h ../include/paraDef.h ../include/paraGbl.h
get_time.o: ../include/sloadDef.h ../include/sloadGbl.h ../include/dtoaDef.h
get_time.o: ../include/dtoaGbl.h ../include/clockDef.h ../include/clockGbl.h
get_time.o: ../include/digitDef.h ../include/digitGbl.h ../include/iruDef.h
get_time.o: ../include/iruGbl.h ../include/dispGbl.h ../include/gpsDef.h
get_time.o: ../include/gpsGbl.h ../include/interfaceDef.h
get_time.o: ../include/interfaceGbl.h ../include/motorDef.h
get_time.o: ../include/motorGbl.h ../include/relayDef.h ../include/relayGbl.h
get_time.o: ../include/laserGbl.h
init_clock.o: ../include/sablIncs.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
init_clock.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
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
init_clock.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h /vx/h/sockLib.h
init_clock.o: /vx/h/sys/socket.h ../../../include/Volume.h /vx/h/rpc/rpc.h
init_clock.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
init_clock.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
init_clock.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
init_clock.o: ../../../include/CellSpacing.h ../../../include/Parameter.h
init_clock.o: ../../../include/Ray.h ../../../include/Platform.h
init_clock.o: ../../../include/LidarDesc.h ../../../include/FieldLidar.h
init_clock.o: ../../../include/LidarParam.h ../include/mvme162Def.h
init_clock.o: ../include/sablDef.h ../include/sablGbl.h
init_clock.o: ../include/dataStruct.h ../include/sablFuncs.h
init_clock.o: ../include/paraDef.h ../include/paraGbl.h ../include/sloadDef.h
init_clock.o: ../include/sloadGbl.h ../include/dtoaDef.h ../include/dtoaGbl.h
init_clock.o: ../include/clockDef.h ../include/clockGbl.h
init_clock.o: ../include/digitDef.h ../include/digitGbl.h ../include/iruDef.h
init_clock.o: ../include/iruGbl.h ../include/dispGbl.h ../include/gpsDef.h
init_clock.o: ../include/gpsGbl.h ../include/interfaceDef.h
init_clock.o: ../include/interfaceGbl.h ../include/motorDef.h
init_clock.o: ../include/motorGbl.h ../include/relayDef.h
init_clock.o: ../include/relayGbl.h ../include/laserGbl.h
clock_menu.o: ../include/sablIncs.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
clock_menu.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
clock_menu.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
clock_menu.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
clock_menu.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
clock_menu.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
clock_menu.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h
clock_menu.o: /vx/h/stdarg.h /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h
clock_menu.o: /vx/h/qLib.h /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h
clock_menu.o: /vx/h/private/eventP.h /vx/h/private/semLibP.h /vx/h/stdlib.h
clock_menu.o: /vx/h/tyLib.h /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h
clock_menu.o: /vx/h/net/uio.h /vx/h/fcntl.h /vx/h/sys/fcntlcom.h
clock_menu.o: /vx/h/unistd.h /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h
clock_menu.o: /vx/h/lstLib.h /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
clock_menu.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h /vx/h/sockLib.h
clock_menu.o: /vx/h/sys/socket.h ../../../include/Volume.h /vx/h/rpc/rpc.h
clock_menu.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
clock_menu.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
clock_menu.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
clock_menu.o: ../../../include/CellSpacing.h ../../../include/Parameter.h
clock_menu.o: ../../../include/Ray.h ../../../include/Platform.h
clock_menu.o: ../../../include/LidarDesc.h ../../../include/FieldLidar.h
clock_menu.o: ../../../include/LidarParam.h ../include/mvme162Def.h
clock_menu.o: ../include/sablDef.h ../include/sablGbl.h
clock_menu.o: ../include/dataStruct.h ../include/sablFuncs.h
clock_menu.o: ../include/paraDef.h ../include/paraGbl.h ../include/sloadDef.h
clock_menu.o: ../include/sloadGbl.h ../include/dtoaDef.h ../include/dtoaGbl.h
clock_menu.o: ../include/clockDef.h ../include/clockGbl.h
clock_menu.o: ../include/digitDef.h ../include/digitGbl.h ../include/iruDef.h
clock_menu.o: ../include/iruGbl.h ../include/dispGbl.h ../include/gpsDef.h
clock_menu.o: ../include/gpsGbl.h ../include/interfaceDef.h
clock_menu.o: ../include/interfaceGbl.h ../include/motorDef.h
clock_menu.o: ../include/motorGbl.h ../include/relayDef.h
clock_menu.o: ../include/relayGbl.h ../include/laserGbl.h
set_time.o: ../include/sablIncs.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
set_time.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
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
set_time.o: /vx/h/bootLib.h /vx/h/sockLib.h /vx/h/sys/socket.h
set_time.o: ../../../include/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
set_time.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
set_time.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
set_time.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
set_time.o: ../../../include/CellSpacing.h ../../../include/Parameter.h
set_time.o: ../../../include/Ray.h ../../../include/Platform.h
set_time.o: ../../../include/LidarDesc.h ../../../include/FieldLidar.h
set_time.o: ../../../include/LidarParam.h ../include/mvme162Def.h
set_time.o: ../include/sablDef.h ../include/sablGbl.h ../include/dataStruct.h
set_time.o: ../include/sablFuncs.h ../include/paraDef.h ../include/paraGbl.h
set_time.o: ../include/sloadDef.h ../include/sloadGbl.h ../include/dtoaDef.h
set_time.o: ../include/dtoaGbl.h ../include/clockDef.h ../include/clockGbl.h
set_time.o: ../include/digitDef.h ../include/digitGbl.h ../include/iruDef.h
set_time.o: ../include/iruGbl.h ../include/dispGbl.h ../include/gpsDef.h
set_time.o: ../include/gpsGbl.h ../include/interfaceDef.h
set_time.o: ../include/interfaceGbl.h ../include/motorDef.h
set_time.o: ../include/motorGbl.h ../include/relayDef.h ../include/relayGbl.h
set_time.o: ../include/laserGbl.h
start_clock.o: ../include/sablIncs.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
start_clock.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
start_clock.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
start_clock.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
start_clock.o: /vx/h/sys/types.h /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h
start_clock.o: /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
start_clock.o: /vx/h/memLib.h /vx/h/private/objLibP.h
start_clock.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/intLib.h
start_clock.o: /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h
start_clock.o: /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
start_clock.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
start_clock.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
start_clock.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
start_clock.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
start_clock.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
start_clock.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h /vx/h/sockLib.h
start_clock.o: /vx/h/sys/socket.h ../../../include/Volume.h /vx/h/rpc/rpc.h
start_clock.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
start_clock.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
start_clock.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
start_clock.o: ../../../include/CellSpacing.h ../../../include/Parameter.h
start_clock.o: ../../../include/Ray.h ../../../include/Platform.h
start_clock.o: ../../../include/LidarDesc.h ../../../include/FieldLidar.h
start_clock.o: ../../../include/LidarParam.h ../include/mvme162Def.h
start_clock.o: ../include/sablDef.h ../include/sablGbl.h
start_clock.o: ../include/dataStruct.h ../include/sablFuncs.h
start_clock.o: ../include/paraDef.h ../include/paraGbl.h
start_clock.o: ../include/sloadDef.h ../include/sloadGbl.h
start_clock.o: ../include/dtoaDef.h ../include/dtoaGbl.h
start_clock.o: ../include/clockDef.h ../include/clockGbl.h
start_clock.o: ../include/digitDef.h ../include/digitGbl.h
start_clock.o: ../include/iruDef.h ../include/iruGbl.h ../include/dispGbl.h
start_clock.o: ../include/gpsDef.h ../include/gpsGbl.h
start_clock.o: ../include/interfaceDef.h ../include/interfaceGbl.h
start_clock.o: ../include/motorDef.h ../include/motorGbl.h
start_clock.o: ../include/relayDef.h ../include/relayGbl.h
start_clock.o: ../include/laserGbl.h
send_packet.o: ../include/sablIncs.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
send_packet.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
send_packet.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
send_packet.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h
send_packet.o: /vx/h/sys/types.h /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h
send_packet.o: /vx/h/classLib.h /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h
send_packet.o: /vx/h/memLib.h /vx/h/private/objLibP.h
send_packet.o: /vx/h/private/classLibP.h /vx/h/stdarg.h /vx/h/intLib.h
send_packet.o: /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h /vx/h/qClass.h
send_packet.o: /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
send_packet.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
send_packet.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
send_packet.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h
send_packet.o: /vx/h/dllLib.h /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h
send_packet.o: /vx/h/sys/times.h /vx/h/in.h /vx/h/netinet/in.h
send_packet.o: /vx/h/systime.h /vx/h/sysLib.h /vx/h/bootLib.h /vx/h/sockLib.h
send_packet.o: /vx/h/sys/socket.h ../../../include/Volume.h /vx/h/rpc/rpc.h
send_packet.o: /vx/h/rpc/rpctypes.h /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h
send_packet.o: /vx/h/rpc/auth.h /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h
send_packet.o: /vx/h/rpc/auth_unix.h /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
send_packet.o: ../../../include/CellSpacing.h ../../../include/Parameter.h
send_packet.o: ../../../include/Ray.h ../../../include/Platform.h
send_packet.o: ../../../include/LidarDesc.h ../../../include/FieldLidar.h
send_packet.o: ../../../include/LidarParam.h ../include/mvme162Def.h
send_packet.o: ../include/sablDef.h ../include/sablGbl.h
send_packet.o: ../include/dataStruct.h ../include/sablFuncs.h
send_packet.o: ../include/paraDef.h ../include/paraGbl.h
send_packet.o: ../include/sloadDef.h ../include/sloadGbl.h
send_packet.o: ../include/dtoaDef.h ../include/dtoaGbl.h
send_packet.o: ../include/clockDef.h ../include/clockGbl.h
send_packet.o: ../include/digitDef.h ../include/digitGbl.h
send_packet.o: ../include/iruDef.h ../include/iruGbl.h ../include/dispGbl.h
send_packet.o: ../include/gpsDef.h ../include/gpsGbl.h
send_packet.o: ../include/interfaceDef.h ../include/interfaceGbl.h
send_packet.o: ../include/motorDef.h ../include/motorGbl.h
send_packet.o: ../include/relayDef.h ../include/relayGbl.h
send_packet.o: ../include/laserGbl.h
get_year.o: ../include/sablIncs.h /vx/h/vxWorks.h /vx/h/types/vxCpu.h
get_year.o: /vx/h/types/vxArch.h /vx/h/types/vxParams.h
get_year.o: /vx/h/types/vxTypesBase.h /vx/h/types/vxTypes.h
get_year.o: /vx/h/types/vxANSI.h /vx/h/types/vxTypesOld.h /vx/h/sys/types.h
get_year.o: /vx/h/math.h /vx/h/stdioLib.h /vx/h/stdio.h /vx/h/classLib.h
get_year.o: /vx/h/vwModNum.h /vx/h/objLib.h /vx/h/errno.h /vx/h/memLib.h
get_year.o: /vx/h/private/objLibP.h /vx/h/private/classLibP.h /vx/h/stdarg.h
get_year.o: /vx/h/intLib.h /vx/h/semLib.h /vx/h/taskLib.h /vx/h/qLib.h
get_year.o: /vx/h/qClass.h /vx/h/regs.h /vx/h/excLib.h /vx/h/private/eventP.h
get_year.o: /vx/h/private/semLibP.h /vx/h/stdlib.h /vx/h/tyLib.h
get_year.o: /vx/h/iosLib.h /vx/h/ioLib.h /vx/h/limits.h /vx/h/net/uio.h
get_year.o: /vx/h/fcntl.h /vx/h/sys/fcntlcom.h /vx/h/unistd.h /vx/h/dllLib.h
get_year.o: /vx/h/rngLib.h /vx/h/selectLib.h /vx/h/lstLib.h /vx/h/sys/times.h
get_year.o: /vx/h/in.h /vx/h/netinet/in.h /vx/h/systime.h /vx/h/sysLib.h
get_year.o: /vx/h/bootLib.h /vx/h/sockLib.h /vx/h/sys/socket.h
get_year.o: ../../../include/Volume.h /vx/h/rpc/rpc.h /vx/h/rpc/rpctypes.h
get_year.o: /vx/h/rpc/xdr.h /vx/h/rpc/rpctypes.h /vx/h/rpc/auth.h
get_year.o: /vx/h/rpc/clnt.h /vx/h/rpc/rpc_msg.h /vx/h/rpc/auth_unix.h
get_year.o: /vx/h/rpc/svc.h /vx/h/rpc/svc_auth.h
get_year.o: ../../../include/CellSpacing.h ../../../include/Parameter.h
get_year.o: ../../../include/Ray.h ../../../include/Platform.h
get_year.o: ../../../include/LidarDesc.h ../../../include/FieldLidar.h
get_year.o: ../../../include/LidarParam.h ../include/mvme162Def.h
get_year.o: ../include/sablDef.h ../include/sablGbl.h ../include/dataStruct.h
get_year.o: ../include/sablFuncs.h ../include/paraDef.h ../include/paraGbl.h
get_year.o: ../include/sloadDef.h ../include/sloadGbl.h ../include/dtoaDef.h
get_year.o: ../include/dtoaGbl.h ../include/clockDef.h ../include/clockGbl.h
get_year.o: ../include/digitDef.h ../include/digitGbl.h ../include/iruDef.h
get_year.o: ../include/iruGbl.h ../include/dispGbl.h ../include/gpsDef.h
get_year.o: ../include/gpsGbl.h ../include/interfaceDef.h
get_year.o: ../include/interfaceGbl.h ../include/motorDef.h
get_year.o: ../include/motorGbl.h ../include/relayDef.h ../include/relayGbl.h
get_year.o: ../include/laserGbl.h
