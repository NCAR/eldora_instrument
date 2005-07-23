#
#	Makefile for Clock class
#	Author: R. Neitzel
#	Date: 1 May 91
#
# GNU C/C++
CXX= gcc68k
#
OPT= -O2 -m68040
#
#
DEFINES=
#
INCLUDES= -I/vw/h -I../include -I/ELDORA/rpc/include -I/ELDORA/include
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= Clock.o Date.o
#
SRCS= Clock.cc Date.cc
#
HEADERS= Clock.hh ClockP.hh Date.hh
#
.KEEP_STATE:

.MAIN: objs

objs: $(OBJS)
	cp $(OBJS) ../bin
	-for i in $(HEADERS); \
		do (diff $$i ../include/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i ../include) \
			fi;); \
	done;
	rm -f tmpdiff

depend:
	makedepend $(INCLUDES) $(SRCS)

tst: $(OBJS) clktst.o

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

put_rcs_new:
	ci -f2.0 $(SRCS) $(HEADERS)

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

Clock.o: Clock.hh ../include/Window.hh ../include/GraphicController.hh
Clock.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
Clock.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
Clock.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
Clock.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h ../include/point.h
Clock.o: ../include/qpdm.h ../include/WdwGbls.h
Clock.o: ../include/GraphicControllerP.hh ClockP.hh
Date.o: Date.hh ../include/Window.hh ../include/GraphicController.hh
Date.o: /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
Date.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
Date.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
Date.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h ../include/point.h
Date.o: ../include/qpdm.h ../include/WdwGbls.h
Date.o: ../include/GraphicControllerP.hh /vw/h/string.h /vw/h/stdioLib.h
Date.o: /vw/h/stdio.h /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h
Date.o: /vw/h/memLib.h /vw/h/private/objLibP.h
