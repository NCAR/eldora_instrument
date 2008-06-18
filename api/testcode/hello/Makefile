#CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -mcpu=strongarm
CXX = arm-unknown-linux-gnu-gcc
#CXX = gcc
CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 
OBJS =		hello.o

LIBS = -lm

TARGET =	hello

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
