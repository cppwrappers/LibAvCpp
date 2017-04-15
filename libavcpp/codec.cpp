#include "codec.h"

#include "__avformat.h"

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
Codec::Codec( int index, Codec& codec, options_t options ) : index_(index) {
    codec_context_ = std::make_shared< __codec_context >(
                &errc_,
                codec.codec_context_->codec_context_,
                options );
}
Codec::Codec( int index, Format& format_context, CODEC::Enum codec, options_t options ) : index_(index) {
    codec_context_ = std::make_shared< __codec_context >( &errc_, format_context.format_context_->format_context_, __codec( codec ), options );
}
CODEC_TYPE::Enum Codec::codec_type() const
{ return __codec( codec_context_->codec_context_->codec_type ); }
CODEC::Enum Codec::codec() const
{ return __codec( codec_context_->codec_context_->codec_id ); }
int Codec::bitrate() const
{ return codec_context_->codec_context_->bit_rate; }
int Codec::samplerate() const
{ return codec_context_->codec_context_->sample_rate; }
int Codec::channels() const
{ return codec_context_->codec_context_->channels; }
int Codec::bits_per_sample() const
{ return codec_context_->codec_context_->bits_per_raw_sample; }
int Codec::width() const
{ return codec_context_->codec_context_->width; }
int Codec::height() const
{ return codec_context_->codec_context_->height; }
int Codec::pixel_format() const
{ return codec_context_->codec_context_->pix_fmt; }
SampleFormat Codec::sample_format() const
{ return static_cast< SampleFormat >( codec_context_->codec_context_->sample_fmt ); }
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

Codec::operator bool() const
{ if( errc_ ) return true; return false; }
bool Codec::operator!() const
{ if( errc_ ) return true; return false; }
bool Codec::good()
{ return errc_.value() == 0; }
bool Codec::fail()
{ return !errc_; }
std::error_code Codec::errc ()
{ return errc_; }
}//namespace libav
