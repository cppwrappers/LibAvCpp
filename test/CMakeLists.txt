# -------------------------------------------------------------------------
# Build LibAvCpp Samples
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5.0)

project( libavcpp-test )

# -------------------------------------------------------------------------
# ---------------------   External Dependencies       ---------------------
# -------------------------------------------------------------------------
include(ExternalProject)
include(${CMAKE_MODULE_PATH}/../external/gtest.cmake)

SET( INCLUDES ${INCLUDES} ${GTEST_INCLUDE_DIR} )

find_package(SDL)
SET( LIBS ${LIBS}
    ${Boost_LIBRARIES} ${AVCODEC_LIBRARIES}
)
SET( INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIRS} ${AVCODEC_INCLUDES} )


# -------------------------------------------------------------------------
# -----------------------   Manage Test Cases       -----------------------
# -------------------------------------------------------------------------
aux_source_directory( ${PROJECT_SOURCE_DIR} TEST_SOURCES )
enable_testing()
include_directories(${ROOT} ${INCLUDES} ${GTEST_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${TEST_SOURCES} )
target_link_libraries(${PROJECT_NAME} libavcpp gtest ${GMOCK_MAIN_PATH} ${LIBS} -pthread )
add_test(${PROJECT_NAME} ${PROJECT_NAME})
