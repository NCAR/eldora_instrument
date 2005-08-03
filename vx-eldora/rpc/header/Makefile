#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CCC= /usr/bin/c++
CC= ${CCC}
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
#
# This where the ELDORA tree is rooted.
ELDORA= /ELDORA
#
OPT= -O2 -g
#
DEFINES= -DUNIX
#
INCLUDES= -I${ELDORA}/rpc/include
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS=${CFLAGS}
#
LIBS=
#
CXX = ${CC}
CXXFLAGS = ${CCFLAGS}
#
HEADEROBJ= Header.o
#
SRCS= Header.cc 
#
HEADERS= CellSpacing.h FieldParam.h FieldRadar.h Parameter.h Platform.h \
	RadarDesc.h Ray.h Volume.h Waveform.h TapeHeader.h Header.h \
	Header.hh Gps.h IndFreq.h Ins.h InSitu.h \
	InSituData.h MiniRIMS.h NavDesc.h NavInfo.h \
	TimeSeries.h Tsacq.h
#
INCDIR= ${ELDORA}/rpc/include
#
BINDIR= ${ELDORA}/rpc/lib
#
.KEEP_STATE:

all: vx68k libHeader.a
	-for i in $(HEADERS); \
		do (diff $$i $(INCDIR)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i"; cp $$i $(INCDIR)) \
			fi;); \
	done;
	-rm tmpdiff

libHeader.a: $(HEADEROBJ)
	ar crv $@ $(HEADEROBJ)
	cp $@ $(BINDIR)

vx68k: 


depend:
	makedepend ${DEFINES} $(INCLUDES) -I/local/lib/gcc-lib/sparc-sun-solaris2.3/2.6.0/include -I/local/lib/g++-include $(SRCS)

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


# DO NOT DELETE THIS LINE -- make depend depends on it.
