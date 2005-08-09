#
#	Makefile for Directory mhen 
#	Author: Eric Loew
#	Date: 12/3/91
#
# GNU C 
CC= gcc68
CCC = $(CC)
#
# Totally agressive optimization.
OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
	-fomit-frame-pointer
#
DEFINES= -DUSE_ANSI
#
ELDORA = /net/eldora/eldora
#
COMM_DIR = /net/projects
#
LIBS= -L/usr/local/lib/gcc-lib/m68k-wrs-vxworks/current -L${ELDORA}/rpc/lib
LIBS += -lVxHeader -L${COMM_DIR}/common/lib/vxworks -lNet 
LIBS += -L/local/lib/gcc-lib/m68k-wrs-vxworks/current -lstdc++ -lgcc
#
HEADERS=
#
OBJS=el.o dspq.o coll.o ad.o timmod.o
#
LAB_OBJS=lab.o coll.o ad.o timmod.o
#
TS_OBJS=ts.o lab_dspq.o coll.o ad.o timmod.o
#
NU_ARCH_OBJS=nu_arch.o dspq.o coll.o ad.o timmod.o eth29.o /net/eldora/eldora/vx/vmevme_int/getInt.o
#
SPOL_OBJS=nu_arch.o spol_dspq.o coll.o spol_ad.o timmod.o
# Binaries go here.
BIN=/ELDORA/vx/mhen/tadpole/bin

.KEEP_STATE:

electra: 
	ld68k -X -Ur -o elbig.o $(OBJS) ${LIBS}
lab: 
	ld68k -X -r -o blab.o $(LAB_OBJS) ${LIBS}
ts: 
	ld68k -X -r -o bts.o $(TS_OBJS)
nu_arch: 
	ld68k -X -r -o bnu_arch.o $(NU_ARCH_OBJS) ${LIBS}
spol: 
	ld68k -X -r -o spol.o $(SPOL_OBJS)
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


nu_arch_install:
	scp bnu_arch.o eldora@dyn78-248:/vxbin/radar

# Modify this to include an access list if desired.
set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,shawn,rich,eric,reif -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






