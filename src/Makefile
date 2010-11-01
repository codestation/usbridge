CXX := g++
TARGET = usbridge
LIBS := -lpcap -lpthread -lusb-1.0
INCLUDES := -I /usr/include/libusb-1.0
LIB_PATH :=
CFLAGS := -O2 -pipe -Wall
CXXFLAGS := $(CFLAGS)
LDFLAGS := -Wl,-O1 -Wl,--sort-common -Wl,--as-needed
RM = rm -f
OBJS = main.o           \
        Interface.o     \
        EthPacket.o     \
        DeviceInfo.o    \
        DeviceBridge.o  \
        InterfaceInfo.o \
        List.o          \
        Logger.o        \
        Thread.o        \
        UsbComm.o		\
        ArgParser.o

SVNREV:= -D'SVN_REV="$(shell LC_ALL=C svnversion -cn ../ | sed -e "s/.*://" -e "s/\([0-9]*\).*/\1/" | grep "[0-9]" )"'
MISC_VARS += $(SVNREV)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIB_PATH) $(LIBS) -o $(TARGET)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(MISC_VARS) -c $*.cpp

clean:
	-$(RM) $(OBJS) $(TARGET)

.PHONY: all clean
