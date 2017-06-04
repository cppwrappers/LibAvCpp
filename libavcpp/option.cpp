#include "option.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavcodec/avcodec.h"
#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
# include <stdint.h>
#endif
}

#include <memory>

namespace av {
std::shared_ptr< AVDictionary > Option::make_options ( const std::vector< Option >& options ) {
    AVDictionary *opts = nullptr;

    for ( auto __option : options ) {
        switch ( __option.type() ) {
        case Option::INT:
            av_dict_set_int ( &opts, __option.key(), __option.c_int(), 0 );
            break;

        case Option::STRING:
            av_dict_set ( &opts, __option.key(), __option.c_str(), 0 );
            break;
        }
    }

    return std::shared_ptr< AVDictionary > ( opts, [] ( AVDictionary* ptr ) {
        //TODO if( ptr ) av_dict_free( &ptr );
    } );
}
}//namespace av
