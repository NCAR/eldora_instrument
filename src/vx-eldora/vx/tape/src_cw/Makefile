#
#	Makefile for  Recording Processor Code 
#	Author:
#	Date:
#
# GNU C 
CC = gcc68k
CCC = $(CC)
LD=ld68k
#
# Totally agressive optimization.
OPT= -O -fvolatile -fno-builtin -nostdinc
#
DEFINES= 
#
# Drag in includes from here.
INCLUDES = -I/vx/h -I. -I/ELDORA/rpc/include -I/ELDORA/rpc/header\
	 -I/ELDORA/vx/c++/include -I/ELDORA/vx/tape/inc_cw\
	 -I/local/include/vx++
#
CFLAGS=	$(DEFINES) $(INCLUDES) $(OPT)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/Task.o\
	/ELDORA/vx/c++/bin/Flags.o /ELDORA/rpc/lib/libVxTapeRpc.a\
	/ELDORA/rpc/lib/libVxLogger.a -L/local/lib/gcc-lib/68k/current\
	-lgcc
#
OBJS= cpcmds.o exbcmds.o chnattn.o tstrdy.o mbox.o cipinit.o mdselinit.o mdsnsinit.o pmblkinit.o prtmdsns.o prtrqst.o prtstat.o rqstsns.o sginit.o radargthr.o navgthr.o adsgthr.o tphdr.o ctrlHandler.o startup.o flag_check.o startRPC.o drvsel.o data_ray_length.o nav_len.o adslen.o reduce.o scsi_error.o drive_init.o write_tape.o cal_sum.o fill_sg.o print_results.o read_tape.o\
sctr_gthr.o send_diags.o space_tape.o tape_menu.o tp_cpy.o print_log_sns.o
#

SRCS= cpcmds.c exbcmds.c chnattn.c tstrdy.c mbox.c cipinit.c mdselinit.c mdsnsinit.c pmblkinit.c prtmdsns.c prtrqst.c prtstat.c rqstsns.c sginit.c radargthr.c navgthr.c adsgthr.c tphdr.c startup.c ctrlHandler.cc startRPC.cc flag_check.c drvsel.c data_ray_length.c nav_len.c adslen.c reduce.c scsi_error.c drive_init.c write_tape.c cal_sum.c fill_sg.c print_results.c read_tape.c\
sctr_gthr.c send_diags.c space_tape.c tape_menu.c tp_cpy.c print_log_sns.c

#
HEADERS=
#
# Include files go here.
include /local/include/make/sun.cc.mk
#
# Binaries go here.
BIN=

.KEEP_STATE:
etape.o:	$(OBJS)
	$(LD) -Ur -X -o $@ $(OBJS) $(LIBS)
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
	for i in $(HEADERS); \
		do (echo "co $$i";co $$i); \
	done;

get_rcs_locked:
	for i in $(SRCS); \
		do (echo "co -l $$i";co -l $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;

# Modify this to include an access list if desired.
set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,reif,rich,eric -c" * " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h
startRPC.o:
	$(CCC) $(CFLAGS) -c startRPC.cc
ctrlHandler.o:
	$(CCC) $(CFLAGS) -c ctrlHandler.cc
headerHandler.o:
	$(CCC) $(CFLAGS) -c headerHandler.cc






