#
#	Makefile for  ELDORA Recording Processor Specific Code 
#	Author: Craig Walther
#	Date: 06/17/96
#
# GNU C 
CC = gcc68k
CCC = $(CC)
LD=ld68k
#
# Totally agressive optimization.
OPT= -O -fvolatile -fno-builtin -nostdinc -m68020
#
DEFINES= 
#
COMMON = /net/projects/common
# Drag in includes from here.
INCLUDES = -I/vx/h -I. -I/ELDORA/rpc/include -I/ELDORA/rpc/header\
	 -I/ELDORA/vx/c++/include -I/ELDORA/vx/tape/inc_rec\
	 -I/ELDORA/vx/tape/inc_dlt -I/local/include/vx++ \
	 -I${COMMON}/include
#
CFLAGS=	$(DEFINES) $(INCLUDES) $(OPT)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/Task.o\
	/ELDORA/vx/c++/bin/Flags.o /ELDORA/rpc/lib/libVxTapeRpc.a\
	/ELDORA/rpc/lib/libVxLogger.a -L/local/lib/gcc-lib/68k/current\
	-lgcc
#
OBJS= flag_check.o startRPC.o mail_box.o startup.o\
drv_sel.o data_ray_length.o nav_length.o ads_length.o reduce.o cal_sum.o\
tape_header.o
#

SRCS= flag_check.c startRPC.cc mail_box.c startup.c\
drv_sel.c data_ray_length.c nav_length.c ads_length.c reduce.c cal_sum.c\
tape_header.c
#
HEADERS=
#
# Include files go here.
CXX = ${CC}
CXXFLAGS=CFLAGS
#
# Binaries go here.
BIN=

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
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,rich,eric -c" * " -L $$i); \
	done;

copyem:
	cp *.o /ELDORA/vx/tape/bin/.

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






