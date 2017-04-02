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

TEST_F( FormatTest, DISABLED_transcode_audio_file ) {
    std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16-nocover.flac";
    std::string dest_file  = "/tmp/file_out.mp3";

    { //Transcode
        Format _format;
        ASSERT_EQ( std::error_code().message(), _format.open( filename ).message() );
        ASSERT_EQ( std::error_code().message(), _format.open( filename, Format::WRITE ).message() );

        EXPECT_EQ ( 1, _format.streams().size() );
        av::Stream _stream = _format.streams().front();

        _stream.samplerate ( 44100 );
        _stream.bitrate ( 192000 );
        _stream.codec ( Codec::MP3 );

        ASSERT_EQ( std::error_code().message(), _format.transcode( _stream ).message() );
    }

    { //open and test transcoded file
        Format _format;
        ASSERT_EQ( std::error_code().message(), _format.open( dest_file ).message() );
        EXPECT_EQ ( 205, _format.playtime() );
        EXPECT_EQ ( 1U, _format.streams().size() );

        av::Stream _stream = _format.streams().front();
        EXPECT_EQ ( av::CodecType::AUDIO, _stream.codec_type() );
        EXPECT_EQ ( av::Codec::MP3, _stream.codec() );
        EXPECT_EQ ( 192000, _stream.bitrate() );
        EXPECT_EQ ( 44100, _stream.samplerate() );
        EXPECT_EQ ( 2, _stream.channels() );
        EXPECT_EQ ( 0, _stream.bits_per_sample() );
        EXPECT_EQ ( 0, _stream.width() );
        EXPECT_EQ ( 0, _stream.height() );

        av::Metadata _metadata = _format.metadata();
        EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _metadata.get( Metadata::ALBUM ) );
        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _metadata.get( Metadata::ARTIST ) );
        EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _metadata.get( Metadata::COMMENT ) );
        EXPECT_EQ ( "Purcell, Henry", _metadata.get( Metadata::COMPOSER ) );
        EXPECT_EQ ( "1/1", _metadata.get( Metadata::DISC) );
        EXPECT_EQ ( "Classical", _metadata.get( Metadata::GENRE ) );
        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _metadata.get( Metadata::PERFORMER ) );
        EXPECT_EQ ( "BIS;eClassical", _metadata.get( Metadata::PUBLISHER ) );
        EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _metadata.get( Metadata::TITLE ) );
        EXPECT_EQ ( "1/19", _metadata.get( Metadata::TRACK ) );
        EXPECT_EQ ( "2007-08-30", _metadata.get( Metadata::YEAR ) );
    }
}
}//namespace av
