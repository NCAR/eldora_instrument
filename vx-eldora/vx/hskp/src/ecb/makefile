#
#	Makefile for Directory ELDORA/vx/hskp/src/ecb
#	Author: Craig Walther
#	Date: 8/19/92
#
# GNU C 
CC= gcc68
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
#	-fomit-frame-pointer
OPT= -O -fvolatile
#
DEFINES= -DUSE_ANSI
#
# Drag in includes from here.
INCLUDES= -I/vw/h -I. -I/ELDORA/rpc/header2 -I/ELDORA/vx/hskp/include\
		-I/ELDORA/vx/c++/include
#
CFLAGS= $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/bin/vxheader.o /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS= calcs.o ecbClrFifos.o ecbCmpltIsr.o ecbGetBusStat.o ecbGetTsami.o\
	ecbGetTsamr.o ecbGiveSem.o ecbIntInit.o ecbMenu.o ecbSetAtten.o\
	ecbSetDDS.o ecbSetDDSfloat.o ecbSetDivN.o ecbSetIF.o ecbSetMux.o\
	ecbShowBim.o ecbShowBusStat.o ecbShowLast.o ecbShowMstat.o\
	ecbShowTempi.o ecbShowTempr.o fifotst1.o
#
SRCS= calcs.c ecbClrFifos.c ecbCmpltIsr.c ecbGetBusStat.c ecbGetTsami.c\
	ecbGetTsamr.c ecbGiveSem.c ecbIntInit.c ecbMenu.c ecbSetAtten.c\
	ecbSetDDS.c ecbSetDDSfloat.c ecbSetDivN.c ecbSetIF.c ecbSetMux.c\
	ecbShowBim.c ecbShowBusStat.c ecbShowLast.c ecbShowMstat.c\
	ecbShowTempi.c ecbShowTempr.c fifotst1.c
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
	 install -m 774 $(OBJS) $(BIN)

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


