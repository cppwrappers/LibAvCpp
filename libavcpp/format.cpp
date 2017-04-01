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

namespace av {

inline libav::Codec::Enum __codec ( AVCodecID codec_id ) {
    switch ( codec_id ) {
    case AV_CODEC_ID_MP3:
        return libav::Codec::MP3;

    case AV_CODEC_ID_FLAC:
        return libav::Codec::FLAC;

    case AV_CODEC_ID_AAC:
        return libav::Codec::AAC;

    case AV_CODEC_ID_VORBIS:
        return libav::Codec::VORBIS;

    default:
        return libav::Codec::NONE;
    }
}
inline av::CodecType::Enum __codec_type ( AVMediaType codec_type ) {
    switch ( codec_type ) {
    case AVMEDIA_TYPE_VIDEO:
        return CodecType::VIDEO;

    case AVMEDIA_TYPE_AUDIO:
        return CodecType::AUDIO;

    case AVMEDIA_TYPE_DATA:
        return CodecType::DATA;

    default:
        return CodecType::NONE;
    }
}
struct __Format {
    std::shared_ptr < AVFormatContext > input_format_context;
};

Format::Format() : format_( std::make_shared< __Format >() ) {
    std::call_once ( _register_flag, []() { av_register_all(); } );

}
Format::~Format() {}
std::error_code Format::open( const std::string& filename ) {
    int _error;
    /** Open the input file to read from it. */
    AVFormatContext* _input_format_context = nullptr;

    if ( ( _error = avformat_open_input ( &_input_format_context, filename.c_str(), NULL, NULL ) ) != 0 )
    { return libav::make_error_code ( _error ); }

    format_->input_format_context = std::shared_ptr< AVFormatContext > ( _input_format_context,
    [] ( AVFormatContext* p ) { if ( p ) { avformat_close_input ( &p ); } } );

    /** Get information on the input file (number of streams etc.). */
    if ( ( _error = avformat_find_stream_info ( format_->input_format_context.get(), NULL ) ) < 0 )
    { return libav::make_error_code ( _error ); };

//TODO only in debug mode
    av_dump_format(format_->input_format_context.get(), 0, filename.c_str(), 0);
//TODO only in debug mode

    return std::error_code ();
}

std::vector< Stream > Format::streams() const {
    std::vector< Stream > _streams;
    for(unsigned short i=0; i<format_->input_format_context->nb_streams; i++)
        _streams.push_back(
            Stream {i,
                    __codec_type( format_->input_format_context->streams[i]->codec->codec_type ),
                    __codec( format_->input_format_context->streams[i]->codec->codec_id ),
                    format_->input_format_context->streams[i]->codec->bit_rate,
                    format_->input_format_context->streams[i]->codec->sample_rate,
                    format_->input_format_context->streams[i]->codec->channels,
                    format_->input_format_context->streams[i]->codec->bits_per_raw_sample,
                    format_->input_format_context->streams[i]->codec->width,
                    format_->input_format_context->streams[i]->codec->height,
                    format_->input_format_context->streams[i]->codec->pix_fmt } );
    return _streams;
}

libav::Metadata Format::metadata() const {
    libav::Metadata _metadata;
    AVDictionaryEntry *tag = NULL;

    tag = av_dict_get ( format_->input_format_context->metadata, "title", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "album", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "artist", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "track", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "composer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "performer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "comment", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "year", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "disc", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "genre", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_->input_format_context->metadata, "publisher", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    return _metadata;
}
av::Frame get( const Stream& stream ) {
//    // Get a pointer to the codec context for the video stream
//    pCodecCtxOrig=pFormatCtx->streams[stream.index]->codec;
//    // Find the decoder for the video stream
//    pCodec=avcodec_find_decoder(pCodecCtxOrig->codec_id);
//    if(pCodec==NULL) {
//      fprintf(stderr, "Unsupported codec!\n");
//      return -1; // Codec not found
//    }

//    // Copy context
//    pCodecCtx = avcodec_alloc_context3(pCodec);
//    if(avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0) {
//      fprintf(stderr, "Couldn't copy codec context");
//      return -1; // Error copying codec context
//    }

//    // Open codec
//    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
//      return -1; // Could not open codec



//    // Allocate video frame
//     pFrame=av_frame_alloc();
//     while( av_read_frame(pFormatCtx, &packet)>=0) {
//         // Is this a packet from the video stream?
//         if(packet.stream_index==videoStream) {
//           // Decode video frame
//           avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

//        }
//    }
}

uint64_t Format::playtime() const {
    int64_t _millis;
    if ( format_->input_format_context->duration != AV_NOPTS_VALUE ) {
        uint64_t duration = format_->input_format_context->duration + ( format_->input_format_context->duration <= INT64_MAX - 5000 ? 5000 : 0 );
        _millis  = duration / AV_TIME_BASE;
    }
    return _millis;
}

std::string Format::time_to_string( int playtime ) {
  int _seconds = playtime / 1000;
  std::stringstream ss;
  if(_seconds >= 3600) {
    int hours = _seconds / 3600;
    if(hours < 10) {
      ss << "0";
    }
    ss << hours << ":";
    _seconds = _seconds - (hours * 3600);
  } else ss << "00:";
  if(_seconds >= 60) {
    int minutes = _seconds / 60;
    if(minutes < 10) {
      ss << "0";
    }
    ss << minutes << ":";
    _seconds = _seconds - (minutes * 60);
  } else ss << "00:";
  if(_seconds < 10) {
    ss << "0";
  }
  ss << _seconds << ".";
  int _millis = (playtime - ( ( playtime / 1000 ) * 1000 ) );
  if( _millis < 100 )  ss << "0";
  if( _millis < 10 )  ss << "0";
  ss << _millis;
  return ss.str();
}

}//namespace av
