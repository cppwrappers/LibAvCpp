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
#include "../libavcpp/_constants.h"

#include <gtest/gtest.h>

#include "_testconstants.h"

namespace av {

TEST( MetadataTest, open_filename ) {

    Format _format( FILE_SAMPLE_MP3 );
    ASSERT_FALSE( _format );

    int found_codec = 0;
    Codec _codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 1, found_codec );
    EXPECT_EQ ( 4, _format.playtime() );

    EXPECT_EQ ( CODEC_TYPE::AUDIO, _codec.codec_type() );
    EXPECT_EQ ( CODEC::MP3, _codec.codec() );
    EXPECT_EQ ( 64075, _codec.bitrate() );
    EXPECT_EQ ( 44100, _codec.samplerate() );
    EXPECT_EQ ( 2, _codec.channels() );
    EXPECT_EQ ( 0, _codec.bits_per_sample() );
    EXPECT_EQ ( 0, _codec.width() );
    EXPECT_EQ ( 0, _codec.height() );
}
TEST( MetadataTest, metadata ) {
    Format _format( FILE_SAMPLE_MP3 );
    ASSERT_FALSE( _format );

    av::Metadata _metadata = _format.metadata();
    EXPECT_EQ( "", _metadata.get( av::Metadata::TITLE ) );
}
TEST( MetadataTest, time_to_string ) {
    EXPECT_EQ( "00:00:01.000", time_to_string( 1000 ) );
    EXPECT_EQ( "01:00:00.000", time_to_string( 3600000 ) );
    EXPECT_EQ( "02:02:28.015", time_to_string( 7348015 ) );
}
TEST( MetadataTest, ParseMpegLayer2Mpthreetest) {

    Format _format( FILE_MPEGTHREE );
    ASSERT_FALSE( _format );

    int found_codec = 0;
    Codec _codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 1, found_codec );
    EXPECT_EQ( 128000, _codec.bitrate() );
    EXPECT_EQ( 1, _codec.channels() );
    EXPECT_EQ( 0, _codec.bits_per_sample() );
    EXPECT_EQ( 44100, _codec.samplerate() );
    EXPECT_EQ( 12, _format.playtime() ); //playlength.

    av::Metadata metadata = _format.metadata();
    EXPECT_EQ( "Test of MP3 File", metadata.get( av::Metadata::TITLE ) );
    EXPECT_EQ( "Me", metadata.get( av::Metadata::ALBUM ) );
    EXPECT_EQ( "test", metadata.get( av::Metadata::COMMENT ) );
    EXPECT_EQ( "Me", metadata.get( av::Metadata::ARTIST ) );
    EXPECT_EQ( "2006", metadata.get( av::Metadata::YEAR ) );
    EXPECT_EQ( "Other", metadata.get( av::Metadata::GENRE ) );
    EXPECT_EQ( "1", metadata.get( av::Metadata::TRACK ) );
}
TEST( MetadataTest, ParseFlacSample) {

    Format _format( FILE_SAMPLE_FLAC );
    ASSERT_FALSE( _format );

    int found_codec = 0;
    Codec _codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 1, found_codec );
    EXPECT_EQ( 0, _codec.bitrate() );
    EXPECT_EQ( 1, _codec.channels() );
    EXPECT_EQ( 16, _codec.bits_per_sample() );
    EXPECT_EQ( 8000, _codec.samplerate() );
    EXPECT_EQ( 4, _format.playtime() ); //playlength.
}
TEST( MetadataTest, ParseAviSample) {

    Format _format( std::string( TESTFILES ) + "/sample.avi" );
    ASSERT_FALSE( _format );

    int found_codec = 0;
    Codec _codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::VIDEO ) {
            _codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 1, found_codec );

    EXPECT_EQ( 256, _codec.width() );
    EXPECT_EQ( 240, _codec.height() );

    EXPECT_EQ( 6, _format.playtime() );
}
TEST( MetadataTest, ParseDivxMicayala_DivX1080p_ASP) {

    Format _format( std::string( TESTFILES ) + "/Micayala_DivX1080p_ASP.divx" );
    ASSERT_FALSE( _format );

    int found_codec = 0;
    Codec _audio_codec, _video_codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::VIDEO ) {
            _video_codec = Codec( _f->index(), *_f );
        } else if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _audio_codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 2, found_codec );

    EXPECT_EQ( 192000, _audio_codec.bitrate() );
    EXPECT_EQ( 2, _audio_codec.channels() );
    EXPECT_EQ( 0, _audio_codec.bits_per_sample() );
    EXPECT_EQ( 44100, _audio_codec.samplerate() );

    EXPECT_EQ( 1920, _video_codec.width() );
    EXPECT_EQ( 768, _video_codec.height() );

    EXPECT_EQ( 137, _format.playtime() );
}
TEST( MetadataTest, ParseDivxWiegelesHeliSki_DivXPlus_19Mbps) {

    Format _format( std::string( TESTFILES ) + "/WiegelesHeliSki_DivXPlus_19Mbps.mkv" );
    ASSERT_FALSE( _format );

    int found_codec = 0;
    Codec _audio_codec, _video_codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::VIDEO ) {
            _video_codec = Codec( _f->index(), *_f );
        } else if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _audio_codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 2, found_codec );
    EXPECT_EQ( 192000, _audio_codec.bitrate() );
    EXPECT_EQ( 2, _audio_codec.channels() );
    EXPECT_EQ( 0, _audio_codec.bits_per_sample() );
    EXPECT_EQ( 44100, _audio_codec.samplerate() );

    EXPECT_EQ( 1920, _video_codec.width() );
    EXPECT_EQ( 1080, _video_codec.height() );

    EXPECT_EQ( 220, _format.playtime() );

    av::Metadata _metadata = _format.metadata();
    EXPECT_EQ( "Alterna Films 2012", _metadata.get ( av::Metadata::TITLE ) );
    EXPECT_EQ( "Encoded in DivX Plus HD!", _metadata.get ( av::Metadata::COMMENT ) );
}
TEST( MetadataTest, ParseMkvCover ) {

    Format _format( std::string( TESTFILES ) + "/cover_art.mkv" );
    ASSERT_FALSE( _format );

    int found_codec = 0;
    Codec _audio_codec, _video_codec;
    bool _video_found = false;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::VIDEO ) {
            if( !_video_found ) {
                _video_codec = Codec( _f->index(), *_f );
                _video_found = true;
            }
        } else if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _audio_codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 6, found_codec );
    EXPECT_EQ( 0, _audio_codec.bitrate() );
    EXPECT_EQ( 2, _audio_codec.channels() );
    EXPECT_EQ( 0, _audio_codec.bits_per_sample() );
    EXPECT_EQ( 44100, _audio_codec.samplerate() );

    EXPECT_EQ( 1272, _video_codec.width() );
    EXPECT_EQ( 720, _video_codec.height() );

    EXPECT_EQ( 156, _format.playtime() );

    av::Metadata _metadata = _format.metadata();
    ASSERT_STREQ( "Dexter Season 5 trailer", _metadata.get ( av::Metadata::TITLE ).c_str() );
    ASSERT_STREQ( "", _metadata.get ( av::Metadata::COMMENT ).c_str() );

    //TODO get cover.
}
TEST( MetadataTest, open_audio_file_metadata_file_flac ) {

    Format _format( std::string( TESTFILES ) + "/BIS1536-001-flac_16.flac" );
    ASSERT_FALSE( _format );
    EXPECT_EQ ( 205, _format.playtime() );

    int found_codec = 0;
    Codec _audio_codec, _video_codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::VIDEO ) {
            _video_codec = Codec( _f->index(), *_f );
        } else if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _audio_codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 6, found_codec );

    EXPECT_EQ ( av::CODEC_TYPE::AUDIO, _audio_codec.codec_type() );
    EXPECT_EQ ( av::CODEC::FLAC, _audio_codec.codec() );
    EXPECT_EQ ( 0, _audio_codec.bitrate() );
    EXPECT_EQ ( 44100, _audio_codec.samplerate() );
    EXPECT_EQ ( 2, _audio_codec.channels() );
    EXPECT_EQ ( 16, _audio_codec.bits_per_sample() );
    EXPECT_EQ ( 0, _audio_codec.width() );
    EXPECT_EQ ( 0, _audio_codec.height() );

    av::Metadata metadata = _format.metadata();
    EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", metadata.get( av::Metadata::ALBUM ) );
    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", metadata.get( av::Metadata::ARTIST ) );
    EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", metadata.get( av::Metadata::COMMENT ) );
    EXPECT_EQ ( "Purcell, Henry", metadata.get( av::Metadata::COMPOSER ) );
    EXPECT_EQ ( "1/1", metadata.get( av::Metadata::DISC) );
    EXPECT_EQ ( "(32)Vocal/Choral;(32)Baroque", metadata.get( av::Metadata::GENRE ) );
    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", metadata.get( av::Metadata::PERFORMER ) );
    EXPECT_EQ ( "BIS;eClassical", metadata.get( av::Metadata::PUBLISHER ) );
    EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", metadata.get( av::Metadata::TITLE ) );
    EXPECT_EQ ( "1/19", metadata.get( av::Metadata::TRACK ) );
    EXPECT_EQ ( "2007", metadata.get( av::Metadata::YEAR ) );
}
TEST( MetadataTest, open_audio_file_metadata_file_flac_hd ) {

    Format _format( std::string( TESTFILES ) + "/BIS1536-001-flac_24.flac" );
    ASSERT_FALSE( _format );
    EXPECT_EQ ( 205, _format.playtime() );

    int found_codec = 0;
    Codec _audio_codec, _video_codec;
    for( auto _f : _format ) {
        ++found_codec;
        if( _f->codec_type() == CODEC_TYPE::VIDEO ) {
            _video_codec = Codec( _f->index(), *_f );
        } else if( _f->codec_type() == CODEC_TYPE::AUDIO ) {
            _audio_codec = Codec( _f->index(), *_f );
        }
    }
    EXPECT_EQ ( 5, found_codec );

    EXPECT_EQ ( av::CODEC_TYPE::AUDIO, _audio_codec.codec_type() );
    EXPECT_EQ ( av::CODEC::FLAC, _audio_codec.codec() );
    EXPECT_EQ ( 0, _audio_codec.bitrate() );
    EXPECT_EQ ( 44100, _audio_codec.samplerate() );
    EXPECT_EQ ( 2, _audio_codec.channels() );
    EXPECT_EQ ( 24, _audio_codec.bits_per_sample() );
    EXPECT_EQ ( 0, _audio_codec.width() );
    EXPECT_EQ ( 0, _audio_codec.height() );

    av::Metadata metadata = _format.metadata();
    EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", metadata.get( av::Metadata::ALBUM ) );
    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", metadata.get( av::Metadata::ARTIST ) );
    EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", metadata.get( av::Metadata::COMMENT ) );
    EXPECT_EQ ( "Purcell, Henry", metadata.get( av::Metadata::COMPOSER ) );
    EXPECT_EQ ( "1/1", metadata.get( av::Metadata::DISC) );
    EXPECT_EQ ( "(32)Vocal/Choral;(32)Baroque", metadata.get( av::Metadata::GENRE ) );
    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", metadata.get( av::Metadata::PERFORMER ) );
    EXPECT_EQ ( "BIS;eClassical", metadata.get( av::Metadata::PUBLISHER ) );
    EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", metadata.get( av::Metadata::TITLE ) );
    EXPECT_EQ ( "1/19", metadata.get( av::Metadata::TRACK ) );
    EXPECT_EQ ( "2007", metadata.get( av::Metadata::YEAR ) );
}
}//namespace av
