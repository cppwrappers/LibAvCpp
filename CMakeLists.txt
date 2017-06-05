cmake_minimum_required(VERSION 3.0)

project(LibAvCpp)

SET(MAJOR_VERSION "0")
SET(MINOR_VERSION "1")
SET(PATCH_VERSION "1" CACHE PATCH_VERSION "The package build number." )

# -------------------------------------------------------------------------
# -----------------------   Options and Parameters  -----------------------
# -------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" )
option(build_tests "Build all unit tests." ON)
option(build_samples "Build samples." ON)
option(build_documentation "Build the project documentation." OFF)

SET(TESTFILES "/testfiles" CACHE TESTFILES "The path to the testfiles.")

# -------------------------------------------------------------------------
# -----------------------   Config Toolchain        -----------------------
# -------------------------------------------------------------------------
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libstdc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=c++11")
    add_definitions(-Wall -Wextra)
    IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
        add_definitions(-DNDEBUG)
        add_definitions(-DELPP_DISABLE_TRACE_LOGS)
    ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)
endif()

add_definitions(-DLIBAV_CPP_RELEASE="${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
add_definitions(-DTESTFILES="${TESTFILES}/")

#add headers for editor
FILE(GLOB_RECURSE HeaderFiles "${PROJECT_SOURCE_DIR}/*.h" "${ROOT}/libavcpp.h")
add_custom_target(headers SOURCES ${HeaderFiles})

# -------------------------------------------------------------------------
# -----------------------      Manage Modules       -----------------------
# -------------------------------------------------------------------------
add_subdirectory ( libavcpp )
if( build_samples )
    add_subdirectory ( samples )
endif()
if( build_tests )
    add_subdirectory ( test )
endif()

# -------------------------------------------------------------------------
# -----------------------   Doxygen                 -----------------------
# -------------------------------------------------------------------------
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
