#include <string>
#include <fstream>
#include <map>

#include "../libavcpp/averrc.h"
#include "../libavcpp/format.h"

#include <gtest/gtest.h>

#include "_testconstants.h"

namespace av {

TEST( LiabavErrcTest, err_condition ) {
    std::error_code errc = std::error_code ( abs ( -2 ), std::generic_category() );
    EXPECT_EQ( ENOENT, errc.value() );
    EXPECT_EQ( "No such file or directory", errc.message() );
}

TEST( LiabavErrcTest, errc ) {
    Format _format_simple_mp3( FILE_SAMPLE_MP3 );
    EXPECT_FALSE( _format_simple_mp3 );
    EXPECT_EQ( "Success", _format_simple_mp3.errc().message() );

    Format _format_false( "dum_trala.mp3" );
    EXPECT_FALSE( !_format_false );
    EXPECT_EQ( "No such file or directory", _format_false.errc().message() );
}
}//namespace av
