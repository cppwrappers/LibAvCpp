ExternalProject_Add(
    boost_main
    URL https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz
    DOWNLOAD_NO_PROGRESS 1
    PATCH_COMMAND ""
    CONFIGURE_COMMAND cd <SOURCE_DIR> && ./bootstrap.sh --with-libraries=filesystem,system
    UPDATE_COMMAND ""
    BUILD_COMMAND cd <SOURCE_DIR> && ./b2 &> boost_build.log
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS "boost_main-prefix/src/boost_main/stage/lib/libboost_system.a"
    BUILD_BYPRODUCTS "boost_main-prefix/src/boost_main/stage/lib/libboost_filesystem.a"
)

ExternalProject_Get_Property(boost_main binary_dir)
set(Boost_LIBRARIES_FILESYSTEM ${binary_dir}/stage/lib/${CMAKE_FIND_LIBRARY_PREFIXES}boost_filesystem.a )
set(Boost_LIBRARIES_SYSTEM ${binary_dir}/stage/lib/${CMAKE_FIND_LIBRARY_PREFIXES}boost_system.a )
ExternalProject_Get_Property(boost_main source_dir)
set(Boost_INCLUDE_DIRS ${source_dir})

set(Boost_LIBRARY boost)
add_library(${Boost_LIBRARY} INTERFACE IMPORTED)
add_dependencies(${Boost_LIBRARY} boost_main)

set(Boost_LIBRARY filesystem)
add_library(${Boost_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${Boost_LIBRARY} PROPERTY IMPORTED_LOCATION ${Boost_LIBRARIES_FILESYSTEM} )
add_dependencies(${Boost_LIBRARY} boost_main)

set(Boost_LIBRARY system)
add_library(${Boost_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${Boost_LIBRARY} PROPERTY IMPORTED_LOCATION ${Boost_LIBRARIES_SYSTEM})
add_dependencies(${Boost_LIBRARY} boost_main)
