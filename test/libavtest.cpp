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

//class LibavTest : public ::testing::Test {
// protected:
//  // Per-test-case set-up.
//  // Called before the first test in this test case.
//  // Can be omitted if not needed.
//  static void SetUpTestCase() {
//    delete_files();
//  }

//  // Per-test-case tear-down.
//  // Called after the last test in this test case.
//  // Can be omitted if not needed.
//  static void TearDownTestCase() {
//      std::cout << "Teardown Testcase:" << std::endl;
//  }

//  static void delete_files () {
//      if( boost::filesystem::exists( "/tmp/out-iostream.mp3" ) )
//          boost::filesystem::remove( "/tmp/out-iostream.mp3" );

//      if( boost::filesystem::exists( "/tmp/file_out.mp3" ) )
//          boost::filesystem::remove( "/tmp/file_out.mp3" );

//      if( boost::filesystem::exists( "/tmp/file_istream_out.mp3" ) )
//          boost::filesystem::remove( "/tmp/file_istream_out.mp3" );
//}

////  // You can define per-test set-up and tear-down logic as usual.
////  virtual void SetUp() { ... }
////  virtual void TearDown() { ... }

////  // Some expensive resource shared by all tests.
////  static T* shared_resource_;
//};

//TEST_F( LibavTest, err_condition ) {
//    std::error_code errc = make_error_code( -2 );
//    EXPECT_EQ( ENOENT, errc.value() );
//    EXPECT_EQ( "No such file or directory", errc.message() );
//}

//TEST_F( LibavTest, errc ) {
//    std::string filename = "/home/e3a/testfiles/sample.flac";

//    LibAv _audio;
//    std::error_code errc = _audio.input( filename.c_str() );
//    ASSERT_FALSE( errc );
//    ASSERT_EQ( "Success", errc.message() );

//    errc = _audio.input( "dum_trala.mp3" );
//    ASSERT_EQ( ENOENT, errc.value() );
//    ASSERT_EQ( "No such file or directory", errc.message() );
//}

//TEST_F( LibavTest, open_audio_file_metadata_file_mp3 ) {
//    std::string filename = std::string( TESTFILES ) + "sample.mp3";

//    LibAv _audio;
//    ASSERT_FALSE( _audio.input( filename.c_str() ) );

//    EXPECT_EQ ( 1, _audio.nb_streams() );

//    MediaInfo _type = _audio.media_info( 0 );
//    EXPECT_EQ ( MediaType::AUDIO, _type.type );
//    EXPECT_EQ ( Codec::MP3, _type.codec );
//    EXPECT_EQ ( 64075, _type.bitrate );
//    EXPECT_EQ ( 44100, _type.samplerate );
//    EXPECT_EQ ( 2, _type.channels );
//    EXPECT_EQ ( 4, _type.playtime );
//    EXPECT_EQ ( 0, _type.bits_per_sample );
//    EXPECT_EQ ( 0, _type.width );
//    EXPECT_EQ ( 0, _type.height );
//}

//TEST_F( LibavTest, open_audio_file_metadata_file_flac ) {
//    std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16.flac";

//    LibAv _audio;
//    ASSERT_FALSE( _audio.input( filename.c_str() ) );

//    EXPECT_EQ ( 6, _audio.nb_streams() );

//    MediaInfo _type = _audio.media_info( 0 );
//    EXPECT_EQ ( MediaType::AUDIO, _type.type );
//    EXPECT_EQ ( Codec::FLAC, _type.codec );
//    EXPECT_EQ ( 0, _type.bitrate );
//    EXPECT_EQ ( 44100, _type.samplerate );
//    EXPECT_EQ ( 2, _type.channels );
//    EXPECT_EQ ( 205, _type.playtime );
//    EXPECT_EQ ( 16, _type.bits_per_sample );
//    EXPECT_EQ ( 0, _type.width );
//    EXPECT_EQ ( 0, _type.height );

//    EXPECT_EQ ( 11U, _type.metadata.tag_names().size() );
//    EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _type.metadata.get( Metadata::ALBUM ) );
//    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::ARTIST ) );
//    EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _type.metadata.get( Metadata::COMMENT ) );
//    EXPECT_EQ ( "Purcell, Henry", _type.metadata.get( Metadata::COMPOSER ) );
//    EXPECT_EQ ( "1/1", _type.metadata.get( Metadata::DISC) );
//    EXPECT_EQ ( "(32)Vocal/Choral;(32)Baroque", _type.metadata.get( Metadata::GENRE ) );
//    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::PERFORMER ) );
//    EXPECT_EQ ( "BIS;eClassical", _type.metadata.get( Metadata::PUBLISHER ) );
//    EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _type.metadata.get( Metadata::TITLE ) );
//    EXPECT_EQ ( "1/19", _type.metadata.get( Metadata::TRACK ) );
//    EXPECT_EQ ( "2007-08-30", _type.metadata.get( Metadata::YEAR ) );
//}

//TEST_F( LibavTest, open_audio_file_metadata_file_flac_hd ) {
//    std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_24.flac";

//    LibAv _audio;
//    ASSERT_FALSE( _audio.input( filename.c_str() ) );

//    EXPECT_EQ ( 5, _audio.nb_streams() );

//    MediaInfo _type = _audio.media_info( 0 );
//    EXPECT_EQ ( MediaType::AUDIO, _type.type );
//    EXPECT_EQ ( Codec::FLAC, _type.codec );
//    EXPECT_EQ ( 0, _type.bitrate );
//    EXPECT_EQ ( 44100, _type.samplerate );
//    EXPECT_EQ ( 2, _type.channels );
//    EXPECT_EQ ( 205, _type.playtime );
//    EXPECT_EQ ( 24, _type.bits_per_sample );
//    EXPECT_EQ ( 0, _type.width );
//    EXPECT_EQ ( 0, _type.height );

//    EXPECT_EQ ( 11U, _type.metadata.tag_names().size() );
//    EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _type.metadata.get( Metadata::ALBUM ) );
//    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::ARTIST ) );
//    EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _type.metadata.get( Metadata::COMMENT ) );
//    EXPECT_EQ ( "Purcell, Henry", _type.metadata.get( Metadata::COMPOSER ) );
//    EXPECT_EQ ( "1/1", _type.metadata.get( Metadata::DISC) );
//    EXPECT_EQ ( "(32)Vocal/Choral;(32)Baroque", _type.metadata.get( Metadata::GENRE ) );
//    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::PERFORMER ) );
//    EXPECT_EQ ( "BIS;eClassical", _type.metadata.get( Metadata::PUBLISHER ) );
//    EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _type.metadata.get( Metadata::TITLE ) );
//    EXPECT_EQ ( "1/19", _type.metadata.get( Metadata::TRACK ) );
//    EXPECT_EQ ( "2007-08-30", _type.metadata.get( Metadata::YEAR ) );
//}

//TEST_F( LibavTest, transcode_audio_file ) {
//    std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16-nocover.flac";
//    std::string dest_file  = "/tmp/file_out.mp3";

//    { //Transcode
//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( filename.c_str() ) );
//        ASSERT_FALSE( _audio.output( dest_file ) );
//        MediaInfo _media_info = _audio.media_info( 0 );
//        _media_info.samplerate = 44100;
//        _media_info.bitrate = 192000;
//        _media_info.codec = Codec::MP3;
//        ASSERT_EQ( std::error_code().message(), _audio.transcode( 0, _media_info ).message() );
//    }

//    { //open and test transcoded file
//        LibAv _audio;
//        ASSERT_FALSE( _audio.input( dest_file ) );

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

//TEST_F( LibavTest, open_audio_file_metadata_stream ) {
//    std::string filename = std::string( TESTFILES ) + "BIS1536-001-flac_16-nocover.flac";

//    LibAv _audio;
//    std::fstream _fs ( filename );
//    ASSERT_FALSE( _audio.input( &_fs ) );

//    EXPECT_EQ ( 1, _audio.nb_streams() );
//    MediaInfo _type = _audio.media_info( 0 );
//    EXPECT_EQ ( 1, _audio.nb_streams() );
//    EXPECT_EQ ( MediaType::AUDIO, _type.type );
//    EXPECT_EQ ( Codec::FLAC, _type.codec );
//    EXPECT_EQ ( 0, _type.bitrate );
//    EXPECT_EQ ( 44100, _type.samplerate );
//    EXPECT_EQ ( 2, _type.channels );
//    EXPECT_EQ ( 205, _type.playtime );
//    EXPECT_EQ ( 16, _type.bits_per_sample );
//    EXPECT_EQ ( 0, _type.width );
//    EXPECT_EQ ( 0, _type.height );

//    EXPECT_EQ ( 11U, _type.metadata.tag_names().size() );
//    EXPECT_EQ ( "Victorious Love - Carolyn Sampson sings Purcell", _type.metadata.get( Metadata::ALBUM ) );
//    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::ARTIST ) );
//    EXPECT_EQ ( "Downloaded from eClassical.com. From album BIS-SACD-1536", _type.metadata.get( Metadata::COMMENT ) );
//    EXPECT_EQ ( "Purcell, Henry", _type.metadata.get( Metadata::COMPOSER ) );
//    EXPECT_EQ ( "1/1", _type.metadata.get( Metadata::DISC) );
//    EXPECT_EQ ( "(32)Vocal/Choral;(32)Baroque", _type.metadata.get( Metadata::GENRE ) );
//    EXPECT_EQ ( "Sampson, Carolyn;Cummings, Laurence;Kenny, Elizabeth;Lasla, Anne-Marie", _type.metadata.get( Metadata::PERFORMER ) );
//    EXPECT_EQ ( "BIS;eClassical", _type.metadata.get( Metadata::PUBLISHER ) );
//    EXPECT_EQ ( "(Sweeter than roses, Z.585 No.1) - Sweeter than roses, Z.585 No.1", _type.metadata.get( Metadata::TITLE ) );
//    EXPECT_EQ ( "1/19", _type.metadata.get( Metadata::TRACK ) );
//    EXPECT_EQ ( "2007-08-30", _type.metadata.get( Metadata::YEAR ) );
//}

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
