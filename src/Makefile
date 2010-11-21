CXX := g++
TARGET = usbridge
LIBS := -lpcap -lpthread -lusb-1.0
INCLUDES := -I /usr/include/libusb-1.0
LIB_PATH :=
CFLAGS := -O2 -pipe -Wall
CXXFLAGS := $(CFLAGS)
LDFLAGS := -Wl,-O1 -Wl,--sort-common -Wl,--as-needed
RM = rm -f
OBJS = main.o

SVNREV:= -D'SVN_REV="$(shell LC_ALL=C svnversion -cn ../ | sed -e "s/.*://" -e "s/\([0-9]*\).*/\1/" | grep "[0-9]" )"'
MISC_VARS += $(SVNREV)

all: static

static: core $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) core/libbahamut-core.a $(LIBS) -o $(TARGET)

shared: core $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIB_PATH) $(LIBS) -lbahamut-core -o $(TARGET)

core:
	cd core && make -f Makefile all

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(MISC_VARS) -c $*.cpp

clean: clean-core
	-$(RM) $(OBJS) $(TARGET)

clean-core:
	cd core && make -f Makefile clean

.PHONY: all clean clean-core core

