CXX := g++
AR := ar
TARGET = bahamut-core
SONAME_VER = 0
VERSION = $(SONAME_VER).0.0
LIBS := -lpcap -lpthread -lusb-1.0
INCLUDES := -I /usr/include/libusb-1.0
LIB_PATH :=
CFLAGS := -O2 -pipe -Wall -fPIC
CXXFLAGS := $(CFLAGS)
LDFLAGS := -shared -Wl,-soname,lib$(TARGET).so.$(SONAME_VER)
RM = rm -f
OBJS =  Interface.o     \
        Socket.o        \
        Packet.o        \
        EthPacket.o     \
        DeviceInfo.o    \
        ServerSocket.o  \
		Adapter.o       \
		SocketAdapter.o \
		StreamSocket.o  \
        InterfaceInfo.o \
        List.o          \
        Logger.o        \
        Thread.o        \
		InterfaceAdapter.o \
		UsbComm.o \
		UsbCommAdapter.o \
        AbstractSocket.o

all: shared static

shared: $(OBJS)
	$(CXX) $(LDFLAGS) $(LIBS) -o lib$(TARGET).so.$(VERSION) $(OBJS)
	ln -sf lib$(TARGET).so.$(VERSION) lib$(TARGET).so.$(SONAME_VER)
	ln -sf lib$(TARGET).so.$(VERSION) lib$(TARGET).so

static: $(OBJS)
	$(AR) cr lib$(TARGET).a $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(MISC_VARS) -c $*.cpp

clean:
	-$(RM) $(OBJS) lib$(TARGET).*

.PHONY: all clean shared static
