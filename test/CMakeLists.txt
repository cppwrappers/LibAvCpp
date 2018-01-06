# -------------------------------------------------------------------------
# Build LibAvCpp tests
# -------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.3)

project( libavcpp-test )

# -------------------------------------------------------------------------
# -----------------------   Manage Test Cases       -----------------------
# -------------------------------------------------------------------------
aux_source_directory( ${PROJECT_SOURCE_DIR} TEST_SOURCES )
enable_testing()
include_directories(${ROOT} ${IMPORTED_INCLUDES})
add_executable(${PROJECT_NAME} ${TEST_SOURCES} )
#target_link_libraries(${PROJECT_NAME} avcpp ${CONAN_LIBS} -lm -lz -ldl -lz -llzma -lbz2 -lrt)
target_link_libraries(${PROJECT_NAME} avcpp  -lboost_system -lboost_exception -lboost_context -lboost_chrono -lboost_date_time -lboost_coroutine -lboost_fiber -lboost_thread -lboost_filesystem -lboost_iostreams -lboost_regex -lboost_graph -lboost_timer -lboost_locale -lboost_math_tr1 -lboost_math_tr1f -lboost_math_tr1l -lboost_math_c99 -lboost_math_c99f -lboost_math_c99l -lboost_random -lboost_program_options -lboost_signals -lboost_serialization -lboost_wserialization -lboost_type_erasure -lboost_wave -lavformat -lavcodec -lswresample -lswscale -lavutil -lgtest -lgtest_main -lpthread -llzma -lbz2 -lz -lm -lz -ldl -lz -llzma -lbz2 -lrt -static -ldl -llzma -lbz2 -lrt)
add_test(${PROJECT_NAME} ${PROJECT_NAME})
