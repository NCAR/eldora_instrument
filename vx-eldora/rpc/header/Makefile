#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C 
CC= gcc
#
CCC= CC
#
OPT= -O2 -pipe
#
DEFINES= -DUNIX -DOK_RPC
#
INCLUDES= -I/ELDORA/rpc/include
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
XDROBJS= xdr_CellSpacing.o xdr_FieldParam.o xdr_FieldRadar.o xdr_Parameter.o \
	xdr_Platform.o xdr_RadarDesc.o xdr_Ray.o xdr_Volume.o xdr_Waveform.o \
	xdr_TapeHeader.o xdr_Gps.o xdr_IndFreq.o xdr_Ins.o xdr_Insitu.o \
	xdr_InsituData.o xdr_MiniRIMS.o xdr_NavDesc.o xdr_NavInfo.o \
	xdr_TimeSeries.o
CLNTOBJ= TapeHeaderClnt.o
HEADEROBJ= Header.o
#
SRCS= xdr_CellSpacing.c xdr_FieldParam.c xdr_FieldRadar.c xdr_Parameter.c \
	xdr_Platform.c xdr_RadarDesc.c xdr_Ray.c xdr_Volume.c xdr_Waveform.c \
	xdr_TapeHeader.c TapeHeaderSvr.c TapeHeaderClnt.c Header.cc \
	xdr_Gps.c xdr_IndFreq.c xdr_Ins.c xdr_Insitu.c \
	xdr_InsituData.c xdr_MiniRIMS.c xdr_NavDesc.c xdr_NavInfo.c \
	xdr_TimeSeries.c
#
HEADERS= CellSpacing.h FieldParam.h FieldRadar.h Parameter.h Platform.h \
	RadarDesc.h Ray.h Volume.h Waveform.h TapeHeader.h Header.h \
	Header.hh Gps.h IndFreq.h Ins.h InSitu.h \
	InSituData.h MiniRIMS.h NavDesc.h NavInfo.h \
	TimeSeries.h
#
INCDIR= /ELDORA/rpc/include
#
BINDIR= /ELDORA/rpc/bin
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

libHeader.a: $(XDROBJS) $(CLNTOBJ) $(HEADEROBJ)
	ar crv $@ $(XDROBJS) $(CLNTOBJ) $(HEADEROBJ)
	cp $@ $(BINDIR)
	ranlib $(BINDIR)/$@

vx68k:
	(cd 68k; $(MAKE))

update_only:
	-for i in $(HEADERS); \
		do (echo "diffing $$i"; diff $$i $(INCDIR)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i"; cp $$i $(INCDIR)) \
			fi;); \
	done;
	-rm tmpdiff

$(HEADEROBJ): Header.cc
	$(CCC) $(CFLAGS) -DUNIX -c Header.cc

al4: al.c
	$(CC) $(CFLAGS) -s -o $@ al.c

al3: al.c
	$(CC) $(CFLAGS) -s -o $@ al.c

tstit: libHeader.a tst.o
	$(CCC) -s -o $@ tst.o -L../bin -lHeader

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

TapeHeaderClnt.o: TapeHeader.h /usr/lang/SC1.0/include/CC/rpc/rpc.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/types.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/c_varieties.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/sys/types.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/sys/time.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/sys/time.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/malloc.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/alloca.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/netinet/in.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/stdio.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
TapeHeaderClnt.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h CellSpacing.h
TapeHeaderClnt.o: FieldRadar.h Parameter.h RadarDesc.h Volume.h Waveform.h
TapeHeaderClnt.o: InSitu.h NavDesc.h
TapeHeaderSvr.o: vxWorks.h stdioLib.h TapeHeader.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/types.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/c_varieties.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/sys/types.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/sys/time.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/sys/time.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/malloc.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/alloca.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/netinet/in.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/stdio.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
TapeHeaderSvr.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h CellSpacing.h
TapeHeaderSvr.o: FieldRadar.h Parameter.h RadarDesc.h Volume.h Waveform.h
TapeHeaderSvr.o: InSitu.h NavDesc.h
al.o: /usr/lang/SC1.0/include/CC/stdio.h
al.o: /usr/lang/SC1.0/include/CC/c_varieties.h
al.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
al.o: /usr/lang/SC1.0/include/CC/rpc/types.h
al.o: /usr/lang/SC1.0/include/CC/sys/types.h
al.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
al.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
al.o: /usr/lang/SC1.0/include/CC/sys/time.h
al.o: /usr/lang/SC1.0/include/CC/sys/time.h
al.o: /usr/lang/SC1.0/include/CC/malloc.h /usr/lang/SC1.0/include/CC/alloca.h
al.o: /usr/lang/SC1.0/include/CC/netinet/in.h
al.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
al.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
al.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
al.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
al.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
al.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
al.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
al.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h CellSpacing.h FieldParam.h
al.o: FieldRadar.h Parameter.h Platform.h RadarDesc.h Ray.h Volume.h
al.o: Waveform.h TapeHeader.h InSitu.h NavDesc.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_CellSpacing.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h CellSpacing.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_FieldParam.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h FieldParam.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_FieldRadar.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h FieldRadar.h
xdr_Gps.o: Gps.h
xdr_IndFreq.o: IndFreq.h
xdr_Ins.o: Ins.h
xdr_Insitu.o: InSitu.h /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_Insitu.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h
xdr_InsituData.o: InSituData.h
xdr_MiniRIMS.o: MiniRIMS.h
xdr_NavDesc.o: NavDesc.h /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_NavDesc.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h
xdr_NavInfo.o: NavInfo.h
xdr_Parameter.o: Parameter.h /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_Parameter.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_Platform.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h Platform.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_RadarDesc.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h RadarDesc.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_Ray.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h Ray.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_TapeHeader.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h TapeHeader.h
xdr_TapeHeader.o: CellSpacing.h FieldRadar.h Parameter.h RadarDesc.h Volume.h
xdr_TapeHeader.o: Waveform.h InSitu.h NavDesc.h
xdr_TimeSeries.o: TimeSeries.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_Volume.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h Volume.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/types.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/c_varieties.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/sys/types.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/sys/time.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/malloc.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/alloca.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/netinet/in.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/stdio.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
xdr_Waveform.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h Waveform.h
Header.o: Header.hh Header.h TapeHeader.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/types.h
Header.o: /usr/lang/SC1.0/include/CC/c_varieties.h
Header.o: /usr/lang/SC1.0/include/CC/sys/types.h
Header.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
Header.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
Header.o: /usr/lang/SC1.0/include/CC/sys/time.h
Header.o: /usr/lang/SC1.0/include/CC/sys/time.h
Header.o: /usr/lang/SC1.0/include/CC/malloc.h
Header.o: /usr/lang/SC1.0/include/CC/alloca.h
Header.o: /usr/lang/SC1.0/include/CC/netinet/in.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
Header.o: /usr/lang/SC1.0/include/CC/stdio.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
Header.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h CellSpacing.h
Header.o: FieldRadar.h Parameter.h RadarDesc.h Volume.h Waveform.h InSitu.h
Header.o: NavDesc.h /usr/lang/SC1.0/include/CC/signal.h
Header.o: /usr/lang/SC1.0/include/CC/sys/signal.h
Header.o: /usr/lang/SC1.0/include/CC/vm/faultcode.h
Header.o: /usr/lang/SC1.0/include/CC/sys/unistd.h
Header.o: /usr/lang/SC1.0/include/CC/string.h
Header.o: /usr/lang/SC1.0/include/CC/memory.h
tst.o: /usr/lang/SC1.0/include/CC/stream.h
tst.o: /usr/lang/SC1.0/include/CC/iostream.h
tst.o: /usr/lang/SC1.0/include/CC/memory.h
tst.o: /usr/lang/SC1.0/include/CC/c_varieties.h
tst.o: /usr/lang/SC1.0/include/CC/iomanip.h
tst.o: /usr/lang/SC1.0/include/CC/generic.h
tst.o: /usr/lang/SC1.0/include/CC/stdiostream.h
tst.o: /usr/lang/SC1.0/include/CC/stdio.h
tst.o: /usr/lang/SC1.0/include/CC/fstream.h Header.hh Header.h TapeHeader.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/rpc.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/types.h
tst.o: /usr/lang/SC1.0/include/CC/sys/types.h
tst.o: /usr/lang/SC1.0/include/CC/sys/stdtypes.h
tst.o: /usr/lang/SC1.0/include/CC/sys/sysmacros.h
tst.o: /usr/lang/SC1.0/include/CC/sys/time.h
tst.o: /usr/lang/SC1.0/include/CC/sys/time.h
tst.o: /usr/lang/SC1.0/include/CC/malloc.h
tst.o: /usr/lang/SC1.0/include/CC/alloca.h
tst.o: /usr/lang/SC1.0/include/CC/netinet/in.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/xdr.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/auth.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/clnt.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/rpc_msg.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/auth_unix.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/auth_des.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/svc.h
tst.o: /usr/lang/SC1.0/include/CC/rpc/svc_auth.h CellSpacing.h FieldRadar.h
tst.o: Parameter.h RadarDesc.h Volume.h Waveform.h InSitu.h NavDesc.h
