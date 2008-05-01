#
#	Makefile for Directory ELDORA/diag/vmevme
#	Author: Craig Walther
#	Date: 5/12/94
#
# GNU C 
CC= gcc68k
LD= ld68k
CCC = $(CC)
INSTALL= /usr/ucb/install
#
OPT= -O2 -fvolatile
#
DEFINES=
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I. -I/ELDORA/vx/hskp/include
#
CFLAGS= $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/bin/vxheader.o
#
OBJSH= clear_int.o init_vmevme.o vmevme_menuh.o vme_isr.o
#
OBJSR= cause_int.o vmevme_menur.o init_vmevmer.o
#
SRCS= clear_int.c cause_int.c init_vmevme.c vmevme_menuh.c vmevme_menur.c\
	 vmevme_isr.c init_vmevmer.c
#
#    
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN= /ELDORA/diag/bin

diagh:	$(OBJSH)
	$(LD) -r -X -o vmevme_diagh.o $(OBJSH)
	$(INSTALL) -m 664 vmevme_diagh.o $(BIN)

diagr:	$(OBJSR)
	${LD} -r -X -o vmevme_diagr.o ${OBJSR}
	${INSTALL} -m 664 vmevme_diagr.o $(BIN)

asm:
	for i in $(SRCS); \
		do (echo "$(CC) -S $(CFLAGS) $$i";$(CC) -S $(CFLAGS) $$i); \
	done;

depend:
	makedepend ${INCLUDES} ${SRCS}

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
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,eric -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h


# DO NOT DELETE THIS LINE -- make depend depends on it.

$(OBJSH): $(INCS)
$(OBJSR): $(INCS)
