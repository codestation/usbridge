find_package(Threads REQUIRED)
find_package(PCAP REQUIRED)
find_package(LibUSB REQUIRED)

add_subdirectory(core)

file(GLOB usbridge_SOURCES *.cpp)

include_directories(${PCAP_INCLUDE_DIRS})
include_directories(${LIBUSB_INCLUDE_DIRS})
include_directories(${usbridge_SOURCE_DIR}/src/core)

link_directories(${usbridge_SOURCE_DIR}/src/core)

add_executable(usbridge ${usbridge_SOURCES})

target_link_libraries(usbridge bahamut-core)
target_link_libraries(usbridge ${LIBUSB_LIBRARIES})
target_link_libraries(usbridge ${PCAP_LIBRARIES})
if(WIN32)
target_link_libraries(usbridge wsock32 iphlpapi ws2_32)
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2 -pipe -Wall -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS "-s")
endif()

#set(CMAKE_BUILD_TYPE Release)
