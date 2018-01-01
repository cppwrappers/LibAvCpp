#include(ExternalProject)
# - download and build ffmpeg
# Once done this will define
#  FFMPEG_INCLUDE_DIR - The ffmpeg include directory
#  FFMPEG_LIBRARIES - The libraries needed to use ffmpeg


ExternalProject_Add(
  lame_src
  URL "https://sourceforge.net/projects/lame/files/lame/3.98.4/lame-3.98.4.tar.gz/download"
  DOWNLOAD_NO_PROGRESS 1
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cd <SOURCE_DIR> && ./configure --enable-static --disable-frontend --prefix=${CMAKE_BINARY_DIR}/target #&> lame_config.log
  BUILD_COMMAND cd <SOURCE_DIR> && make &> lame_build.log
  INSTALL_COMMAND cd <SOURCE_DIR> && make install
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_BYPRODUCTS lame_src-prefix/src/lame_src/libmp3lame/.libs/libmp3lame.a
)
ExternalProject_Get_Property(lame_src source_dir)
set(LAME_INCLUDE_DIR ${source_dir}/include)
ExternalProject_Get_Property(lame_src binary_dir)
set(LAME_LIBRARIES ${binary_dir}/libmp3lame/.libs/${CMAKE_FIND_LIBRARY_PREFIXES}mp3lame.a)

set(LAME_LIBRARY lame)
add_library(${LAME_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${LAME_LIBRARY} PROPERTY IMPORTED_LOCATION ${LAME_LIBRARIES} )
add_dependencies(${LAME_LIBRARY} lame_src)

ExternalProject_Add(
  zlib_src
  URL "http://zlib.net/zlib-1.2.11.tar.gz"
  DOWNLOAD_NO_PROGRESS 1
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cd <SOURCE_DIR> && ./configure --prefix=${CMAKE_BINARY_DIR}/target
  BUILD_COMMAND cd <SOURCE_DIR> && make &> zlib_build.log
  INSTALL_COMMAND cd <SOURCE_DIR> && make install
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_BYPRODUCTS zlib_src-prefix/src/zlib_src/libz.a
)
ExternalProject_Get_Property(zlib_src source_dir)
set(ZLIB_INCLUDE_DIR ${source_dir})
ExternalProject_Get_Property(zlib_src binary_dir)
set(ZLIB_LIBRARIES ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}z.a)

set(ZLIB_LIBRARY zlib)
add_library(${ZLIB_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${ZLIB_LIBRARY} PROPERTY IMPORTED_LOCATION ${ZLIB_LIBRARIES} )
add_dependencies(${ZLIB_LIBRARY} zlib_src)

ExternalProject_Add(
  xzutils_src
  URL "https://sourceforge.net/projects/lzmautils/files/xz-5.2.3.tar.xz/download"
  DOWNLOAD_NO_PROGRESS 1
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cd <SOURCE_DIR> && ./configure --prefix=${CMAKE_BINARY_DIR}/target
  BUILD_COMMAND cd <SOURCE_DIR> && make &> lzma_build.log
  INSTALL_COMMAND cd <SOURCE_DIR> && make install
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_BYPRODUCTS xzutils_src-prefix/src/xzutils_src/src/liblzma/.libs/liblzma.a
)
ExternalProject_Get_Property(xzutils_src source_dir)
set(XZUTILS_INCLUDE_DIR ${source_dir})
ExternalProject_Get_Property(xzutils_src binary_dir)
set(XZUTILS_LIBRARIES ${binary_dir}/src/liblzma/.libs/${CMAKE_FIND_LIBRARY_PREFIXES}lzma.a)

set(XZUTILS_LIBRARY lzma)
add_library(${XZUTILS_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${XZUTILS_LIBRARY} PROPERTY IMPORTED_LOCATION ${XZUTILS_LIBRARIES} )
add_dependencies(${XZUTILS_LIBRARY} xzutils_src)


#ExternalProject_Add(
#  libarchive
#  URL "https://github.com/libarchive/libarchive/tarball/master"
#  DOWNLOAD_NO_PROGRESS 1
#  BUILD_IN_SOURCE 1
#  CONFIGURE_COMMAND cd <SOURCE_DIR> &&  /bin/sh build/autogen.sh && ./configure --prefix=${CMAKE_BINARY_DIR}/target
#  BUILD_COMMAND cd <SOURCE_DIR> && make #&> lzma_build.log
#  INSTALL_COMMAND cd <SOURCE_DIR> && make install
#  UPDATE_COMMAND ""
#  PATCH_COMMAND ""
#  BUILD_BYPRODUCTS libarchive-prefix/src/libarchive/.libs/libarchive.a
#)
#ExternalProject_Get_Property(libarchive source_dir)
#set(LIBARCHIVE_INCLUDE_DIR ${source_dir})
#ExternalProject_Get_Property(libarchive binary_dir)
#set(LIBARCHIVE_LIBRARIES ${binary_dir}/.libs/${CMAKE_FIND_LIBRARY_PREFIXES}archive.a)

#set(LIBARCHIVE_LIBRARY archive)
#add_library(${LIBARCHIVE_LIBRARY} STATIC IMPORTED)
#set_property(TARGET ${LIBARCHIVE_LIBRARY} PROPERTY IMPORTED_LOCATION ${LIBARCHIVE_LIBRARIES} )
#add_dependencies(${LIBARCHIVE_LIBRARY} libarchive)


ExternalProject_Add(
  bzip2
  URL "http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz"
  DOWNLOAD_NO_PROGRESS 1
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND "" #cd <SOURCE_DIR> && ./configure
  BUILD_COMMAND cd <SOURCE_DIR> && make &> bzip2_build.log
  INSTALL_COMMAND cd <SOURCE_DIR> && make install PREFIX=${CMAKE_BINARY_DIR}/target
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_BYPRODUCTS bzip2-prefix/src/bzip2/libbz2.a
)
ExternalProject_Get_Property(bzip2 source_dir)
set(BZIP2_INCLUDE_DIR ${source_dir})
ExternalProject_Get_Property(bzip2 binary_dir)
set(BZLIB2_LIBRARIES ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}bz2.a)

set(BZLIB2_LIBRARY bz2)
add_library(${BZLIB2_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${BZLIB2_LIBRARY} PROPERTY IMPORTED_LOCATION ${BZLIB2_LIBRARIES} )
add_dependencies(${BZLIB2_LIBRARY} bzip2)

message(${CMAKE_BINARY_DIR}/target/lib)

ExternalProject_Add(
  ffmpeg_src
  DEPENDS lame_src
  DEPENDS zlib_src
  DEPENDS lzma
  URL "https://github.com/FFmpeg/FFmpeg/archive/release/3.3.zip"
  DOWNLOAD_NO_PROGRESS 1
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cd <SOURCE_DIR> && ./configure --enable-debug --enable-logging --disable-ffplay --disable-ffserver
    --disable-doc --disable-network --enable-libmp3lame
    --disable-hwaccels --disable-cuda --disable-cuvid --disable-d3d11va --disable-dxva2 --disable-nvenc --disable-vaapi --disable-vda --disable-vdpau --disable-videotoolbox --disable-sdl2
    --disable-encoders --enable-encoder=vorbis --enable-encoder=flac --enable-encoder=libmp3lame
    --disable-decoder=atrac1 --disable-decoder=atrac3 --disable-decoder=atrac3al --disable-decoder=atrac3p --disable-decoder=atrac3pal
    --extra-ldflags=-L/build/target/lib --extra-cflags=-I/build/target/include
#    --disable-decoders --enable-decoder=aac --enable-decoder=ac3 --enable-decoder=ape --enable-decoder=bmp --enable-decoder=mjpeg
#                       --enable-decoder=mjpeg_cuvid --enable-decoder=mjpegb --enable-decoder=mp3 --enable-decoder=mp2 --enable-decoder=mp1
#                       --enable-decoder=mpeg1video --enable-decoder=mpeg2video --enable-decoder=mpeg4 --enable-decoder=h264
#                       --enable-decoder=flac --enable-decoder=vorbis --enable-decoder=png
#                       --enable-decoder=vp8 --enable-decoder=vp9 --enable-decoder=wavpack
#                       --enable-decoder=zlib
  BUILD_COMMAND cd <SOURCE_DIR> && make &> ffmpeg_build.log
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libavcodec/libavcodec.a
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libavfilter/libavfilter.a
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libavresample/libavresample.a
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libswresample/libswresample.a
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libavdevide/libavdevice.a
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libavformat/libavformat.a
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libavutil/libavutil.a
  BUILD_BYPRODUCTS ffmpeg_src-prefix/src/ffmpeg_src/libswscale/libswscale.a
)

#--disable-muxers --enable-muxer=mp3 --enable-muxer=flac --enable-muxer=mjpeg
#--disable-demuxers --enable-demuxer=mp3 --enable-demuxer=flac --enable-demuxer=ffmetadata --enable-demuxer=wav
#                   --enable-demuxer=mjpeg --enable-demuxer=data --enable-demuxer=ogg --enable-demuxer=image2
#--disable-parsers --enable-parser=aac --enable-parser=flac --enable-parser=mpegaudio
#                  --enable-parser=mpegvideo --enable-parser=opus --enable-parser=vorbis
#                  --enable-parser=png --enable-parser=mjpeg --enable-parser=bmp --enable-parser=h261
#                  --enable-parser=h263 --enable-parser=h264 --enable-parser=hevc --enable-parser=png
#                  --enable-parser=pnm --enable-parser=vorbis --enable-parser=mpeg4video
#--disable-protocols --enable-protocol=file --enable-protocol=async --enable-protocol=cache --enable-protocol=concat
#                    --enable-protocol=data --enable-protocol=crypto
#--disable-devices --enable-outdev=oss
#--disable-filters --enable-filter=aresample --enable-filter=resample --enable-filter=format

ExternalProject_Get_Property(ffmpeg_src source_dir)
set(FFMPEG_INCLUDE_DIR ${source_dir})
ExternalProject_Get_Property(ffmpeg_src binary_dir)

set(FFMPEG_AVCODEC_LIBRARIES ${binary_dir}/libavcodec/${CMAKE_FIND_LIBRARY_PREFIXES}avcodec.a)
set(FFMPEG_AVFILTER_LIBRARIES ${binary_dir}/libavfilter/${CMAKE_FIND_LIBRARY_PREFIXES}avfilter.a)
set(FFMPEG_AVRESAMPLE_LIBRARIES ${binary_dir}/libavresample/${CMAKE_FIND_LIBRARY_PREFIXES}avresample.a)
set(FFMPEG_SWRESAMPLE_LIBRARIES ${binary_dir}/libswresample/${CMAKE_FIND_LIBRARY_PREFIXES}swresample.a)
set(FFMPEG_AVDEVICE_LIBRARIES ${binary_dir}/libavdevice/${CMAKE_FIND_LIBRARY_PREFIXES}avdevice.a)
set(FFMPEG_AVFORMAT_LIBRARIES ${binary_dir}/libavformat/${CMAKE_FIND_LIBRARY_PREFIXES}avformat.a)
set(FFMPEG_AVUTIL_LIBRARIES ${binary_dir}/libavutil/${CMAKE_FIND_LIBRARY_PREFIXES}avutil.a)
set(FFMPEG_SWSCALE_LIBRARIES ${binary_dir}/libswscale/${CMAKE_FIND_LIBRARY_PREFIXES}swscale.a)

set(AVCODEC_LIBRARY avcodec)
add_library(${AVCODEC_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${AVCODEC_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_AVCODEC_LIBRARIES} )
add_dependencies(${AVCODEC_LIBRARY} ffmpeg_src)

set(AVFILTER_LIBRARY avfilter)
add_library(${AVFILTER_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${AVFILTER_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_AVFILTER_LIBRARIES} )
add_dependencies(${AVFILTER_LIBRARY} ffmpeg_src)

set(AVRESAMPLE_LIBRARY avresample)
add_library(${AVRESAMPLE_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${AVRESAMPLE_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_AVRESAMPLE_LIBRARIES} )
add_dependencies(${AVRESAMPLE_LIBRARY} ffmpeg_src)

set(SWRESAMPLE_LIBRARY swresample)
add_library(${SWRESAMPLE_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${SWRESAMPLE_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_SWRESAMPLE_LIBRARIES} )
add_dependencies(${SWRESAMPLE_LIBRARY} ffmpeg_src)

set(AVDEVICE_LIBRARY avdevice)
add_library(${AVDEVICE_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${AVDEVICE_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_AVDEVICE_LIBRARIES} )
add_dependencies(${AVDEVICE_LIBRARY} ffmpeg_src)

set(AVFORMAT_LIBRARY avformat)
add_library(${AVFORMAT_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${AVFORMAT_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_AVFORMAT_LIBRARIES} )
add_dependencies(${AVFORMAT_LIBRARY} ffmpeg_src)

set(AVUTIL_LIBRARY avutil)
add_library(${AVUTIL_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${AVUTIL_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_AVUTIL_LIBRARIES} )
add_dependencies(${AVUTIL_LIBRARY} ffmpeg_src)

set(SWSCALE_LIBRARY swscale)
add_library(${SWSCALE_LIBRARY} STATIC IMPORTED)
set_property(TARGET ${SWSCALE_LIBRARY} PROPERTY IMPORTED_LOCATION ${FFMPEG_SWSCALE_LIBRARIES} )
add_dependencies(${SWSCALE_LIBRARY} ffmpeg_src)
