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

#include <functional>
#include <string>
#include <fstream>
#include <map>

#include <boost/filesystem.hpp>

#include "../libavcpp.h"

#include "_testconstants.h"

#include <gtest/gtest.h>

namespace av {

class FormatTest : public ::testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

  static void delete_files () {
      if( boost::filesystem::exists( "/tmp/out-iostream.mp3" ) )
          boost::filesystem::remove( "/tmp/out-iostream.mp3" );

      if( boost::filesystem::exists( "/tmp/file_out.mp3" ) )
          boost::filesystem::remove( "/tmp/file_out.mp3" );

      if( boost::filesystem::exists( "/tmp/file_istream_out.mp3" ) )
          boost::filesystem::remove( "/tmp/file_istream_out.mp3" );
}
};

TEST_F( FormatTest, open ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );

    int audio=0, other=0;
    std::for_each( _format.begin(), _format.end(), [&audio,&other]( Codec& codec ) {
        if( codec.codec_type() == CODEC_TYPE::AUDIO ) ++audio;
        else ++other;
    });

    EXPECT_EQ( 1, audio );
    EXPECT_EQ( 5, other );
}

TEST_F( FormatTest, codec_types ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );

    auto _format_iterator = _format.begin();
    EXPECT_EQ( CODEC_TYPE::AUDIO, _format_iterator->codec_type() );
    _format_iterator++;
    EXPECT_EQ( CODEC_TYPE::VIDEO, _format_iterator->codec_type() );
    _format_iterator++;
    EXPECT_EQ( CODEC_TYPE::VIDEO, _format_iterator->codec_type() );
    _format_iterator++;
    EXPECT_EQ( CODEC_TYPE::VIDEO, _format_iterator->codec_type() );
    _format_iterator++;
    EXPECT_EQ( CODEC_TYPE::VIDEO, _format_iterator->codec_type() );
    _format_iterator++;
    EXPECT_EQ( CODEC_TYPE::VIDEO, _format_iterator->codec_type() );
}


TEST_F( FormatTest, find_codec ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );
    auto _codec = _format.find_codec( CODEC_TYPE::AUDIO );
    EXPECT_EQ( CODEC::FLAC, _codec->codec() );
}

TEST_F( FormatTest, iterate_codec ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );

    int audio=0, other=0;
    std::for_each( _format.begin(), _format.end(), [&audio,&other]( Codec& codec ) {
        if( codec.codec_type() == CODEC_TYPE::AUDIO ) ++audio;
        else ++other;
    });

    EXPECT_EQ( 1, audio );
    EXPECT_EQ( 5, other );
}

TEST_F( FormatTest, load_codec ) {
    Format _format ( FILE_SAMPLE_MP3 );
    ASSERT_FALSE( _format );

    auto _codec = _format.find_codec( CODEC_TYPE::AUDIO );
    EXPECT_EQ ( CODEC_TYPE::AUDIO, _codec->codec_type() );
    EXPECT_EQ ( CODEC::MP3, _codec->codec() );
    EXPECT_EQ ( 64075, _codec->bitrate() );
    EXPECT_EQ ( 44100, _codec->sample_rate() );
    EXPECT_EQ ( 2, _codec->channels() );
    EXPECT_EQ ( 0, _codec->bits_per_sample() );
    EXPECT_EQ ( 0, _codec->width() );
    EXPECT_EQ ( 0, _codec->height() );
}

TEST_F( FormatTest, read_packages ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );

    int audio_packets=0, other_packets=0;
    Packet _packet;
    while( !_format.read( _packet ) ) {
        if( _packet.stream_index() == 0 )
            ++audio_packets;
        else
            ++other_packets;
    }
    EXPECT_EQ( "End of file", _format.errc().message() );
    EXPECT_EQ( 2217, audio_packets );
    EXPECT_EQ( 5, other_packets );
}

TEST_F( FormatTest, read_filter_packages ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );

    auto codec = _format.find_codec( CODEC_TYPE::AUDIO );
    int audio_packets=0;
    Packet _packet;
    while( !_format.read( codec, _packet ) ) {
        EXPECT_EQ( CODEC::FLAC, codec->codec() );
        ++audio_packets;
    }
    EXPECT_EQ( "End of file", _format.errc().message() );
    EXPECT_EQ( 2217, audio_packets );
}

TEST_F( FormatTest, decode ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );

    int _audio_frames=0;
    Packet _packet;
    while( !_format.read( _format.find_codec( CODEC_TYPE::AUDIO ), _packet ) ) {
        Frame _frame;
        if( !_format.decode( _packet, _frame ) ) {
            ++_audio_frames;
        } else ASSERT_FALSE( true );
    }
    EXPECT_EQ( "End of file", _format.errc().message() );
    EXPECT_EQ( 2217, _audio_frames );
}

TEST_F( FormatTest, resample_frames ) {

    Format _format( FILE_FLAC_HD );
    ASSERT_EQ( std::error_code().message(), _format.errc().message() );

    auto _codec = _format.find_codec( CODEC_TYPE::AUDIO );
    Resample _res(
        _codec->channels(), _codec->sample_fmt(), _codec->sample_rate(),
        _codec->channels(), _codec->sample_fmt(), _codec->sample_rate(),
        options_t()
    );

    AudioFifo _fifo( _codec->sample_fmt(), _codec->channels() );

    int audio_packets=0, _audio_frames=0;
    Packet _packet;
    while( !_format.read( _format.find_codec( CODEC_TYPE::AUDIO ), _packet ) ) {
        Frame _frame;
        if( !_format.decode( _packet, _frame ) ) {
            ++_audio_frames;
            std::cout << "Frame#: " << _audio_frames << std::endl;
            _fifo.write( _res, _frame );

            //use fifo...

        } else ASSERT_FALSE( true );
    }
    EXPECT_EQ( "End of file", _format.errc().message() );
    EXPECT_EQ( 514997648, _fifo.nb_samples() );
    EXPECT_EQ( 2222, audio_packets );
    EXPECT_EQ( 9078720, _audio_frames );
}

//TEST_F( FormatTest, transcode_audio_file ) {

//    std::string dest_file  = "/tmp/file_out.mp3";

//    { //Transcode
//        Format _format ( FILE_FLAC );
//        Format _target_format(  dest_file, Format::WRITE );
//        ASSERT_EQ( std::error_code().message(), _format.errc().message() );
//        ASSERT_EQ( std::error_code().message(), _target_format.errc().message() );

//        Codec _codec;
//        int _index = 0;
//        for( auto& __codec : _format ) {
//            if( __codec->codec_type() == CODEC_TYPE::AUDIO )
//            { _codec = Codec( *__codec ); break; }
//            ++_index;
//        }

//        Codec _encoder;

//        Resample _resampler(
//            _codec.channels(), _codec.sample_format(), _codec.samplerate(),
//            _codec.channels(), _codec.sample_format(), _codec.samplerate(),
//            options_t() );

//        AudioFifo _fifo( _codec.sample_format(), _codec.channels() );

//        Packet _packet = Packet();
//        int samples=0;
//        while( !_format.read( _packet ) ) {
//            if( _packet.stream_index() == _index ) {
//                Frame _frame;
//                if( !_packet.decode( _codec, _frame ) ) {
//                    samples += _frame.nb_samples();
//                    _fifo.write( _resampler, _frame );

//                    //write audio data
//                    //TODO _target_format.write( )
//                }
//            }
//        }
//        //TODO EXPECT_EQ( std::error_code().message(), errc.message() );
//        EXPECT_EQ( 9078720, samples );





////        Format _format;
////        ASSERT_EQ( std::error_code().message(), _format.open( filename ).message() );
////        ASSERT_EQ( std::error_code().message(), _format.open( dest_file, Format::WRITE ).message() );

////        EXPECT_EQ ( 6U, _format.streams().size() );
////        av::Codec _codec = _format.streams().front();

////        _codec.samplerate ( 44100 );
////        _codec.bitrate ( 192000 );
////        _codec.codec ( Codec::CODEC::MP3 );

////        ASSERT_EQ( std::error_code().message(), _format.transcode( _codec ).message() );
//    }

//    { //open and test transcoded file
//        Format _format ( dest_file );
//        ASSERT_EQ( std::error_code().message(), _format.errc().message() );
//        EXPECT_EQ ( 205U, _format.playtime() );

//        Codec _codec;
//        int _index = 0;
//        for( auto& __codec : _format ) {
//            if( __codec->codec_type() == CODEC_TYPE::AUDIO )
//            { _codec = Codec( *__codec ); break; }
//            ++_index;
//        }
//        EXPECT_EQ ( 1U, _index );

//        EXPECT_EQ ( av::CODEC_TYPE::AUDIO, _codec.codec_type() );
//        EXPECT_EQ ( av::CODEC::MP3, _codec.codec() );
//        EXPECT_EQ ( 192000, _codec.bitrate() );
//        EXPECT_EQ ( 44100, _codec.samplerate() );
//        EXPECT_EQ ( 2, _codec.channels() );
//        EXPECT_EQ ( 0, _codec.bits_per_sample() );
//        EXPECT_EQ ( 0, _codec.width() );
//        EXPECT_EQ ( 0, _codec.height() );

//        av::Metadata _metadata = _format.metadata();
//        EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _metadata.get( Metadata::ALBUM ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _metadata.get( Metadata::ARTIST ) );
//        EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _metadata.get( Metadata::COMMENT ) );
//        EXPECT_EQ ( "Purcell, Henry", _metadata.get( Metadata::COMPOSER ) );
//        EXPECT_EQ ( "1/1", _metadata.get( Metadata::DISC) );
//        EXPECT_EQ ( "Classical", _metadata.get( Metadata::GENRE ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _metadata.get( Metadata::PERFORMER ) );
//        EXPECT_EQ ( "BIS;eClassical", _metadata.get( Metadata::PUBLISHER ) );
//        EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _metadata.get( Metadata::TITLE ) );
//        EXPECT_EQ ( "1/19", _metadata.get( Metadata::TRACK ) );
//        EXPECT_EQ ( "2007-08-30", _metadata.get( Metadata::YEAR ) );
//    }
//}
}//namespace av
