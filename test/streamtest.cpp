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

#include "../libavcpp/codec.h"
#include "../libavcpp/format.h"
#include "../libavcpp/metadata.h"
#include "../libavcpp/packet.h"

#include "_testconstants.h"

#include <gtest/gtest.h>

namespace av {

TEST( StreamTest, open_audio_file ) {

    std::fstream _fs( FILE_FLAC, std::ifstream::in  );
    Format _format ( _fs );
    ASSERT_FALSE( _format );
    int audio=0, other=0;
    for( auto& __stream : _format ) {
        if(__stream.codec_type() == CODEC_TYPE::AUDIO ) ++audio;
        else ++other;
    }
    EXPECT_EQ( 1, audio );
    EXPECT_EQ( 5, other );
}
TEST( StreamTest, open_audio_decoder ) {

    std::fstream _fs( FILE_FLAC, std::ifstream::in  );
    Format _format ( _fs );
    ASSERT_FALSE( _format );

    int audio=0, other=0;

    for( auto& __codec : _format ) {
        if(__codec.codec_type() == CODEC_TYPE::AUDIO ) {
            EXPECT_EQ( CODEC::FLAC, __codec.codec() );
            ++audio;
        } else ++other;
    }
    EXPECT_EQ( 1, audio );
    EXPECT_EQ( 5, other );
}

//TEST( StreamTest, read_packages ) {
//    std::fstream _fs( FILE_FLAC, std::ifstream::in  );
//    Format _format ( _fs );
//    ASSERT_FALSE( _format );

//    Packet packet;
//    int audio_packets=0, other_packets=0;
//    std::error_code errc;
//    auto _codec( _format.find_codec( CODEC_TYPE::AUDIO ) );
//    Frame _frame;
//    while( !packet.decode( _codec, _frame) )
//        if( packet.stream_index() == 0 ) ++audio_packets;
//        else ++other_packets;

//    EXPECT_EQ( 2217, audio_packets );
//    EXPECT_EQ( 5, other_packets );
//}






////TEST( StreamTest, ParseMpegLayer2Mpthreetest) {

////    av::Format format;
////    std::string filename = std::string( TESTFILES ) + "mpthreetest.mp3";
////    //"mpthreetest.mp3"
////    std::fstream _fs( filename, std::ifstream::in  );
////    ASSERT_FALSE( !_fs );
////    ASSERT_EQ( std::error_code().message(), format.open( &_fs ).message() );

//////    ASSERT_EQ( 1U, format.streams().size() );
//////    auto codec = format.streams().front();
//////    ASSERT_EQ( 128000, codec->bitrate() );
//////    ASSERT_EQ( 1, codec->channels() );
//////    ASSERT_EQ( 0, codec->bits_per_sample() );
//////    ASSERT_EQ( 44100, codec->samplerate() );
//////    ASSERT_EQ( 0, format.playtime() ); //playlength.

//////TODO    av::Metadata metadata = format.metadata();
//////    ASSERT_EQ( "Test of MP3 File", metadata.get( av::Metadata::TITLE ) );
//////    ASSERT_EQ( "Me", metadata.get( av::Metadata::ALBUM ) );
//////    ASSERT_EQ( "test", metadata.get( av::Metadata::COMMENT ) );
//////    ASSERT_EQ( "Me", metadata.get( av::Metadata::ARTIST ) );
//////    ASSERT_EQ( "2006", metadata.get( av::Metadata::YEAR ) );
//////    ASSERT_EQ( "Other", metadata.get( av::Metadata::GENRE ) );
//////    ASSERT_EQ( "1", metadata.get( av::Metadata::TRACK ) );
////}

////TEST_F( StreamTest, DISABLED_transcode_audio_stream ) {
////    std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16.flac";
////    std::string dest_file  = "/tmp/file_out.mp3";

////    { //Transcode
////        Format _format;
////        ASSERT_EQ( std::error_code().message(), _format.open( filename ).message() );
////        ASSERT_EQ( std::error_code().message(), _format.open( dest_file, Format::WRITE ).message() );

////        EXPECT_EQ ( 6, _format.streams().size() );
////        auto _codec = _format.streams().front();

//////        _codec->samplerate ( 44100 );
//////        _codec->bitrate ( 192000 );
//////        _codec->codec ( Codec::CODEC::MP3 );

//////TODO        ASSERT_EQ( std::error_code().message(), _format.transcode( _codec ).message() );
////    }

////    { //open and test transcoded file
////        Format _format;
////        ASSERT_EQ( std::error_code().message(), _format.open( dest_file ).message() );
////        EXPECT_EQ ( 205, _format.playtime() );
////        EXPECT_EQ ( 1U, _format.streams().size() );

//////        auto _codec = _format.streams().front();
//////        EXPECT_EQ ( av::Codec::CODEC_TYPE::AUDIO, _codec->codec_type() );
//////        EXPECT_EQ ( av::Codec::CODEC::MP3, _codec->codec() );
//////        EXPECT_EQ ( 192000, _codec->bitrate() );
//////        EXPECT_EQ ( 44100, _codec->samplerate() );
//////        EXPECT_EQ ( 2, _codec->channels() );
//////        EXPECT_EQ ( 0, _codec->bits_per_sample() );
//////        EXPECT_EQ ( 0, _codec->width() );
//////        EXPECT_EQ ( 0, _codec->height() );

////        av::Metadata _metadata = _format.metadata();
////        EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _metadata.get( Metadata::ALBUM ) );
////        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _metadata.get( Metadata::ARTIST ) );
////        EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _metadata.get( Metadata::COMMENT ) );
////        EXPECT_EQ ( "Purcell, Henry", _metadata.get( Metadata::COMPOSER ) );
////        EXPECT_EQ ( "1/1", _metadata.get( Metadata::DISC) );
////        EXPECT_EQ ( "Classical", _metadata.get( Metadata::GENRE ) );
////        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _metadata.get( Metadata::PERFORMER ) );
////        EXPECT_EQ ( "BIS;eClassical", _metadata.get( Metadata::PUBLISHER ) );
////        EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _metadata.get( Metadata::TITLE ) );
////        EXPECT_EQ ( "1/19", _metadata.get( Metadata::TRACK ) );
////        EXPECT_EQ ( "2007-08-30", _metadata.get( Metadata::YEAR ) );
////    }
////}
}//namespace av
