cmake_minimum_required(VERSION 3.3)

project(LibAvCpp)

SET(MAJOR_VERSION "0")
SET(MINOR_VERSION "1")
SET(PATCH_VERSION "1" CACHE PATCH_VERSION "The package build number." )

# -----------------------   Options and Parameters  -----------------------

option(build_tests "Build all unit tests." OFF)
option(build_samples "Build samples." OFF)
option(build_documentation "Build the project documentation." OFF)

SET(TESTFILES "/testfiles" CACHE TESTFILES "The path to the testfiles.")

SET(LIBAVCPP_VERSION "master" CACHE STRING "The tagged version.")

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

link_libraries("-static")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

add_definitions( -DHTTP_BUFFER_SIZE=4096 -DHTTP_CLIENT_TIMEOUT=5
                 -DHTTP_SERVER_THREAD_POOL_SIZE=10 -DASIO_STANDALONE )

add_definitions(-DLIBAV_CPP_RELEASE="${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
add_definitions(-DTESTFILES="${TESTFILES}/")

# ---------------------   External Dependencies       ---------------------
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#include(ExternalProject)
#include(cmake/gtest.cmake)
#include(cmake/boost.cmake)
#include(cmake/ffmpeg.cmake)

#SET(IMPORTED_INCLUDES ${Boost_INCLUDE_DIRS} ${FFMPEG_INCLUDE_DIR} ${GTEST_INCLUDE_DIR})

# -----------------------      Manage Modules       -----------------------

add_subdirectory ( libavcpp )
if( build_samples )
    add_subdirectory ( samples )
endif()
if( build_tests )
    add_subdirectory ( test )
endif()

# -----------------------   Doxygen                 -----------------------

if( build_documentation )
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    endif(DOXYGEN_FOUND)
endif()
