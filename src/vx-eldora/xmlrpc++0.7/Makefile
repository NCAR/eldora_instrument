# makefile written for gnu make
#CXX		= g++
CXX             = /opt/local/m68k-wrs-vxworks/bin/m68k-wrs-vxworks-g++
#RANLIB         = /bin/true
RANLIB          = /opt/local/m68k-wrs-vxworks/bin/m68k-wrs-vxworks-ranlib
SRC		= ./src
CPPFLAGS	= -I$(SRC)
DEBUG		= -g
OPTIMIZE	= -O2
GCCWARN		= -Wall -Wstrict-prototypes
CXXFLAGS	= $(DEBUG) $(GCCWARN) $(OPTIMIZE) $(INCLUDES)
# The ld "Build global constructor/destructor tables" flag (-Ur) is 
# absolutely required for VxWorks. The -S and -X flags to ld just give us
# a smaller final product by removing some unnecesary symbols.
LDFLAGS         = -Wl,-Ur -Wl,-S -Wl,-X

LIB		= ./libXmlRpc.a

# Add your system-dependent network libs here. These are
# only used to build the tests (your application will need them too).
# Linux: none
# Solaris: -lsocket -lnsl
#SYSTEMLIBS	= -lsocket -lnsl
SYSTEMLIBS	=
LDLIBS		= $(LIB) $(SYSTEMLIBS)

OBJ		= $(SRC)/XmlRpcClient.o $(SRC)/XmlRpcDispatch.o \
		$(SRC)/XmlRpcServer.o $(SRC)/XmlRpcServerConnection.o \
		$(SRC)/XmlRpcServerMethod.o $(SRC)/XmlRpcSocket.o $(SRC)/XmlRpcSource.o \
		$(SRC)/XmlRpcUtil.o $(SRC)/XmlRpcValue.o

all:		$(LIB) tests

$(LIB):		$(OBJ)
		$(AR) $(ARFLAGS) $(LIB) $(OBJ)
		$(RANLIB) $(LIB)


tests:		$(LIB)
		cd test && $(MAKE) CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" SYSTEMLIBS="$(SYSTEMLIBS)"

doc doxygen:
		cd src && doxygen Doxyfile

clean:
		rm -f $(SRC)/*.o
		rm -f $(SRC)/*~
		rm -f $(LIB)
		cd test && $(MAKE) clean

