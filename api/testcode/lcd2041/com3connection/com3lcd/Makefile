CXX = arm-unknown-linux-gnu-gcc

CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		com3lcd.o

LIBS =		-lm

TARGET =	com3lcd

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

	
