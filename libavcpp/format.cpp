#include "format.h"

#include <sstream>
#include <string>

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

#include "codec.h"
#include "packet.h"
#include "__avformat.h"

namespace av {

///@cond DOC_INTERNAL
static std::once_flag _register_flag;
static void av_init( LOG_LEVEL log = LOG_LEVEL::INFO ) {
    /** @param log the global loglevel. */
    std::call_once ( _register_flag, [&log]() {
        av_register_all ();
        av_log_set_flags( AV_LOG_SKIP_REPEATED );
        av_log_set_level( static_cast< int >( log ) );
    }
    );
}
///@endcond DOC_INTERNAL

Format::Format( const std::string& filename, Mode mode, options_t options ) {
    av_init();
    format_context_ = std::make_shared< __av_format_context >( &errc_, filename.c_str(), (mode==Mode::WRITE), options );
    if( !errc_ ) load_codecs();
}
Format::Format( std::iostream& stream, Mode mode, options_t options ) {
    av_init();
    format_context_ = std::make_shared< __av_format_context >( &errc_, &stream, "", (mode==Mode::WRITE), options );
    if( !errc_ ) load_codecs();
}

Format::~Format() {}

std::vector< std::shared_ptr< Codec > >::iterator Format::begin() {
    return codecs_.begin();
}

std::vector< std::shared_ptr< Codec > >::iterator Format::end() {
    return codecs_.end();
}

//std::vector< std::shared_ptr< __codec_context > >::iterator Format::find( CODEC_TYPE::Enum codec_type, int start_pos ) {
//    for(unsigned short i=0; i<format_context_->format_context_->nb_streams; i++) {
//        if( codec_type == CODEC_TYPE::NONE ||
//            codec_type == format_context_->format_context_->streams[i]->codec->codec_type ) { //TODO CONVERT ONE
//            std::shared_ptr< __codec_context > _codec = std::make_shared< __codec_context >(
//                                    format_context_->format_context_->streams[i]->codec );
//            codecs_.push_back( _codec );
//        }
//    }
//    return codecs_.begin();
//}

//std::vector< Codec >::iterator find( std::vector< Codec >::iterator, int pos = 0 ) {
//    return _codec.find();
//}

//std::vector< Codec > Format::find( CODEC_TYPE::Enum codec_type ) const {

//    std::vector< Codec > _codecs;
//    for(unsigned short i=0; i<format_context_->format_context_->nb_streams; i++) {
//        if( codec_type == CODEC_TYPE::NONE ||
//            codec_type == format_context_->format_context_->streams[i]->codec->codec_type ) { //TODO CONVERT ONE
//            std::shared_ptr< __codec_context > _codec = std::make_shared< __codec_context >(
//                                    format_context_->format_context_->streams[i]->codec );
//            Codec _codec_ptr( _codec );
//            _codecs.push_back( _codec_ptr );
//        }
//    }
////    auto it = std::iterator<std::input_iterator_tag, Codec>( &_codecs );
//    decltype(_codecs.begin()) codec_itr_t = _codecs.begin();
////    codec_itr_t std::iterator<Codec> _it = _codecs.begin();
//    return _codecs;
//}

av::Metadata Format::metadata() const {
    av::Metadata _metadata;
    AVDictionaryEntry *tag = NULL;

    tag = av_dict_get ( format_context_->format_context_->metadata, "title", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "album", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "artist", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "track", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "composer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "performer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "comment", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "date", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( "year", tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "year", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "disc", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "genre", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->format_context_->metadata, "publisher", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    return _metadata;
}

uint64_t Format::playtime() const {
    uint64_t _millis = 0;
    if ( format_context_->format_context_->duration != AV_NOPTS_VALUE ) {
        uint64_t duration = format_context_->format_context_->duration +
            ( format_context_->format_context_->duration <= INT64_MAX - 5000 ? 5000 : 0 );
        _millis = duration / AV_TIME_BASE;
    }
    return _millis;
}

Format& Format::read( Packet& packet ) {
    int err;
    packet.packet_ = std::make_shared< __av_packet >( format_context_->read_packet( &err ) );
    if( err < 0 ) errc_ = make_error_code( err );
    return *this;
}

void Format::load_codecs() {
    codecs_.clear();
    for(unsigned short i=0; i<format_context_->format_context_->nb_streams; i++) {
        std::shared_ptr< __codec_context > _codec = std::make_shared< __codec_context >(
                                format_context_->format_context_->streams[i]->codec );
        auto _codec_ptr = std::make_shared< Codec >( i, _codec );
        codecs_.push_back( _codec_ptr );
    }
}

bool Format::operator!() const
{ return !errc_ ; }
bool Format::good()
{ return errc_.value() == 0; }
bool Format::eof()
{ return errc_.value() == AV_EOF; }
bool Format::fail()
{ return !errc_ && errc_.value() != AV_EOF; }
std::error_code Format::errc ()
{ return errc_; }

}//namespace av
