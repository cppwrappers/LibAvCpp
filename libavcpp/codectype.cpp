#include "codectype.h"

namespace av {
std::array< std::string, 6 > CodecType::codec_type_names_ {"VIDEO", "AUDIO", "DATA", "SUBTITLE", "ATTACHEMENT", "NB"};
std::string CodecType::name ( Enum codec ) {
    if ( codec == NONE ) { return "none"; }
    else { return codec_type_names_[ static_cast< int > ( codec )]; }
}
}//namespace libav
