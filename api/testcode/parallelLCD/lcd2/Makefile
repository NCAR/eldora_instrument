CXX = arm-unknown-linux-gnu-gcc
#CXX = gcc

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		lcd2.o

LIBS =		-lm

TARGET =	lcd2

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

	
