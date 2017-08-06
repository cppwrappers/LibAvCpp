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

    case CODEC::MP2:
        return AV_CODEC_ID_MP2;

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

Codec::Codec ( AVCodecContext* codec, Options options ) {

    AVCodec *_input_codec;
    if ( ! ( _input_codec = avcodec_find_decoder ( codec->codec_id ) ) ) {
        errc_ = make_error_code ( AV_DECODER_NOT_FOUND );

    } else {
        int error;
        if ( ( error = avcodec_open2 ( codec, _input_codec, options.av_options() ) ) < 0 ) {
            errc_ = make_error_code ( error );
            return;
        }
    }
    codec_context_ = codec;
}

Codec::Codec ( Format& format, CODEC::Enum codec, Options options ) {

    /* Find the encoder to be used by its name. */
    AVCodec* _output_codec = nullptr;

    if ( ! ( _output_codec = avcodec_find_encoder ( __codec ( codec ) ) ) ) {
        errc_ = make_error_code ( AV_ENCODER_NOT_FOUND );
        return;
    }

    /* Create a new audio stream in the output file container. */
    AVStream *_stream = nullptr;
    if ( ! ( _stream = avformat_new_stream ( format.format_context_, _output_codec ) ) ) {
        errc_ = make_error_code ( ENOMEM );
        return;
    }

    codec_context_ = avcodec_alloc_context3(_output_codec);
    if (!codec_context_) {
        errc_ = make_error_code ( ENOMEM );
        return;
    }

    codec_context_ = _stream->codec;
    codec_context_->sample_fmt = _output_codec->sample_fmts[0];
    codec_context_->channel_layout = av_get_default_channel_layout ( 2 /*TODO*/ );

    /* Allow the use of the experimental AAC encoder */
    codec_context_->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
    /* Set the sample rate for the container. */
    _stream->time_base.den = codec_context_->sample_rate;
    _stream->time_base.num = 1;

    /* Set global headers */
    if ( format.format_context_->oformat->flags & AVFMT_GLOBALHEADER )
    { codec_context_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; }

    /* Open the encoder for the audio stream to use it later. */
    int error;
    if ( ( error = avcodec_open2 ( codec_context_, _output_codec, options.av_options() ) ) < 0 ) {
        errc_ = make_error_code ( error );
        return;
    }
}

Codec::Codec ( Codec&& ) = default;
Codec& Codec::operator= ( Codec&& ) = default;

Codec::~Codec() {
//TODO    if ( codec_context_ )
//    { avcodec_close( codec_context_ ); }
}

CODEC_TYPE::Enum Codec::codec_type() const
{ return __codec ( codec_context_->codec_type ); }
CODEC::Enum Codec::codec() const
{ return __codec ( codec_context_->codec_id ); }
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
{ return static_cast< SampleFormat > ( codec_context_->sample_fmt ); }
bool Codec::sample_fmt( SampleFormat format ) {
    codec_context_->sample_fmt = static_cast< AVSampleFormat >( format );
    const enum AVSampleFormat *p = codec_context_->codec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) {
        if ( *p == static_cast< AVSampleFormat >( format ) )
            return 1;
        p++;
    }
    return 0;
}
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

    else { return codec_type_names_[ static_cast< size_t > ( codec )]; }
}
}//namespace libav
