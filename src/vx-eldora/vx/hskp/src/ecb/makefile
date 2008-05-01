#
#	Makefile for Directory ELDORA/vx/hskp/src/ecb
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
INCLUDES= -I/vx/h -I. -I/ELDORA/rpc/header -I/ELDORA/vx/hskp/include\
		-I/ELDORA/vx/c++/include -I/ELDORA/rpc/hskp
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS= ecbClrFifos.o ecbCmpltIsr.o ecbGetBusStat.o ecbGetTsami.o\
	ecbGetTsamr.o ecbGiveSem.o ecbIntInit.o ecb_menu.o ecbSetAtten.o\
	ecbSetDDS.o ecbSetDDSfloat.o ecbSetDivN.o ecbSetIF.o ecbSetMux.o\
	ecbShowBim.o ecbShowBusStat.o ecbShowLast.o ecbShowMstat.o\
	ecbShowTempi.o ecbShowTempr.o fifotst1.o ldsrec.o dpclr.o\
	stop11.o go11.o ecbErrorIsr.o ecbGetDDSool.o ecbShowDDSool.o\
	ecbShowRpc.o
#
SRCS= ecbClrFifos.c ecbCmpltIsr.c ecbGetBusStat.c ecbGetTsami.c\
	ecbGetTsamr.c ecbGiveSem.c ecbIntInit.c ecb_menu.c ecbSetAtten.c\
	ecbSetDDS.c ecbSetDDSfloat.c ecbSetDivN.c ecbSetIF.c ecbSetMux.c\
	ecbShowBim.c ecbShowBusStat.c ecbShowLast.c ecbShowMstat.c\
	ecbShowTempi.c ecbShowTempr.c fifotst1.c ldsrec.c dpclr.c\
	stop11.c go11.c ecbErrorIsr.c ecbGetDDSool.c ecbShowDDSool.c\
	ecbShowRpc.c

GETOBJS  = ecbGetBusStat.o ecbGetTsami.o ecbGetTsamr.o ecbGetDDSool.o
#
INTOBJS  = ecbCmpltIsr.o ecbIntInit.o ecbErrorIsr.o
#
SETOBJS  = ecbSetAtten.o ecbSetDDS.o ecbSetDDSfloat.o ecbSetDivN.o\
		ecbSetIF.o ecbSetMux.o 
#
SHOWOBJS = ecbShowBim.o ecbShowBusStat.o ecbShowLast.o ecbShowMstat.o\
		ecbShowTempi.o ecbShowTempr.o ecbShowDDSool.o\
		ecbShowRpc.o
#
UTILOBJS = ecbClrFifos.o ecbGiveSem.o ecb_menu.o fifotst1.o
#
MSTROBJS = ldsrec.o dpclr.o stop11.o go11.o
#
FINALOBJS =  ecbGets.o ecbInts.o ecbSets.o ecbShows.o ecbUtils.o mstrFuncs.o
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
	ld68k -X -r -o ecbGets.o ${GETOBJS}
	ld68k -X -r -o ecbInts.o ${INTOBJS}
	ld68k -X -r -o ecbSets.o ${SETOBJS}
	ld68k -X -r -o ecbShows.o ${SHOWOBJS}
	ld68k -X -r -o ecbUtils.o ${UTILOBJS}
	ld68k -X -r -o mstrFuncs.o ${MSTROBJS}

#	 bsdinst -m 774 $(FINALOBJS) $(BIN)

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
		do (echo "rcs -c\" * \" -L $$i";rcs -aericloew,granger,martinc -c" * " -L $$i); \
	done;

copyem:
	cp ecbGets.o /ELDORA/vx/hskp/bin/.
	cp ecbInts.o /ELDORA/vx/hskp/bin/.
	cp ecbSets.o /ELDORA/vx/hskp/bin/.
	cp ecbShows.o /ELDORA/vx/hskp/bin/.
	cp ecbUtils.o /ELDORA/vx/hskp/bin/.
	cp mstrFuncs.o /ELDORA/vx/hskp/bin/.

clean:
	rm -f *.o *~ core
