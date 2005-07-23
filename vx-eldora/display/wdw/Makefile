#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++
CXX= gcc68k
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
OPT= -O2 -m68040
#
DEFINES=
#
CC_INC= /ELDORA/vx/c++/include
#
INCLUDES= -I/vw/h -I$(CC_INC) -I../include -I/local/include/vx++
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
# Controller files.
GOBJS= GraphicController.o GraphicShift.o GraphicLines.o GraphicRects.o \
	GraphicCircles.o GraphicColors.o GraphicWindow.o GraphicTri.o \
	AgcQpdm.o
#
# Mouse files.
MOBJS= Mouse.o
#
# Window files.
WOBJS= Window.o WindowGraphics.o WindowText.o
#
#
OBJS= $(MOBJS) $(GOBJS) $(WOBJS)
#
GSRCS= GraphicController.cc GraphicShift.cc GraphicLines.cc GraphicRects.cc \
	GraphicCircles.cc GraphicColors.cc GraphicWindow.cc GraphicTri.cc \
	AgcQpdm.cc
#
MSRCS= Mouse.cc 
#
WSRCS= Window.cc WindowGraphics.cc WindowText.cc
#
SRCS= $(GSRCS) $(MSRCS) $(WSRCS)
#
GHDRS= GraphicController.hh GraphicControllerP.hh qpdm.h default_colors.h
#
MHDRS= Mouse.hh MouseP.hh
#
WHDRS= Window.hh WindowP.hh mitchFont.h
#
HEADERS= $(GHDRS) $(WHDRS) $(MHDRS)
#
#
.KEEP_STATE:

libGraphics.a: $(OBJS)
	$(AR) crv $@ $(OBJS)
	cp $@ ../lib
	$(RANLIB) ../lib/$@
	-for i in $(HEADERS); \
		do (diff $$i ../include/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i ../include) \
			fi;); \
	rm tmpdiff; \
	done;

depend:
	makedepend $(INCLUDES) $(SRCS)

asm:
	for i in $(SRCS); \
		do (echo "$(CCC) -S $(CCFLAGS) $$i";$(CCC) -S $(CCFLAGS) $$i);\
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
	-for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;
	-for i in $(HEADERS); \
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

GraphicController.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicController.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicController.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicController.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
GraphicController.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h
GraphicController.o: ../include/point.h qpdm.h ../include/WdwGbls.h
GraphicController.o: GraphicControllerP.hh /vw/h/taskLib.h /vw/h/classLib.h
GraphicController.o: /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h /vw/h/qLib.h
GraphicController.o: /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
GraphicController.o: /vw/h/private/semLibP.h /vw/h/private/classLibP.h
GraphicController.o: /vw/h/private/objLibP.h /vw/h/stdlib.h
GraphicShift.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicShift.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicShift.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicShift.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
GraphicShift.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h
GraphicShift.o: ../include/point.h qpdm.h ../include/WdwGbls.h
GraphicShift.o: GraphicControllerP.hh
GraphicLines.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicLines.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicLines.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicLines.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
GraphicLines.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h
GraphicLines.o: ../include/point.h qpdm.h ../include/WdwGbls.h
GraphicLines.o: GraphicControllerP.hh
GraphicRects.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicRects.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicRects.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicRects.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
GraphicRects.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h
GraphicRects.o: ../include/point.h qpdm.h ../include/WdwGbls.h
GraphicRects.o: GraphicControllerP.hh /vw/h/string.h
GraphicCircles.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicCircles.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicCircles.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicCircles.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
GraphicCircles.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h
GraphicCircles.o: ../include/point.h qpdm.h ../include/WdwGbls.h
GraphicCircles.o: GraphicControllerP.hh
GraphicColors.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicColors.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicColors.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicColors.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
GraphicColors.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h
GraphicColors.o: ../include/point.h qpdm.h ../include/WdwGbls.h
GraphicColors.o: GraphicControllerP.hh default_colors.h
GraphicWindow.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicWindow.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicWindow.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicWindow.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
GraphicWindow.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h
GraphicWindow.o: ../include/point.h qpdm.h ../include/WdwGbls.h
GraphicWindow.o: GraphicControllerP.hh
GraphicTri.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
GraphicTri.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
GraphicTri.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
GraphicTri.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
GraphicTri.o: /vw/h/semLib.h /vw/h/vwModNum.h ../include/point.h qpdm.h
GraphicTri.o: ../include/WdwGbls.h GraphicControllerP.hh
AgcQpdm.o: GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
AgcQpdm.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
AgcQpdm.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
AgcQpdm.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
AgcQpdm.o: /vw/h/semLib.h /vw/h/vwModNum.h ../include/point.h qpdm.h
AgcQpdm.o: ../include/WdwGbls.h GraphicControllerP.hh /vw/h/taskLib.h
AgcQpdm.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
AgcQpdm.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
AgcQpdm.o: /vw/h/private/semLibP.h /vw/h/private/classLibP.h
AgcQpdm.o: /vw/h/private/objLibP.h /vw/h/stdlib.h
Mouse.o: Mouse.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
Mouse.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
Mouse.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h
Mouse.o: /vw/h/sys/types.h /vw/h/semLib.h /vw/h/vwModNum.h /vw/h/taskLib.h
Mouse.o: /vw/h/classLib.h /vw/h/objLib.h /vw/h/errno.h /vw/h/memLib.h
Mouse.o: /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h /vw/h/excLib.h
Mouse.o: /vw/h/private/semLibP.h /vw/h/private/classLibP.h
Mouse.o: /vw/h/private/objLibP.h /vw/h/stdlib.h ../include/point.h
Mouse.o: /ELDORA/vx/c++/include/Isr.hh /vw/h/intLib.h MouseP.hh
Mouse.o: /vw/h/string.h /vw/h/tickLib.h
Window.o: Window.hh GraphicController.hh /vw/h/vxWorks.h /vw/h/types/vxCpu.h
Window.o: /vw/h/types/vxArch.h /vw/h/types/vxParams.h
Window.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
Window.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
Window.o: /vw/h/semLib.h /vw/h/vwModNum.h ../include/point.h qpdm.h
Window.o: ../include/WdwGbls.h GraphicControllerP.hh WindowP.hh
Window.o: /vw/h/stdioLib.h /vw/h/stdio.h /vw/h/classLib.h /vw/h/objLib.h
Window.o: /vw/h/errno.h /vw/h/memLib.h /vw/h/private/objLibP.h
Window.o: /vw/h/taskLib.h /vw/h/qLib.h /vw/h/qClass.h /vw/h/regs.h
Window.o: /vw/h/excLib.h /vw/h/private/semLibP.h /vw/h/private/classLibP.h
Window.o: /vw/h/stdlib.h mitchFont.h
WindowGraphics.o: Window.hh GraphicController.hh /vw/h/vxWorks.h
WindowGraphics.o: /vw/h/types/vxCpu.h /vw/h/types/vxArch.h
WindowGraphics.o: /vw/h/types/vxParams.h /vw/h/types/vxTypesBase.h
WindowGraphics.o: /vw/h/types/vxTypes.h /vw/h/types/vxANSI.h
WindowGraphics.o: /vw/h/types/vxTypesOld.h /vw/h/sys/types.h /vw/h/semLib.h
WindowGraphics.o: /vw/h/vwModNum.h ../include/point.h qpdm.h
WindowGraphics.o: ../include/WdwGbls.h GraphicControllerP.hh
WindowText.o: Window.hh GraphicController.hh /vw/h/vxWorks.h
WindowText.o: /vw/h/types/vxCpu.h /vw/h/types/vxArch.h /vw/h/types/vxParams.h
WindowText.o: /vw/h/types/vxTypesBase.h /vw/h/types/vxTypes.h
WindowText.o: /vw/h/types/vxANSI.h /vw/h/types/vxTypesOld.h /vw/h/sys/types.h
WindowText.o: /vw/h/semLib.h /vw/h/vwModNum.h ../include/point.h qpdm.h
WindowText.o: ../include/WdwGbls.h GraphicControllerP.hh /vw/h/strLib.h
WindowText.o: /vw/h/string.h
