CXX = arm-unknown-linux-gnu-gcc

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		com23lcd2.o

LIBS =		-lm

TARGET =	com23lcd2

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

	
