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

const std::vector< Codec >::iterator Format::begin() {
    return codecs_.begin();
}

const std::vector< Codec >::iterator Format::end() {
    return codecs_.end();
}
const std::vector< Codec >::iterator Format::find_codec( CODEC_TYPE::Enum type ) {
    for (auto _itr_codec = codecs_.begin(); _itr_codec != codecs_.end(); ++_itr_codec ) {
            if ( _itr_codec->codec_type() == type ) {
                return _itr_codec;
            }
        }
        return codecs_.end();
}

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
    int _err = 0;
    if ( ( _err  = av_read_frame( format_context_->format_context_, packet.packet_ ) ) < 0 ) {
        errc_ = make_error_code( _err );
    }
    return *this;
}
Format& Format::read( std::vector< Codec >::iterator itr, Packet& packet ) {
    int _err = 0;
    do {
        if ( ( _err  = av_read_frame( format_context_->format_context_, packet.packet_ ) ) < 0 ) {
            errc_ = make_error_code( _err );
        }
    } while( !_err && ( itr - codecs_.begin() ) != packet.stream_index() );
    return *this;
}

Format& Format::write( Packet& packet ) {
    int error;
    if ( ( error = av_write_frame(format_context_->format_context_, packet.packet_ ) ) < 0)
    { errc_ = std::error_code( error, av_category ); }
    return *this;
}

std::error_code Format::decode( Packet& packet, Frame& frame) {

    int data_present = 0, error = 0;
    error = avcodec_decode_audio4(
                codecs_.at( packet.stream_index() ).codec_context_,
                frame.frame_,
                &data_present,
                packet.packet_ );

    if( error < 0 )
    { return make_error_code( error ); }
    if( !data_present )
    { return make_error_code( AV_EOF ); }
    return std::error_code();
}

std::error_code Format::encode( Codec& codec, Frame& frame, std::function< void( Packet& packet ) > fnc ) {

    Packet packet_;
    int error, data_present = 0;

    /** Set a timestamp based on the sample rate for the container. */
    frame.pts( pts );
    pts += frame.nb_samples();

    /**
     * Encode the audio frame and store it in the temporary packet.
     * The output audio stream encoder is used to do this.
     */
    if ((error = avcodec_encode_audio2( codec.codec_context_, packet_.packet_, frame.frame_, &data_present)) < 0)
    { return std::error_code( error, av_category ); }

    fnc( packet_ );
    return std::error_code();
}

void Format::load_codecs() {
    codecs_.clear();
    for(unsigned short i=0; i<format_context_->format_context_->nb_streams; i++) {
        codecs_.push_back( Codec( format_context_->format_context_->streams[i]->codec, options_t() /*TODO */ ) );
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
