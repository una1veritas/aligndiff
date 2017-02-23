CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		aligndiff.o cutils.o

LIBS =

TARGET =	aligndiff

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
