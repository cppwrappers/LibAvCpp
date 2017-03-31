# - Find AVCODEC
# Find the native installation of this package: includes and libraries.
#
# AVCODEC_INCLUDES - where to find headers for this package.
# AVCODEC_LIBRARIES - List of libraries when using this package.
# AVCODEC_FOUND - True if this package can be found.

set(LibAVCodec_VALID_COMPONENTS avformat avcodec avutil)

if (AVCODEC_INCLUDES)
set (AVCODEC_FIND_QUIETLY TRUE)
endif (AVCODEC_INCLUDES)

find_path (AVCODEC_INCLUDES libavcodec/avcodec.h)

#find_library (AVCODEC_LIBRARIES NAMES avcodec avdevice avfilter avformat avresample avutil swscale HINTS "/usr/local")
#find_library (AVCODEC_LIBRARIES NAMES avdevice HINTS "/usr/local")

#include (FindPackageHandleStandardArgs)
# find_package_handle_standard_args (AVCODEC DEFAULT_MSG AVCODEC_LIBRARIES AVCODEC_INCLUDES)

if(LibAVCodec_FIND_COMPONENTS)
  foreach(component ${LibAVCodec_FIND_COMPONENTS})
    STRING(TOUPPER ${component} component_upper)
    find_library(AVCODEC_${component_upper}_LIBRARIES NAMES ${component} HINTS "/usr/local")
    set(AVCODEC_LIBRARIES ${AVCODEC_LIBRARIES} ${AVCODEC_${component_upper}_LIBRARIES})
    set(AVCODEC_${component_upper}_FOUND true)
  endforeach()
endif()

mark_as_advanced (AVCODEC_LIBRARIES AVCODEC_INCLUDES)
