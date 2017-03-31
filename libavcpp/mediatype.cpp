#include "mediatype.h"

namespace libav {
std::array< std::string, 6 > MediaType::type_names_ {"VIDEO", "AUDIO", "DATA", "SUBTITLE", "ATTACHEMENT", "NB" };
std::string MediaType::name ( Enum type ) {
    if ( type == UNKNOWN ) { return "unknown"; }

    else { return type_names_[ static_cast< int > ( type )]; }
}
}//namespace libav
