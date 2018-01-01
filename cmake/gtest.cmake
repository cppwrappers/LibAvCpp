ExternalProject_Add(
  googletest
  URL "https://github.com/google/googletest/archive/master.zip"
  DOWNLOAD_NO_PROGRESS 1
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_BYPRODUCTS "googletest-prefix/src/googletest-build/googlemock/gtest/libgtest.a"
  BUILD_BYPRODUCTS "googletest-prefix/src/googletest-build/googlemock/gtest/libgtest_main.a"
  BUILD_BYPRODUCTS "googletest-prefix/src/googletest-build/googlemock/libgmock_main.a"
  BUILD_BYPRODUCTS "googletest-prefix/src/googletest-build/googlemock/libgmock.a"
)
ExternalProject_Get_Property(googletest source_dir)
set(GTEST_INCLUDE_DIR ${source_dir}/googletest/include/ ${source_dir}/googlemock/include/)
ExternalProject_Get_Property(googletest binary_dir)

set(GTEST_LIBRARY_PATH ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a )
set(GMOCK_LIBRARY_PATH ${binary_dir}/googlemock/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.a )
set(GTEST_MAIN_PATH ${binary_dir}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.a )
set(GMOCK_MAIN_PATH ${binary_dir}/googlemock/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main.a )

set(GTEST_LIBRARY gtest)
add_library(${GTEST_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${GTEST_LIBRARY} PROPERTY IMPORTED_LOCATION ${GTEST_LIBRARY_PATH} )
add_dependencies(${GTEST_LIBRARY} googletest)

set(GMOCK_LIBRARY gmock)
add_library(${GMOCK_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${GMOCK_LIBRARY} PROPERTY IMPORTED_LOCATION ${GMOCK_LIBRARY_PATH} )
add_dependencies(${GMOCK_LIBRARY} googletest)
