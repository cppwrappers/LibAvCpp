# -------------------------------------------------------------------------
# Build LibAvCpp library
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.3)

project( avcpp )

# -----------------------   Compile library          -----------------------

#include_directories(${ROOT} ${IMPORTED_INCLUDES})
aux_source_directory(${PROJECT_SOURCE_DIR} LIBAVCPP_SOURCES )
add_library(${PROJECT_NAME} ${LIBAVCPP_SOURCES} )
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib COMPONENT library)
