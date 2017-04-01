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
TEST_F( FormatTest, time_to_string ) {
    EXPECT_EQ( "00:00:01.000", Format::time_to_string( 1000 ) );
    EXPECT_EQ( "01:00:00.000", Format::time_to_string( 3600000 ) );
    EXPECT_EQ( "02:02:28.015", Format::time_to_string( 7348015 ) );
}
TEST_F( FormatTest, ParseMpegLayer2Mpthreetest) {

    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "mpthreetest.mp3" ).message() );

    ASSERT_EQ( 1U, format.streams().size() );
    av::Stream stream = format.streams().front();
    ASSERT_EQ( 128000, stream.bitrate );
    ASSERT_EQ( 1, stream.channels );
    ASSERT_EQ( 0, stream.bits_per_sample );
    ASSERT_EQ( 44100, stream.samplerate );
    ASSERT_EQ( 12, format.playtime() ); //playlength.

    libav::Metadata metadata = format.metadata();
    ASSERT_EQ( "Test of MP3 File", metadata.get( libav::Metadata::TITLE ) );
    ASSERT_EQ( "Me", metadata.get( libav::Metadata::ALBUM ) );
    ASSERT_EQ( "test", metadata.get( libav::Metadata::COMMENT ) );
    ASSERT_EQ( "Me", metadata.get( libav::Metadata::ARTIST ) );
    ASSERT_EQ( "2006", metadata.get( libav::Metadata::YEAR ) );
    ASSERT_EQ( "Other", metadata.get( libav::Metadata::GENRE ) );
    ASSERT_EQ( "1", metadata.get( libav::Metadata::TRACK ) );
}
TEST_F( FormatTest, ParseMpegLayer2Sample) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "sample.mp3" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 1U, _streams.size() );
    ASSERT_EQ( 64075, _streams.front().bitrate );
    ASSERT_EQ( 2, _streams.front().channels );
    ASSERT_EQ( 0, _streams.front().bits_per_sample );
    ASSERT_EQ( 44100, _streams.front().samplerate );
    ASSERT_EQ( 4, format.playtime() ); //playlength.

    ASSERT_STREQ( "Other", format.metadata().get( libav::Metadata::GENRE ).c_str() );
}
TEST_F( FormatTest, ParseFlacSample) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "sample.flac" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 1U, _streams.size() );
    ASSERT_EQ( 0, _streams.front().bitrate );
    ASSERT_EQ( 1, _streams.front().channels );
    ASSERT_EQ( 16, _streams.front().bits_per_sample );
    ASSERT_EQ( 8000, _streams.front().samplerate );
    ASSERT_EQ( 4, format.playtime() ); //playlength.
}
TEST_F( FormatTest, ParseAviSample) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "sample.avi" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 1U, _streams.size() );

    ASSERT_EQ( 256, _streams[0].width );
    ASSERT_EQ( 240, _streams[0].height );

    ASSERT_EQ( 6, format.playtime() );
}
TEST_F( FormatTest, ParseDivxMicayala_DivX1080p_ASP) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "Micayala_DivX1080p_ASP.divx" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 2U, _streams.size() );
    ASSERT_EQ( 192000, _streams[1].bitrate );
    ASSERT_EQ( 2, _streams[1].channels );
    ASSERT_EQ( 0, _streams[1].bits_per_sample );
    ASSERT_EQ( 44100, _streams[1].samplerate );

    ASSERT_EQ( 1920, _streams[0].width );
    ASSERT_EQ( 768, _streams[0].height );

    ASSERT_EQ( 137, format.playtime() );
}
TEST_F( FormatTest, ParseDivxWiegelesHeliSki_DivXPlus_19Mbps) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "WiegelesHeliSki_DivXPlus_19Mbps.mkv" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 2U, _streams.size() );
    ASSERT_EQ( 192000, _streams[1].bitrate );
    ASSERT_EQ( 2, _streams[1].channels );
    ASSERT_EQ( 0, _streams[1].bits_per_sample );
    ASSERT_EQ( 44100, _streams[1].samplerate );

    ASSERT_EQ( 1920, _streams[0].width );
    ASSERT_EQ( 1080, _streams[0].height );

    ASSERT_EQ( 220, format.playtime() );

    libav::Metadata _metadata = format.metadata();
    ASSERT_EQ( "Alterna Films 2012", _metadata.get ( libav::Metadata::TITLE ) );
    ASSERT_EQ( "Encoded in DivX Plus HD!", _metadata.get ( libav::Metadata::COMMENT ) );
}
TEST_F( FormatTest, DISABLED_ParseMkvCover ) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "cover_art.mkv" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 2U, _streams.size() );
    ASSERT_EQ( 0, _streams.front().bitrate );
    ASSERT_EQ( 2, _streams.front().channels );
    ASSERT_EQ( 0, _streams.front().bits_per_sample );
    ASSERT_EQ( 44100, _streams.front().samplerate );

    ASSERT_EQ( 1272, _streams[1].width );
    ASSERT_EQ( 720, _streams[1].height );

    ASSERT_EQ( 156, format.playtime() );

    libav::Metadata _metadata = format.metadata();
    ASSERT_STREQ( "Dexter Season 5 trailer", _metadata.get ( libav::Metadata::TITLE ).c_str() );
    ASSERT_STREQ( "", _metadata.get ( libav::Metadata::COMMENT ).c_str() );
}
}//namespace av
