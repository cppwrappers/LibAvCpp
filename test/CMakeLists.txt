# -------------------------------------------------------------------------
# Build LibAvCpp Samples
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

project( libavcpp-test )

# -------------------------------------------------------------------------
# ---------------------   External Dependencies       ---------------------
# -------------------------------------------------------------------------
include(ExternalProject)
include(${CMAKE_SOURCE_DIR}/cmake/external/gtest.cmake)

SET( INCLUDES ${INCLUDES} ${GTEST_INCLUDE_DIR} )

SET( INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIRS} ${AVCODEC_INCLUDES} )


# -------------------------------------------------------------------------
# -----------------------   Manage Test Cases       -----------------------
# -------------------------------------------------------------------------
aux_source_directory( ${PROJECT_SOURCE_DIR} TEST_SOURCES )
enable_testing()
include_directories(${ROOT} ${INCLUDES} ${GTEST_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${TEST_SOURCES} )
target_link_libraries(${PROJECT_NAME} avcpp gtest ${GMOCK_MAIN_PATH} ${LIBS} -pthread -lswresample -lm -lz )
add_test(${PROJECT_NAME} ${PROJECT_NAME})
