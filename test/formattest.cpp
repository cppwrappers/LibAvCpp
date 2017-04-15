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

#include "../libavcpp.h"

#include "_testconstants.h"

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

TEST_F( FormatTest, open_audio_file ) {

    Format _format ( FILE_FLAC );
    ASSERT_FALSE( _format );

    int audio=0, other=0;
    for( auto& __stream : _format ) {
        if(__stream->codec_type() == CODEC_TYPE::AUDIO ) ++audio;
        else ++other;
    }
    EXPECT_EQ( 1, audio );
    EXPECT_EQ( 5, other );
}

TEST_F( FormatTest, open_audio_decoder ) {

    Format _format ( FILE_FLAC );
    ASSERT_EQ( std::error_code().message(), _format.errc().message() );

    int audio=0, other=0;
    Codec codec;
    for( auto& __stream : _format ) {
        if(__stream->codec_type() == CODEC_TYPE::AUDIO ) {
            codec = Codec( *__stream );

            EXPECT_EQ( CODEC::FLAC, codec.codec() );
            ++audio;
        }
        else ++other;
    }
    EXPECT_EQ( 1, audio );
    EXPECT_EQ( 5, other );
}

TEST_F( FormatTest, read_packages ) {

    Format _format ( FILE_FLAC );
    ASSERT_EQ( std::error_code().message(), _format.errc().message() );

    Packet _packet;
    int audio_packets=0, other_packets=0;
    while( !_format.read( _packet ) ) {
        if( _packet.stream_index() == 0 )
            ++audio_packets;
        else
            ++other_packets;

    }
    EXPECT_EQ( 2217, audio_packets );
    EXPECT_EQ( 5, other_packets );
}

TEST_F( FormatTest, read_frames ) {

    Format _format ( FILE_FLAC );
    ASSERT_EQ( std::error_code().message(), _format.errc().message() );

    Codec _codec;
    for( auto& __codec : _format ) {
        if( __codec->codec_type() == CODEC_TYPE::AUDIO )
        { _codec = Codec( __codec->index(), *__codec ); break; }
    }

    Resample _res(
        _codec.channels(), _codec.sample_format(), _codec.samplerate(),
        _codec.channels(), _codec.sample_format(), _codec.samplerate(),
        options_t() );

    AudioFifo _fifo( _codec.sample_format(), _codec.channels() );

    Packet _packet = Packet();
    int samples=0, packets=0;
    while( !_format.read( _packet ) ) {
        ++packets;
        if( _packet.stream_index() == _codec.index() ) {
            Frame _frame;
            if( !_packet.decode( _codec, _frame ) ) {
                samples += _frame.nb_samples();
                _fifo.write( _res, _frame );
            } else std::cout << _packet.errc().message() << std::endl;
        }
    }
    EXPECT_EQ( "End of file", _format.errc().message() );
    EXPECT_EQ( 2222, packets );
    EXPECT_EQ( 9078720, samples );
}

//TEST_F( FormatTest, resample_frames ) {
//    std::error_code errc;

//    Format _format; // = Format( Format::LOG_LEVEL::TRACE );
//    ASSERT_EQ( std::error_code().message(), _format.open( source_file ).message() );
//    ASSERT_EQ( std::error_code().message(), _format.open( dest_file, Format::WRITE ).message() );

//    codec_ptr _input_codec = _format.codec( &errc, 0 );
//    ASSERT_TRUE( _input_codec );
//    ASSERT_EQ( std::error_code().message(), errc.message() );

//    codec_ptr _output_codec = _format.codec( &errc, Codec::CODEC::MP3, {{ "b", 128000 }, {"ac", 2}, {"ar", 44100 }} );
//    ASSERT_TRUE( _output_codec );
//    ASSERT_EQ( std::error_code().message(), errc.message() );

//    resample_ptr _resample = _format.resample( &errc, _input_codec, _output_codec );
//    ASSERT_TRUE( _resample );
//    ASSERT_EQ( std::error_code().message(), errc.message() );

//    packet_ptr _packet = nullptr;
//    int samples=0;
//    while( ( _packet = _format.read( &errc ) ) && !errc ) {
//        if( _packet->stream_index() == 0 ) {
//            frame_ptr _frame = _packet->read( &errc, _input_codec );
//            samples += _frame->nb_samples();

////            Fifo _fifo();
////            _resample->resample( &errc, _fifo, _frame );
//        }
//    }
//    EXPECT_EQ( AV_EOF, errc.value() );
//    EXPECT_EQ( 9078720, samples );
//}

TEST_F( FormatTest, DISABLED_transcode_audio_file ) {
    std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16.flac";
    std::string dest_file  = "/tmp/file_out.mp3";

    { //Transcode
        Format _format ( FILE_FLAC );
        Format _target_format(  dest_file, Format::WRITE );
        ASSERT_EQ( std::error_code().message(), _format.errc().message() );
        ASSERT_EQ( std::error_code().message(), _target_format.errc().message() );

        Codec _codec;
        int _index = 0;
        for( auto& __codec : _format ) {
            if( __codec->codec_type() == CODEC_TYPE::AUDIO )
            { _codec = Codec( *__codec ); break; }
            ++_index;
        }

        Resample _res(
            _codec.channels(), _codec.sample_format(), _codec.samplerate(),
            _codec.channels(), _codec.sample_format(), _codec.samplerate(),
            options_t() );

        AudioFifo _fifo( _codec.sample_format(), _codec.channels() );

        Packet _packet = Packet();
        int samples=0;
        while( !_format.read( _packet ) ) {
            if( _packet.stream_index() == 0 ) {
                Frame _frame;
                if( !_packet.decode( _codec, _frame ) ) {
                    samples += _frame.nb_samples();
                    _fifo.write( _res, _frame );

                    //write audio data

                }
            }
        }
        //TODO EXPECT_EQ( std::error_code().message(), errc.message() );
        EXPECT_EQ( 9078720, samples );





//        Format _format;
//        ASSERT_EQ( std::error_code().message(), _format.open( filename ).message() );
//        ASSERT_EQ( std::error_code().message(), _format.open( dest_file, Format::WRITE ).message() );

//        EXPECT_EQ ( 6U, _format.streams().size() );
//        av::Codec _codec = _format.streams().front();

//        _codec.samplerate ( 44100 );
//        _codec.bitrate ( 192000 );
//        _codec.codec ( Codec::CODEC::MP3 );

//        ASSERT_EQ( std::error_code().message(), _format.transcode( _codec ).message() );
    }

    { //open and test transcoded file
        Format _format ( dest_file );
        ASSERT_EQ( std::error_code().message(), _format.errc().message() );
        EXPECT_EQ ( 205U, _format.playtime() );

        Codec _codec;
        int _index = 0;
        for( auto& __codec : _format ) {
            if( __codec->codec_type() == CODEC_TYPE::AUDIO )
            { _codec = Codec( *__codec ); break; }
            ++_index;
        }
        EXPECT_EQ ( 1U, _index );

        EXPECT_EQ ( av::CODEC_TYPE::AUDIO, _codec.codec_type() );
        EXPECT_EQ ( av::CODEC::MP3, _codec.codec() );
        EXPECT_EQ ( 192000, _codec.bitrate() );
        EXPECT_EQ ( 44100, _codec.samplerate() );
        EXPECT_EQ ( 2, _codec.channels() );
        EXPECT_EQ ( 0, _codec.bits_per_sample() );
        EXPECT_EQ ( 0, _codec.width() );
        EXPECT_EQ ( 0, _codec.height() );

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
