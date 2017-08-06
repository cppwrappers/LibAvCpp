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
AVDictionary* make_options ( const std::vector< Option >& options ) {
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

    //return std::shared_ptr< AVDictionary > ( opts, [] ( AVDictionary* ptr ) {
        //TODO if( ptr ) av_dict_free( &ptr );
    return nullptr; //TODO
    }

Options::Options( std::vector< std::pair< std::string, std::string > > values ) {
    for( auto& o : values ) {
        options_map_[o.first] = Option( o.first, o.second );
    }
}

Option& Options::operator[] ( std::string x ) {
    return options_map_[x];
}
AVDictionary** Options::av_options() {
    AVDictionary *opts = nullptr;
    for ( auto __option : options_map_ ) {
        switch ( __option.second.type() ) {
        case Option::INT:
            av_dict_set_int ( &opts, __option.second.key(), __option.second.c_int(), 0 );
            break;

        case Option::STRING:
            av_dict_set ( &opts, __option.second.key(), __option.second.c_str(), 0 );
            break;
        }
    }

    //return std::shared_ptr< AVDictionary > ( opts, [] ( AVDictionary* ptr ) {
        //TODO if( ptr ) av_dict_free( &ptr );
    return &opts;
}
}//namespace av
