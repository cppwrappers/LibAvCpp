/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <string>
#include <fstream>
#include <map>

#include <boost/filesystem.hpp>

#include "../libavcpp/format.h"
#include "../libavcpp/metadata.h"
#include "../libavcpp/stream.h"

#include <gtest/gtest.h>

namespace av {

class FormatTest : public ::testing::Test {
 protected:
  static void SetUpTestCase() {}

  static void TearDownTestCase() {}

//  static void delete_files () {
//      if( boost::filesystem::exists( "/tmp/out-iostream.mp3" ) )
//          boost::filesystem::remove( "/tmp/out-iostream.mp3" );

//      if( boost::filesystem::exists( "/tmp/file_out.mp3" ) )
//          boost::filesystem::remove( "/tmp/file_out.mp3" );

//      if( boost::filesystem::exists( "/tmp/file_istream_out.mp3" ) )
//          boost::filesystem::remove( "/tmp/file_istream_out.mp3" );
//}
};

TEST_F( FormatTest, open_filename ) {
    Format _format;
    std::error_code _code = _format.open( std::string( TESTFILES ) + "/sample.mp3" );
    EXPECT_FALSE( _code );
}
TEST_F( FormatTest, codec_type ) {
    Format _format;
    std::error_code _code = _format.open( std::string( TESTFILES ) + "/sample.mp3" );
    EXPECT_FALSE( _code );

    std::vector< av::Stream > _streams = _format.streams();
    EXPECT_EQ( 1U, _streams.size() );
    EXPECT_EQ( CodecType::AUDIO, _streams[0].codec_type );
}
TEST_F( FormatTest, metadata ) { //TODO file with metadata
    Format _format;
    std::error_code _code = _format.open( std::string( TESTFILES ) + "/sample.mp3" );
    EXPECT_FALSE( _code );

    libav::Metadata _metadata = _format.metadata();
    EXPECT_EQ( "", _metadata.get( libav::Metadata::TITLE ) );
}
}//namespace av
