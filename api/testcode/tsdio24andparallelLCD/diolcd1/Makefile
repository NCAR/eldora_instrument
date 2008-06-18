CXX = arm-unknown-linux-gnu-gcc

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		diolcd1.o

LIBS =		-lm

TARGET =	diolcd1

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

	
