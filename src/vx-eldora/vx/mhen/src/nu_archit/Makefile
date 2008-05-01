#
#	Makefile for Directory mhen 
#	Author: Eric Loew
#	Date: 12/3/91
#
# GNU C 
CC= gcc68k -fno-builtin -nostdinc 
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions \
	-fomit-frame-pointer
OPT= -O2
#
DEFINES= -DUSE_ANSI -DTADPL -DNEW_DSP_CODE -DETH29 -DUSE_INT_HNDSHK
DEFINES += -DFAST_ETH29
#
COMMON = /net/projects/common
#
# Drag in includes from here.
INCLUDES= -I/. -I/net/vx/h -I/net/eldora/eldora/vx/mhen/include -I/net/eldora/eldora/dsp/include \
	  -I/net/eldora/eldora/rpc/include -I/net/misc/tcl/rpc/fft_xdr -I/net/eldora/eldora/rpc/mhen \
	  -I/net/eldora/eldora/vx/hskp/include -I/net/eldora/eldora/include -I/net/eldora/eldora/vx/vmevme_int \
	  -I/net/eldora/eldora/vx/tp41/startup -I/net/local_lnx/include/vx++ -I${COMMON}/include 
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
CXX = ${CCC}
CXXFLAGS = ${CCFLAGS}
#
LIBS=
#
HEADERS=
#
OBJS=headerHandler.o startup.o mcpl_init.o mad_xfer.o init_data_xfer.o \
         broadcast_data.o exec_dac.o relay_fft_cmds_serv.o getmandb.o \
         pow_to_counts.o coll_data_xfer.o sync.o dc_adjust.o rp7_start.o \
         fft_data_xfer.o block_dat.o data_ray_length.o
#
NU_OBJS=startup.o mcpl_init.o mad_xfer.o broadcast_data.o \
         init_data_xfer.o relay_fft_cmds_serv.o getmandb.o \
         pow_to_counts.o nu_arch_data_xfer.o sync.o rp7_start.o \
         fft_data_xfer.o block_dat.o data_ray_length.o \
	 nu_archit_dac.o quad_dmp.o col_dmp.o mcpl_recover.o
#
NU_LAB_OBJS=headerHandler.o startup.o \
         relay_fft_cmds_serv.o getmandb.o \
         pow_to_counts.o nu_arch_data_xfer.o sync.o dc_adjust.o rp7_start.o \
         fft_data_xfer.o block_dat.o \
	 nu_lab_dac.o quad_dmp.o col_dmp.o
#
ELOBJS=headerHandler.o startup.o \
	 mcpl_init.o mad_xfer.o init_data_xfer.o broadcast_data.o \
         exec_dac.o \
         relay_fft_cmds_serv.o getmandb.o pow_to_counts.o \
         coll_data_xfer.o sync.o dc_adjust.o rp7_start.o \
         /net/eldora/eldora/rpc/lib/libVxRadarRpc.a fft_data_xfer.o block_dat.o \
         /net/misc/tcl/rpc/fft_xdr/vx/fftCMDS_RPCsvc.o data_ray_length.o \
	/net/misc/tcl/rsf_libs/streamsock/vx/libstreamsock.o \
	/net/eldora/eldora/vx/bin/tp41Lib.o /net/eldora/eldora/rpc/lib/libVxHeader.a \
	-L/net/local_lnx/lib/gcc-lib/68k/current -lstdc++ -lgcc

#
TNU_OBJS=startup.o \
	 mcpl_init.o mad_xfer.o init_data_xfer.o broadcast_data.o \
         nu_archit_dac.o \
         relay_fft_cmds_serv.o getmandb.o pow_to_counts.o \
         nu_arch_data_xfer.o sync.o rp7_start.o \
         /net/eldora/eldora/rpc/lib/libVxRadarRpc.a fft_data_xfer.o block_dat.o \
         /net/misc/tcl/rpc/fft_xdr/vx/fftCMDS_RPCsvc.o data_ray_length.o \
	/net/misc/tcl/rsf_libs/streamsock/vx/libstreamsock.o \
	/net/eldora/eldora/vx/bin/tp41Lib.o quad_dmp.o col_dmp.o mcpl_recover.o \
	/net/eldora/eldora/diag/mhen/ascope.o /net/eldora/eldora/diag/mhen/fft.o \
	/net/eldora/eldora/diag/mhen/getseres.o
TNU_LAB_OBJS=startup.o \
         nu_lab_dac.o \
         relay_fft_cmds_serv.o getmandb.o pow_to_counts.o \
         nu_arch_data_xfer.o sync.o dc_adjust.o rp7_start.o \
         fft_data_xfer.o block_dat.o \
         /net/misc/tcl/rpc/fft_xdr/vx/fftCMDS_RPCsvc.o data_ray_length.o \
	/net/misc/tcl/rsf_libs/streamsock/vx/libstreamsock.o \
	/net/eldora/eldora/vx/bin/tp41Lib.o quad_dmp.o col_dmp.o -L/net/eldora/eldora/rpc/lib \
	-lVxRadarRpc -lVxHeader -L/net/local_lnx/lib/gcc-lib/68k/current \
	-lstdc++ -lgcc
#
# Include files go here.
INC=
#
SRCS=headerHandler.cc startup.cc mcpl_init.c mad_xfer.c init_data_xfer.c \
         broadcast_data.c nu_archit_dac.c relay_fft_cmds_serv.c getmandb.c \
         pow_to_counts.c nu_arch_data_xfer.c sync.c dc_adjust.c rp7_start.c \
         fft_data_xfer.c block_dat.c data_ray_length.c mcpl_recover.c
#
# Binaries go here.
BIN=/net/eldora/eldora/vx/mhen/bin

.KEEP_STATE:

compile: $(OBJS)

electra: $(OBJS)	
	ld68k -X -r -o el.o $(ELOBJS)
	install -m 774 el.o $(BIN)

nu_arch: $(NU_OBJS)
	ld68k -X -r -o nu_arch.o $(TNU_OBJS)
	install -m 774 nu_arch.o $(BIN)

lab: $(NU_LAB_OBJS)
	ld68k -X -r -o nu_lab.o $(TNU_LAB_OBJS)
	install -m 774 nu_lab.o $(BIN)

depend:	
	makedepend ${INCLUDES} ${SRCS}

update:
	-for i in $(HEADERS); \
+		do (diff $$i $(INC)/$$i >tmpdiff; \
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
