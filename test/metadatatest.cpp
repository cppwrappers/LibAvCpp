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

TEST( MetadataTest, open_filename ) {
    Format _format;
    ASSERT_EQ( std::error_code().message(), _format.open( std::string( TESTFILES ) + "/sample.mp3" ).message() );

    EXPECT_EQ ( 1, _format.streams().size() );
    EXPECT_EQ ( 4, _format.playtime() );

    av::Stream _stream = _format.streams().front();
    EXPECT_EQ ( av::CodecType::AUDIO, _stream.codec_type() );
    EXPECT_EQ ( av::Codec::MP3, _stream.codec() );
    EXPECT_EQ ( 64075, _stream.bitrate() );
    EXPECT_EQ ( 44100, _stream.samplerate() );
    EXPECT_EQ ( 2, _stream.channels() );
    EXPECT_EQ ( 0, _stream.bits_per_sample() );
    EXPECT_EQ ( 0, _stream.width() );
    EXPECT_EQ ( 0, _stream.height() );
}
TEST( MetadataTest, codec_type ) {
    Format _format;
    std::error_code _code = _format.open( std::string( TESTFILES ) + "/sample.mp3" );
    EXPECT_FALSE( _code );

    std::vector< av::Stream > _streams = _format.streams();
    EXPECT_EQ( 1U, _streams.size() );
    EXPECT_EQ( CodecType::AUDIO, _streams[0].codec_type() );
}
TEST( MetadataTest, metadata ) { //TODO file with metadata
    Format _format;
    std::error_code _code = _format.open( std::string( TESTFILES ) + "/sample.mp3" );
    EXPECT_FALSE( _code );

    av::Metadata _metadata = _format.metadata();
    EXPECT_EQ( "", _metadata.get( av::Metadata::TITLE ) );
}
TEST( MetadataTest, time_to_string ) {
    EXPECT_EQ( "00:00:01.000", Format::time_to_string( 1000 ) );
    EXPECT_EQ( "01:00:00.000", Format::time_to_string( 3600000 ) );
    EXPECT_EQ( "02:02:28.015", Format::time_to_string( 7348015 ) );
}
TEST( MetadataTest, ParseMpegLayer2Mpthreetest) {

    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "mpthreetest.mp3" ).message() );

    ASSERT_EQ( 1U, format.streams().size() );
    av::Stream stream = format.streams().front();
    ASSERT_EQ( 128000, stream.bitrate() );
    ASSERT_EQ( 1, stream.channels() );
    ASSERT_EQ( 0, stream.bits_per_sample() );
    ASSERT_EQ( 44100, stream.samplerate() );
    ASSERT_EQ( 12, format.playtime() ); //playlength.

    av::Metadata metadata = format.metadata();
    ASSERT_EQ( "Test of MP3 File", metadata.get( av::Metadata::TITLE ) );
    ASSERT_EQ( "Me", metadata.get( av::Metadata::ALBUM ) );
    ASSERT_EQ( "test", metadata.get( av::Metadata::COMMENT ) );
    ASSERT_EQ( "Me", metadata.get( av::Metadata::ARTIST ) );
    ASSERT_EQ( "2006", metadata.get( av::Metadata::YEAR ) );
    ASSERT_EQ( "Other", metadata.get( av::Metadata::GENRE ) );
    ASSERT_EQ( "1", metadata.get( av::Metadata::TRACK ) );
}
TEST( MetadataTest, ParseMpegLayer2Sample) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "sample.mp3" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 1U, _streams.size() );
    ASSERT_EQ( 64075, _streams.front().bitrate() );
    ASSERT_EQ( 2, _streams.front().channels() );
    ASSERT_EQ( 0, _streams.front().bits_per_sample() );
    ASSERT_EQ( 44100, _streams.front().samplerate() );
    ASSERT_EQ( 4, format.playtime() ); //playlength.

    ASSERT_STREQ( "Other", format.metadata().get( av::Metadata::GENRE ).c_str() );
}
TEST( MetadataTest, ParseFlacSample) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "sample.flac" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 1U, _streams.size() );
    ASSERT_EQ( 0, _streams.front().bitrate() );
    ASSERT_EQ( 1, _streams.front().channels() );
    ASSERT_EQ( 16, _streams.front().bits_per_sample() );
    ASSERT_EQ( 8000, _streams.front().samplerate() );
    ASSERT_EQ( 4, format.playtime() ); //playlength.
}
TEST( MetadataTest, ParseAviSample) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "sample.avi" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 1U, _streams.size() );

    ASSERT_EQ( 256, _streams[0].width() );
    ASSERT_EQ( 240, _streams[0].height() );

    ASSERT_EQ( 6, format.playtime() );
}
TEST( MetadataTest, ParseDivxMicayala_DivX1080p_ASP) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "Micayala_DivX1080p_ASP.divx" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 2U, _streams.size() );
    ASSERT_EQ( 192000, _streams[1].bitrate() );
    ASSERT_EQ( 2, _streams[1].channels() );
    ASSERT_EQ( 0, _streams[1].bits_per_sample() );
    ASSERT_EQ( 44100, _streams[1].samplerate() );

    ASSERT_EQ( 1920, _streams[0].width() );
    ASSERT_EQ( 768, _streams[0].height() );

    ASSERT_EQ( 137, format.playtime() );
}
TEST( MetadataTest, ParseDivxWiegelesHeliSki_DivXPlus_19Mbps) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "WiegelesHeliSki_DivXPlus_19Mbps.mkv" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 2U, _streams.size() );
    ASSERT_EQ( 192000, _streams[1].bitrate() );
    ASSERT_EQ( 2, _streams[1].channels() );
    ASSERT_EQ( 0, _streams[1].bits_per_sample() );
    ASSERT_EQ( 44100, _streams[1].samplerate() );

    ASSERT_EQ( 1920, _streams[0].width() );
    ASSERT_EQ( 1080, _streams[0].height() );

    ASSERT_EQ( 220, format.playtime() );

    av::Metadata _metadata = format.metadata();
    ASSERT_EQ( "Alterna Films 2012", _metadata.get ( av::Metadata::TITLE ) );
    ASSERT_EQ( "Encoded in DivX Plus HD!", _metadata.get ( av::Metadata::COMMENT ) );
}
TEST( MetadataTest, DISABLED_ParseMkvCover ) {
    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "cover_art.mkv" ).message() );

    auto _streams = format.streams();
    ASSERT_EQ( 2U, _streams.size() );
    ASSERT_EQ( 0, _streams.front().bitrate() );
    ASSERT_EQ( 2, _streams.front().channels() );
    ASSERT_EQ( 0, _streams.front().bits_per_sample() );
    ASSERT_EQ( 44100, _streams.front().samplerate() );

    ASSERT_EQ( 1272, _streams[1].width() );
    ASSERT_EQ( 720, _streams[1].height() );

    ASSERT_EQ( 156, format.playtime() );

    av::Metadata _metadata = format.metadata();
    ASSERT_STREQ( "Dexter Season 5 trailer", _metadata.get ( av::Metadata::TITLE ).c_str() );
    ASSERT_STREQ( "", _metadata.get ( av::Metadata::COMMENT ).c_str() );
}
TEST( MetadataTest, open_audio_file_metadata_file_flac ) {

    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "BIS1536-001-flac_16.flac" ).message() );
    EXPECT_EQ ( 205, format.playtime() );

    EXPECT_EQ ( 6U, format.streams().size() );

    av::Stream stream = format.streams().front();
    EXPECT_EQ ( av::CodecType::AUDIO, stream.codec_type() );
    EXPECT_EQ ( av::Codec::FLAC, stream.codec() );
    EXPECT_EQ ( 0, stream.bitrate() );
    EXPECT_EQ ( 44100, stream.samplerate() );
    EXPECT_EQ ( 2, stream.channels() );
    EXPECT_EQ ( 16, stream.bits_per_sample() );
    EXPECT_EQ ( 0, stream.width() );
    EXPECT_EQ ( 0, stream.height() );

    av::Metadata metadata = format.metadata();
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

    av::Format format;
    ASSERT_EQ( std::error_code().message(), format.open( std::string(TESTFILES) + "BIS1536-001-flac_24.flac" ).message() );
    EXPECT_EQ ( 205, format.playtime() );

    EXPECT_EQ ( 5U, format.streams().size() );
    av::Stream _stream = format.streams().front();
    EXPECT_EQ ( CodecType::AUDIO, _stream.codec_type() );
    EXPECT_EQ ( av::Codec::FLAC, _stream.codec() );
    EXPECT_EQ ( 0, _stream.bitrate() );
    EXPECT_EQ ( 44100, _stream.samplerate() );
    EXPECT_EQ ( 2, _stream.channels() );
    EXPECT_EQ ( 24, _stream.bits_per_sample() );
    EXPECT_EQ ( 0, _stream.width() );
    EXPECT_EQ ( 0, _stream.height() );

    av::Metadata metadata = format.metadata();
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
