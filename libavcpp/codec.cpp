#include "codec.h"

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

namespace av {
inline CODEC::Enum __codec ( AVCodecID codec_id ) {
    switch ( codec_id ) {
    case AV_CODEC_ID_MP3:
        return CODEC::MP3;

    case AV_CODEC_ID_FLAC:
        return CODEC::FLAC;

    case AV_CODEC_ID_AAC:
        return CODEC::AAC;

    case AV_CODEC_ID_VORBIS:
        return CODEC::VORBIS;

    default:
        return CODEC::NONE;
    }
}
inline AVCodecID __codec ( CODEC::Enum codec ) {
    switch ( codec ) {
    case CODEC::AAC:
        return AV_CODEC_ID_AAC;

    case CODEC::FLAC:
        return AV_CODEC_ID_FLAC;

    case CODEC::MP3:
        return AV_CODEC_ID_MP3;

    case CODEC::VORBIS:
        return AV_CODEC_ID_VORBIS;

    case CODEC::NONE:
        break;
    }
    return AV_CODEC_ID_NONE;
}
inline CODEC_TYPE::Enum __codec ( AVMediaType codec_type ) {
    switch ( codec_type ) {
    case AVMEDIA_TYPE_VIDEO:
        return CODEC_TYPE::VIDEO;

    case AVMEDIA_TYPE_AUDIO:
        return CODEC_TYPE::AUDIO;

    case AVMEDIA_TYPE_DATA:
        return CODEC_TYPE::DATA;

    default:
        return CODEC_TYPE::NONE;
    }
}

Codec::Codec( AVCodecContext* codec, options_t options ) {
    AVCodec *_input_codec;
    if ( ! ( _input_codec = avcodec_find_decoder ( codec->codec_id ) ) ) {
        errc_ = make_error_code ( AV_DECODER_NOT_FOUND );
    } else {
        int error;
        auto _options = Option::make_options( options ).get(); //TODO
        if ( ( error = avcodec_open2 ( codec, _input_codec, &_options ) ) < 0 ) {
            errc_ = make_error_code ( error );
            return;
        }
    }
    codec_context_ = codec;
}

Codec::Codec(Codec&&) = default;
Codec& Codec::operator=(Codec&&) = default;

//Codec::Codec( std::unique_ptr< __codec_context > context ) : codec_context_( std::move( context ) ) {}
//Codec::Codec( Format& format_context, CODEC::Enum codec, options_t options ) {




//}
Codec::~Codec() {}

CODEC_TYPE::Enum Codec::codec_type() const
{ return __codec( codec_context_->codec_type ); }
CODEC::Enum Codec::codec() const
{ return __codec( codec_context_->codec_id ); }
int Codec::bitrate() const
{ return codec_context_->bit_rate; }
int Codec::sample_rate() const
{ return codec_context_->sample_rate; }
int Codec::channels() const
{ return codec_context_->channels; }
int Codec::bits_per_sample() const
{ return codec_context_->bits_per_raw_sample; }
int Codec::width() const
{ return codec_context_->width; }
int Codec::height() const
{ return codec_context_->height; }
int Codec::pixel_format() const
{ return codec_context_->pix_fmt; }
SampleFormat Codec::sample_fmt() const
{ return static_cast< SampleFormat >( codec_context_->sample_fmt ); }
int Codec::frame_size()
{ return codec_context_->frame_size; }
uint64_t Codec::channel_layout()
{ return codec_context_->channel_layout; }


std::array< std::string, 4 > Codec::codec_names_ {{"mp3", "flac", "aac", "vorbis"} };
std::string Codec::name ( CODEC::Enum codec ) {
    if ( codec == CODEC::NONE ) { return "none"; }
    else { return codec_names_[ static_cast< size_t > ( codec )]; }
}
std::array< std::string, 6 > Codec::codec_type_names_ {{"VIDEO", "AUDIO", "DATA", "SUBTITLE", "ATTACHEMENT", "NB"}};
std::string Codec::name ( CODEC_TYPE::Enum codec ) {
    if ( codec == CODEC_TYPE::NONE ) { return "none"; }
    else { return codec_type_names_[ static_cast< size_t >( codec )]; }
}
}//namespace libav
