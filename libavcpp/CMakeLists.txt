# -------------------------------------------------------------------------
# Build LibAvCpp Samples
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

project( avcpp )

# -------------------------------------------------------------------------
# ---------------------   External Dependencies       ---------------------
# -------------------------------------------------------------------------
find_package(Boost 1.54.0 COMPONENTS filesystem system)
find_package(LibAVCodec COMPONENTS avformat avcodec avfilter avresample avutil )
SET( LIBS ${LIBS}
    ${Boost_LIBRARIES} ${AVCODEC_LIBRARIES}
#    -lpcrecpp -lpthread -luuid -lswresample -lm -lz
)
SET( INCLUDES   ${INCLUDES} ${Boost_INCLUDE_DIRS} ${AVCODEC_INCLUDES} )

# -------------------------------------------------------------------------
# -----------------------   Compile Binary          -----------------------
# -------------------------------------------------------------------------
include_directories(${ROOT} ${INCLUDES})
aux_source_directory( ${PROJECT_SOURCE_DIR} LIBAVCPP_SOURCES )

add_library( avcpp SHARED ${LIBAVCPP_SOURCES} )
target_link_libraries(avcpp ${LIBS})
install (TARGETS avcpp
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin)

#add headers for editor
FILE(GLOB_RECURSE HeaderFiles "${PROJECT_SOURCE_DIR}/*.h")
add_custom_target(libavcpp-headers SOURCES ${HeaderFiles})
