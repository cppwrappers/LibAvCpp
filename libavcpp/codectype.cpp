#include "codectype.h"

namespace av {
std::array< std::string, 3 > CodecType::codec_type_names_ {"AUDIO", "VIDEO", "DATA" };
std::string CodecType::name ( Enum codec ) {
    if ( codec == NONE ) { return "none"; }
    else { return codec_type_names_[ static_cast< int > ( codec )]; }
}
}//namespace libav
