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

TEST( OptionsTest, add_options ) {

    Options option;
    option[ "option1" ] = Option( "option1", "value1" );
    option[ "option2" ] = Option( "option2", "value2" );
    option[ "option3" ] = Option( "option3", "value3" );

    std::string str1 = option["option1"].c_str();
    EXPECT_EQ( "value1", str1 );

    std::string str2 = option["option2"].c_str();
    EXPECT_EQ( "value2", str2 );

    std::string str3 = option["option3"].c_str();
    EXPECT_EQ( "value3", str3 );
}
TEST( OptionsTest, construct_with_options ) {

    Options option( { { "option1", "value1" }, {"option2", "value2"}, {"option3", "value3" } } );

    std::string str1 = option["option1"].c_str();
    EXPECT_EQ( "value1", str1 );

    std::string str2 = option["option2"].c_str();
    EXPECT_EQ( "value2", str2 );

    std::string str3 = option["option3"].c_str();
    EXPECT_EQ( "value3", str3 );
}


}//namespace av
