#
#	Makefile for 
#	Author:
#	Date:
#
# 
CCC= gcc
#
OPT= -O2
#
#DEFINES=
#
#INCLUDES=
#
CCFLAGS= $(OPT) # $(DEFINES) $(INCLUDES)
#
OBJS= xdr_Hpa.o HpaRpcClnt.o HpaRpcSvr.o
#
SRCS= xdr_Hpa.cc HpaRpcClnt.cc HpaRpcSvr.cc
#
HEADERS= HpaRpc.hh
#
BIN= ../bin
#
INC= ../include

.KEEP_STATE:

.MAIN: all

all: obj libHpaRpc.a
	cp libHpaRpc.a $(BIN)
	ranlib -t $(BIN)/libHpaRpc.a

obj: $(OBJS)

libHpaRpc.a: obj
	ar crv $@ $(OBJS)
	ranlib $@

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
		do (echo "$(CCC) -S $(CCFLAGS) $$i";$(CCC) -S $(CCFLAGS) $$i); \
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

depend:
	makedepend -I/usr/lang/SC1.0/include/CC $(SRCS)
# DO NOT DELETE THIS LINE -- make depend depends on it.

xdr_Hpa.o: HpaRpc.hh
HpaRpcClnt.o: HpaRpc.hh
HpaRpcSvr.o: HpaRpc.hh /usr/lang/SC1.0/include/CC/stream.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/iostream.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/memory.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/c_varieties.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/iomanip.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/generic.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/stdiostream.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/stdio.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/fstream.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/rpc/pmap_clnt.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/rpc/types.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/sys/types.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/sys/time.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/sys/time.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/malloc.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/alloca.h
HpaRpcSvr.o: /usr/lang/SC1.0/include/CC/netinet/in.h
