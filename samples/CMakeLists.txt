# -------------------------------------------------------------------------
# Build LibAvCpp Samples
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5.0)

project( samples )

SET( PROJECT_TEST_NAME "lightning-tests" )

# -------------------------------------------------------------------------
# ---------------------   External Dependencies       ---------------------
# -------------------------------------------------------------------------
find_package(Boost 1.54.0 COMPONENTS filesystem system)
find_package(LibAVCodec COMPONENTS avformat avcodec avfilter avresample swscale avutil )
SET( LIBS ${LIBS}
    ${Boost_LIBRARIES} ${AVCODEC_LIBRARIES}
#    -lpcrecpp -lpthread -luuid -lswresample -lm -lz
)
SET( INCLUDES   ${INCLUDES} ${Boost_INCLUDE_DIRS} ${AVCODEC_INCLUDES} )

# -------------------------------------------------------------------------
# -----------------------   Compile Binary          -----------------------
# -------------------------------------------------------------------------
include_directories(${ROOT} ${INCLUDES})
#aux_source_directory( ${PROJECT_SOURCE_DIR} SQUAWK_SOURCES )

find_package(LibAO)
if( ${LIBAO_FOUND} )
    include_directories(${ROOT} ${INCLUDES} ${LIBAO_INCLUDE_DIR})
    add_executable(ao_play play_ao.cpp)
    target_link_libraries(ao_play libavcpp ${LIBAO_LIBRARIES} ${LIBS})
endif()

find_package(SDL)
#if( ${SDL_FOUND} )
    include_directories(${ROOT} ${INCLUDES} ${SDL_INCLUDE_DIR})
    add_executable(sdl_play play_sdl.cpp)
    target_link_libraries(sdl_play libavcpp ${LIBS} ${SDL_LIBRARIES})
#endif()

add_executable( metadata metadata.cpp )
target_link_libraries(metadata libavcpp ${LIBS} -lpcrecpp -lpthread -luuid -lswresample -lm -lz)

## -------------------------------------------------------------------------
## -----------------------        Test Code          -----------------------
## -------------------------------------------------------------------------
#aux_source_directory( . TEST_LIST)
#include_directories( ${ROOT}/src ${INCLUDES} ${GTEST_INCLUDE_DIR} )
#add_executable(testmain_lightning ${TEST_LIST})
#if(CMAKE_COMPILER_IS_GNUCXX)
#target_compile_options(testmain_lightning PRIVATE -g -O0 -fprofile-arcs -ftest-coverage -Wall -Wextra )
#endif()
#target_link_libraries(testmain_lightning ${GMOCK_MAIN_PATH} re2 bustache asio gtest ${LIBS} -fprofile-arcs -ftest-coverage )
#add_test(${PROJECT_TEST_NAME} testmain_lightning)
