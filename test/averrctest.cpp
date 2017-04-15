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
    Format _format = Format( FILE_SAMPLE_MP3 );
    EXPECT_FALSE( _format );
    EXPECT_EQ( "Success", _format.errc().message() );

    _format = Format ( "dum_trala.mp3" );
    EXPECT_FALSE( !_format );
    EXPECT_EQ( "No such file or directory", _format.errc().message() );
}
}//namespace av
