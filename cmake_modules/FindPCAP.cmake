if(EXISTS $ENV{PCAPDIR})
	find_path(PCAP_INCLUDE_DIR 	NAMES pcap/pcap.h pcap.h
		PATHS $ENV{PCAPDIR}	NO_DEFAULT_PATH)
	if(WIN32)
		find_library(PCAP_LIBRARY NAMES pcap
			PATHS $ENV{PCAPDIR} NO_DEFAULT_PATH)
	else(WIN32)
		find_library(PCAP_LIBRARY NAMES wpcap
			PATHS $ENV{PCAPDIR} NO_DEFAULT_PATH)
	endif(WIN32)
else(EXISTS $ENV{PCAPDIR})
	find_path(PCAP_INCLUDE_DIR NAMES pcap/pcap.h pcap.h PATHS ${PCAP_HINT_INCLUDE_DIR})
	if(WIN32)
		find_library(PCAP_LIBRARY wpcap PATHS ${PCAP_HINT_LIB_DIR})
	else(WIN32)
		find_library(PCAP_LIBRARY pcap ${PCAP_HINT_LIB_DIR})
	endif(WIN32)

endif(EXISTS $ENV{PCAPDIR})

set(PCAP_INCLUDE_DIRS ${PCAP_INCLUDE_DIR})
set(PCAP_LIBRARIES ${PCAP_LIBRARY})

if(PCAP_INCLUDE_DIRS)
  message(STATUS " Pcap include dirs set to ${PCAP_INCLUDE_DIRS}")
else(PCAP_INCLUDE_DIRS)
  message(FATAL " Pcap include dirs cannot be found")
endif(PCAP_INCLUDE_DIRS)

if(PCAP_LIBRARIES)
  message(STATUS " Pcap library set to  ${PCAP_LIBRARIES}")
else(PCAP_LIBRARIES)
  message(FATAL " Pcap library cannot be found")
endif(PCAP_LIBRARIES)

#Functions
INCLUDE(CheckFunctionExists)
INCLUDE(CheckVariableExists)
SET(CMAKE_REQUIRED_INCLUDES ${PCAP_INCLUDE_DIRS})
SET(CMAKE_REQUIRED_LIBRARIES ${PCAP_LIBRARIES})
CHECK_VARIABLE_EXISTS("pcap_version" HAVE_PCAP_VERSION)
CHECK_FUNCTION_EXISTS("pcap_open_dead" HAVE_PCAP_OPEN_DEAD)
CHECK_FUNCTION_EXISTS("pcap_freecode" HAVE_PCAP_FREECODE)
#
# Note: for pcap_breakloop() and pcap_findalldevs(), the autoconf script
# checks for more than just whether the function exists, it also checks
# for whether pcap.h declares it; Mac OS X software/security updates can
# update libpcap without updating the headers.
#
CHECK_FUNCTION_EXISTS("pcap_breakloop" HAVE_PCAP_BREAKLOOP)
CHECK_FUNCTION_EXISTS("pcap_findalldevs" HAVE_PCAP_FINDALLDEVS)
CHECK_FUNCTION_EXISTS("pcap_datalink_val_to_name" HAVE_PCAP_DATALINK_VAL_TO_NAME)
CHECK_FUNCTION_EXISTS("pcap_datalink_name_to_val" HAVE_PCAP_DATALINK_NAME_TO_VAL)
CHECK_FUNCTION_EXISTS("pcap_datalink_val_to_description" HAVE_PCAP_DATALINK_VAL_TO_DESCRIPTION)
CHECK_FUNCTION_EXISTS("pcap_list_datalinks" HAVE_PCAP_LIST_DATALINKS)
CHECK_FUNCTION_EXISTS("pcap_set_datalink" HAVE_PCAP_SET_DATALINK)
CHECK_FUNCTION_EXISTS("pcap_lib_version" HAVE_PCAP_LIB_VERSION)
CHECK_FUNCTION_EXISTS("pcap_get_selectable_fd" HAVE_PCAP_GET_SELECTABLE_FD)
CHECK_FUNCTION_EXISTS("pcap_free_datalinks" HAVE_PCAP_FREE_DATALINKS)
# reset vars
SET(CMAKE_REQUIRED_INCLUDES "")
SET(CMAKE_REQUIRED_LIBRARIES "")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PCAP DEFAULT_MSG PCAP_INCLUDE_DIRS PCAP_LIBRARIES)

MARK_AS_ADVANCED(
  PCAP_LIBRARIES
  PCAP_INCLUDE_DIRS
)
