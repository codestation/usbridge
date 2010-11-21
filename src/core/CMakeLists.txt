set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB core_SOURCES *.cpp)
add_library (bahamut-core ${core_SOURCES})
include_directories(${LIBUSB_INCLUDE_DIRS})
include_directories(${PCAP_INCLUDE_DIRS})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2 -pipe -Wall -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS "-s")
endif()

#set(CMAKE_BUILD_TYPE Release)
