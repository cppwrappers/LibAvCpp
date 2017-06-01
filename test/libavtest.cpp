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

//#include "../src/libav.h"
//#include "../src/averrc.h"

//#include <gtest/gtest.h>

//namespace libav {

//TEST_F( LibavTest, transcode_file_stream ) {

//    { //Transcode
//        std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16-nocover.flac";
//        std::fstream _fs ( filename );

//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( &_fs ) );
//        ASSERT_FALSE( _audio.output( "/tmp/file_istream_out.mp3" ) );
//        MediaInfo _media_info = _audio.media_info( 0 );
//        _media_info.samplerate = 44100;
//        _media_info.bitrate = 192000;
//        _media_info.codec = Codec::MP3;
//        ASSERT_FALSE( _audio.transcode( 0, _media_info ) );
//    }

//    { //open and test transcoded file
//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( "/tmp/file_istream_out.mp3" ) );

//        EXPECT_EQ ( 1, _audio.nb_streams() );

//        MediaInfo _type = _audio.media_info( 0 );
//        EXPECT_EQ ( 1, _audio.nb_streams() );
//        EXPECT_EQ ( MediaType::AUDIO, _type.type );
//        EXPECT_EQ ( Codec::MP3, _type.codec );
//        EXPECT_EQ ( 192000, _type.bitrate );
//        EXPECT_EQ ( 44100, _type.samplerate );
//        EXPECT_EQ ( 2, _type.channels );
//        EXPECT_EQ ( 205, _type.playtime );
//        EXPECT_EQ ( 0, _type.bits_per_sample );
//        EXPECT_EQ ( 0, _type.width );
//        EXPECT_EQ ( 0, _type.height );

//        EXPECT_EQ ( 11U, _type.metadata.tag_names().size() );
//        EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _type.metadata.get( Metadata::ALBUM ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::ARTIST ) );
//        EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _type.metadata.get( Metadata::COMMENT ) );
//        EXPECT_EQ ( "Purcell, Henry", _type.metadata.get( Metadata::COMPOSER ) );
//        EXPECT_EQ ( "1/1", _type.metadata.get( Metadata::DISC) );
//        EXPECT_EQ ( "Classical", _type.metadata.get( Metadata::GENRE ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::PERFORMER ) );
//        EXPECT_EQ ( "BIS;eClassical", _type.metadata.get( Metadata::PUBLISHER ) );
//        EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _type.metadata.get( Metadata::TITLE ) );
//        EXPECT_EQ ( "1/19", _type.metadata.get( Metadata::TRACK ) );
//        EXPECT_EQ ( "2007-08-30", _type.metadata.get( Metadata::YEAR ) );
//    }
//}
//TEST_F( LibavTest, DISABLED_transcode_io_stream ) {

//    { //Transcode
//        std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16-nocover.flac";
//        std::fstream _fs ( filename );

//        std::string out_filename = "/tmp/out-iostream.mp3";
//        std::fstream _os (out_filename, std::ios_base::out );

//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( &_fs ) );

//        MediaInfo _media_info = _audio.media_info( 0 );
//        _media_info.bitrate = 320000;
//        _media_info.samplerate = 44100;
//        _media_info.codec = Codec::MP3;
//        ASSERT_FALSE( _audio.output( _media_info) );

//        ASSERT_EQ( 1, _audio.nb_streams() );
//        ASSERT_EQ( MediaType::AUDIO, _audio.media_info( 0 ).type );


//        ASSERT_FALSE( _audio.transcode( 0, _media_info ) );

////TODO        std::array< unsigned char, 1024 > _buffer;
////        int _r_size = 0;
////        while( ( _r_size = _audio.readsome( _buffer.data(), 1024 ) ) >= 0 ) {
////            //write to file
////            _os.write ( reinterpret_cast< char* >(_buffer.data() ), _r_size );
////        }
//        _os.write( _audio.get_output().c_str(), _audio.get_output().size() );
//    }

//    { //open and test transcoded file
//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( "/tmp/out-iostream.mp3" ) );

//        EXPECT_EQ ( 1, _audio.nb_streams() );

//        MediaInfo _type = _audio.media_info( 0 );
//        EXPECT_EQ ( 1, _audio.nb_streams() );
//        EXPECT_EQ ( MediaType::AUDIO, _type.type );
//        EXPECT_EQ ( Codec::MP3, _type.codec );
//        EXPECT_EQ ( 320000, _type.bitrate );
//        EXPECT_EQ ( 44100, _type.samplerate );
//        EXPECT_EQ ( 2, _type.channels );
//        EXPECT_EQ ( 205, _type.playtime );
//        EXPECT_EQ ( 0, _type.bits_per_sample );
//        EXPECT_EQ ( 0, _type.width );
//        EXPECT_EQ ( 0, _type.height );

//        EXPECT_EQ ( 11U, _type.metadata.tag_names().size() );
//        EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _type.metadata.get( Metadata::ALBUM ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::ARTIST ) );
//        EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _type.metadata.get( Metadata::COMMENT ) );
//        EXPECT_EQ ( "Purcell, Henry", _type.metadata.get( Metadata::COMPOSER ) );
//        EXPECT_EQ ( "1/1", _type.metadata.get( Metadata::DISC) );
//        EXPECT_EQ ( "Classical", _type.metadata.get( Metadata::GENRE ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::PERFORMER ) );
//        EXPECT_EQ ( "BIS;eClassical", _type.metadata.get( Metadata::PUBLISHER ) );
//        EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _type.metadata.get( Metadata::TITLE ) );
//        EXPECT_EQ ( "1/19", _type.metadata.get( Metadata::TRACK ) );
//        EXPECT_EQ ( "2007-08-30", _type.metadata.get( Metadata::YEAR ) );
//    }
//}
//TEST_F( LibavTest, DISABLED_transcode_io_stream_hd_images) {

//    { //Transcode
//        std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_24.flac";
//        std::fstream _fs ( filename );

//        std::string out_filename = "/tmp/out-iostream-transcode.mp3";
//        std::fstream _os (out_filename, std::ios_base::out );

//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( &_fs ) );

//        MediaInfo _media_info = _audio.media_info( 0 );
//        ASSERT_EQ( MediaType::AUDIO, _media_info.type );
//        _media_info.bitrate = 320000;
//        _media_info.samplerate = 44100;
//        _media_info.codec = Codec::MP3;

//        ASSERT_FALSE( _audio.output( _media_info ) );
//        ASSERT_EQ( 5, _audio.nb_streams() );

//        ASSERT_EQ( std::error_code ().message(), _audio.transcode( 0, _media_info ).message() );

////TODO        std::array< unsigned char, 1024 > _buffer;
////        int _r_size = 0;
////        while( ( _r_size = _audio.readsome( _buffer.data(), 1024 ) ) >= 0 ) {
////            //write to file
////            _os.write ( reinterpret_cast< char* >(_buffer.data() ), _r_size );
////        }
//        _os.write( _audio.get_output().c_str(), _audio.get_output().size() );
//    }

//    { //open and test transcoded file
//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( "/tmp/out-iostream.mp3" ) );

//        EXPECT_EQ ( 1, _audio.nb_streams() );

//        MediaInfo _type = _audio.media_info( 0 );
//        EXPECT_EQ ( 1, _audio.nb_streams() );
//        EXPECT_EQ ( MediaType::AUDIO, _type.type );
//        EXPECT_EQ ( Codec::MP3, _type.codec );
//        EXPECT_EQ ( 320000, _type.bitrate );
//        EXPECT_EQ ( 44100, _type.samplerate );
//        EXPECT_EQ ( 2, _type.channels );
//        EXPECT_EQ ( 205, _type.playtime );
//        EXPECT_EQ ( 0, _type.bits_per_sample );
//        EXPECT_EQ ( 0, _type.width );
//        EXPECT_EQ ( 0, _type.height );

//        EXPECT_EQ ( 11U, _type.metadata.tag_names().size() );
//        EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _type.metadata.get( Metadata::ALBUM ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::ARTIST ) );
//        EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _type.metadata.get( Metadata::COMMENT ) );
//        EXPECT_EQ ( "Purcell, Henry", _type.metadata.get( Metadata::COMPOSER ) );
//        EXPECT_EQ ( "1/1", _type.metadata.get( Metadata::DISC) );
//        EXPECT_EQ ( "Classical", _type.metadata.get( Metadata::GENRE ) );
//        EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::PERFORMER ) );
//        EXPECT_EQ ( "BIS;eClassical", _type.metadata.get( Metadata::PUBLISHER ) );
//        EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _type.metadata.get( Metadata::TITLE ) );
//        EXPECT_EQ ( "1/19", _type.metadata.get( Metadata::TRACK ) );
//        EXPECT_EQ ( "2007-08-30", _type.metadata.get( Metadata::YEAR ) );
//    }
//}
//}//namespace libaav
