#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C 
CC= gcc
CXX = ${CC}
GCC= gcc68k
LD68 = ld68k
#
# Totally agressive optimization.
OPT= -O2
#
DEFINES= -DUNIX
#
COMMON= /net/projects/common
#
INCLUDES= -I${COMMON}/include
#
# Drag in includes from here.
VXINCLUDES= -I/vx/h -I/local/include/vx++ -nostdinc -fno-builtin ${INCLUDES}
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CXXFLAGS= ${CFLAGS}
#
LDFLAGS= -s
#
LIBS= -L${COMMON}/lib/linux -lNet
#
PROGRAM= Logger
UOBJS= Logger.o
VXOBJS= loggerClnt.o
#
SRCS= loggerClnt.cc Logger.cc
#
HEADERS= logger.h
#
# Include files go here.
INC= /ELDORA/rpc/include
#
# Binaries go here.
BIN= ../lib
BIN2=

.KEEP_STATE:

all: Logger libVxLogger.a update

Logger: $(UOBJS)
	$(CC) $(LDFLAGS) -o $@ $(UOBJS) ${LIBS} -lstdc++

libVxLogger.a: $(VXOBJS)
	ar68k crv $@ ${VXOBJS}
	cp $@ ${BIN}
	ranlib68k ${BIN}/$@

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;

loggerClnt.o:
	$(GCC) $(OPT) $(VXINCLUDES) -c loggerClnt.cc

vxtst.o:
	$(GCC) $(OPT) $(VXINCLUDES) -c vxtst.c

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
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






