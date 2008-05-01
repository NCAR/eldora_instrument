#
#       Makefile for Directory ELDORA/vx/hskp/src/minirims/sandia
#       Author: Craig Walther
#       Date: 8/19/92
#
# GNU C 
CC= gcc68k
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
#       -fomit-frame-pointer
#OPT= -O -fvolatile -fno-builtin -nostdinc -m68040
OPT= -O -fvolatile -fno-builtin -nostdinc
#
DEFINES= -DUSE_ANSI -DMV133 -DVxWorks
#
# Drag in includes from here.
INCLUDES= -I/vw/h -I. -I/ELDORA/rpc/header -I/ELDORA/vx/minirims/include\
        -I/ELDORA/vx/c++/include -I/ELDORA/rpc/include -I/ELDORA/vx/c/include \
	-I/ELDORA/vx/tp41
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a \
        /ELDORA/vx/c/lib/libcurses.a \
        /ELDORA/vx/c/lib/libtermlib.a
#

.KEEP_STATE:

default: navtask

OBJS = init_har.o align.o navrot.o rmrdcalc.o align_v.o euler.o acomp.o arctan.o block_cr.o crc_task.o dircos.o erthal.o erthnav.o gcomp.o gravity.o i_align.o init_nav.o init_sof.o modechg.o navvars.o nealt.o ortho.o s_align.o tstrims.o updcon.o c_align.o fltseq_3.o navtask.o rd_minir.o

SRCS = init_har.c align.c navrot.c rmrdcalc.c align_v.c euler.c acomp.c arctan.c block_cr.c crc_task.c dircos.c erthal.c erthnav.c gcomp.c gravity.c i_align.c init_nav.c init_sof.c modechg.c navvars.c nealt.c ortho.c s_align.c tstrims.c updcon.c c_align.c fltseq_3.c navtask.c rd_minir.c

navtask: $(OBJS)
	ld68k -X -r -o snav.o $(OBJS)





