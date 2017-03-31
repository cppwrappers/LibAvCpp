#include "codec.h"

namespace libav {
std::array< std::string, 4 > Codec::codec_names_ {"mp3", "flac", "aac", "vorbis" };
std::string Codec::name ( Enum codec ) {
    if ( codec == NONE ) { return "none"; }

    else { return codec_names_[ static_cast< int > ( codec )]; }
}
}//namespace libav
