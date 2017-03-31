///*
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation; either
//    version 2.1 of the License, or (at your option) any later version.

//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.

//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//*/

//#include <string>
//#include <fstream>
//#include <map>

//#include <boost/filesystem.hpp>

//#include "../src/libav/libavwrapper.h"
//#include "../src/libav/averrc.h"

//#include <gtest/gtest.h>

//namespace libav {

//class LibavWrapperTest : public ::testing::Test {
// protected:
//  static void SetUpTestCase() {
//      //      if( boost::filesystem::exists( "/tmp/out-iostream.mp3" ) )
//      //          boost::filesystem::remove( "/tmp/out-iostream.mp3" );
//  }
//  static void TearDownTestCase() {}
//};

//TEST_F( LibavWrapperTest, open_audio_context ) {
//    std::string filename = std::string( TESTFILES ) + "sample.mp3";
//    LibavWrapper wrapper;
//    ASSERT_EQ( "Success", wrapper.make_input_format_context( filename.c_str() ).message() );

//    EXPECT_EQ ( 1, wrapper.input_format_context->nb_streams );
//    EXPECT_EQ ( AVMEDIA_TYPE_AUDIO, wrapper.input_format_context->streams[0]->codec->codec_type );
//}
//TEST_F( LibavWrapperTest, open_audio_context_stream ) {
//    std::string filename = std::string( TESTFILES ) + "sample.mp3";
//    std::fstream _fs ( filename );
//    LibavWrapper wrapper;
//    ASSERT_EQ( "Success", wrapper.make_input_format_context( &_fs ).message() );

//    EXPECT_EQ ( 1, wrapper.input_format_context->nb_streams );
//    EXPECT_EQ ( AVMEDIA_TYPE_AUDIO, wrapper.input_format_context->streams[0]->codec->codec_type );
//}
//TEST_F( LibavWrapperTest, DISABLED_open_audio_context_image ) {
//    std::string filename = std::string( TESTFILES ) + "agp27.jpg";
//    std::fstream _fs ( filename );
//    LibavWrapper wrapper;
//    ASSERT_EQ( "Success", wrapper.make_input_format_context( &_fs ).message() );

//    EXPECT_EQ ( 1, wrapper.input_format_context->nb_streams );
//    EXPECT_EQ ( AVMEDIA_TYPE_AUDIO, wrapper.input_format_context->streams[0]->codec->codec_type );
//}
//TEST_F( LibavWrapperTest, open_audio_context_video ) {
//    std::string filename = std::string( TESTFILES ) + "sample.avi";
//    std::fstream _fs ( filename );
//    LibavWrapper wrapper;
//    ASSERT_EQ( "Success", wrapper.make_input_format_context( &_fs ).message() );

//    EXPECT_EQ ( 1, wrapper.input_format_context->nb_streams );
//    EXPECT_EQ ( AVMEDIA_TYPE_VIDEO, wrapper.input_format_context->streams[0]->codec->codec_type );
//}
//}//namespace libav
