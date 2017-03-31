#include "formatcontext.h"

#include <iostream>

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

#include "averrc.h"
#include "__avformatcontext.h"

namespace libav {

FormatContext::FormatContext() : av_format_context_( std::make_shared< __AVFormatContext >() ) {}
//FormatContext::FormatContext( auto options ) : options_( std::move( options ) ) {}
FormatContext::~FormatContext() {}

std::error_code FormatContext::input( const std::string& filename ) {
    return ( av_format_context_->make_input_format_context( filename.c_str() ) );
}
unsigned int FormatContext::count () {
    return av_format_context_->input_format_context->nb_streams;
}
stream_ptr FormatContext::stream ( const int& index ) {
    return std::make_shared< AvIoStream >( index, av_format_context_ );
}
int64_t FormatContext::duration () {
    int64_t _secs = 0;
    if ( av_format_context_->input_format_context->duration != AV_NOPTS_VALUE ) {
        int64_t duration = av_format_context_->input_format_context->duration +
                           ( av_format_context_->input_format_context->duration <=
                             INT64_MAX - 5000 ? 5000 : 0 );
        _secs  = duration / AV_TIME_BASE;
     }
    return _secs;
}
Metadata FormatContext::metadata () {
    Metadata _metadata;
    AVDictionaryEntry *tag = NULL;

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "title", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "album", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "artist", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "track", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "composer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "performer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "comment", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "year", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "disc", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "genre", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( av_format_context_->input_format_context->metadata, "publisher", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    return _metadata;
}
}//namespace libav
