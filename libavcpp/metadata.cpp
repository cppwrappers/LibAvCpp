#include "metadata.h"

#include <cstring>
#include <algorithm>

#include <boost/algorithm/string/case_conv.hpp>

namespace libav {

static std::array< std::string, 11 > _tag_names {
    { "title", "album", "artist", "composer", "performer", "comment", "year", "track", "disc", "genre", "publisher"  }
};
void Metadata::set ( const char* name, const char* value ) {
    std::string _upper_name = name;
    boost::to_upper ( _upper_name );

    if ( strcmp ( "TITLE", _upper_name.c_str() ) == 0 )
    { tags[ TITLE ] = value; }

    else if ( strcmp ( "ALBUM", _upper_name.c_str() ) == 0 )
    { tags[ ALBUM ] = value; }

    else if ( strcmp ( "ARTIST", _upper_name.c_str() ) == 0 )
    { tags[ ARTIST ] = value; }

    else if ( strcmp ( "COMPOSER", _upper_name.c_str() ) == 0 )
    { tags[ COMPOSER ] = value; }

    else if ( strcmp ( "PERFORMER", _upper_name.c_str() ) == 0 )
    { tags[ PERFORMER ] = value; }

    else if ( strcmp ( "COMMENT", _upper_name.c_str() ) == 0 )
    { tags[ COMMENT ] = value; }

    else if ( strcmp ( "DATE", _upper_name.c_str() ) == 0 )
    { tags[ YEAR ] = value; }

    else if ( strcmp ( "TRACK", _upper_name.c_str() ) == 0 )
    { tags[ TRACK ] = value; }

    else if ( strcmp ( "DISC", _upper_name.c_str() ) == 0 )
    { tags[ DISC ] = value; }

    else if ( strcmp ( "GENRE", _upper_name.c_str() ) == 0 )
    { tags[ GENRE ] = value; }

    else if ( strcmp ( "PUBLISHER", _upper_name.c_str() ) == 0 )
    { tags[ PUBLISHER ] = value; }

    //extra values are skipped
}
std::string& Metadata::get ( const Metadata::Enum& key ) {
    return tags[ key ];
}
std::vector< Metadata::Enum > Metadata::tag_names () {
    std::vector< Enum > _keys;

    for ( auto const& k: tags )
    { _keys.push_back ( k.first ); }

    return _keys;
}
std::string Metadata::name ( Enum tag ) {
    return _tag_names[ static_cast< int > ( tag )];
}
}//namespace libav
