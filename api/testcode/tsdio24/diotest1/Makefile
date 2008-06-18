CXX = arm-unknown-linux-gnu-gcc

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		diotest1.o

LIBS =		-lm

TARGET =	diotest1

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

	
