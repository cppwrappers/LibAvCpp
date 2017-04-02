//#include "aviostream.h"

//extern "C" {
//#include "libavformat/avformat.h"
//#include "libavformat/avio.h"

//#include "libavcodec/avcodec.h"

//#include "libavutil/audio_fifo.h"
//#include "libavutil/avassert.h"
//#include "libavutil/avstring.h"
//#include "libavutil/frame.h"
//#include "libavutil/opt.h"
//#include "libavutil/fifo.h"
//#include "libavutil/mathematics.h"

//#include "libswresample/swresample.h"

//#ifdef __cplusplus
//#define __STDC_CONSTANT_MACROS
//#ifdef _STDINT_H
//#undef _STDINT_H
//#endif
//# include <stdint.h>
//#endif
//}

//#include "__avformatcontext.h"

//namespace av {
//const int AvIoStream::MAX_AUDIO_FRAME_SIZE = 192000; //1 second of 48khz 32bit audio
//const int AvIoStream::NPOS = -1;

//inline MediaType::Enum __type ( AVMediaType type ) {
//    switch ( type ) {
//    case AVMEDIA_TYPE_VIDEO:
//        return MediaType::VIDEO;

//    case AVMEDIA_TYPE_AUDIO:
//        return MediaType::AUDIO;

//    case AVMEDIA_TYPE_DATA:
//        return MediaType::DATA;

//    case AVMEDIA_TYPE_SUBTITLE:
//        return MediaType::SUBTITLE;

//    case AVMEDIA_TYPE_ATTACHMENT:
//        return MediaType::ATTACHMENT;

//    case AVMEDIA_TYPE_NB:
//        return MediaType::NB;

//    default:
//        return MediaType::UNKNOWN;
//    }
//}
//inline Codec::Enum __codec ( AVCodecID codec_id ) {
//    switch ( codec_id ) {
//    case AV_CODEC_ID_MP3:
//        return Codec::MP3;

//    case AV_CODEC_ID_FLAC:
//        return Codec::FLAC;

//    case AV_CODEC_ID_AAC:
//        return Codec::AAC;

//    case AV_CODEC_ID_VORBIS:
//        return Codec::VORBIS;

//    default:
//        return Codec::NONE;
//    }
//}
//AvIoStream::AvIoStream ( const unsigned int index, std::shared_ptr< __AVFormatContext > av_format_context ) :
//    index_(index), av_format_context_(av_format_context) {
//    errc_ = av_format_context_->make_input_codec_context( index_ );
//}

//unsigned int AvIoStream::index() const
//{ return index_; }
//MediaType::Enum AvIoStream::type() const
//{ return ( __type ( av_format_context_->input_format_context->streams[index_]->codec->codec_type ) ); }
//Codec::Enum AvIoStream::codec() const
//{ return ( __codec ( av_format_context_->input_format_context->streams[index_]->codec->codec_id ) ); }
//int AvIoStream::bitrate() const
//{ return ( av_format_context_->input_format_context->streams[index_]->codec->bit_rate ); }
//int AvIoStream::samplerate() const
//{ return ( av_format_context_->input_format_context->streams[index_]->codec->sample_rate ); }
//int AvIoStream::channels() const
//{ return ( av_format_context_->input_format_context->streams[index_]->codec->channels ); }
//int AvIoStream::bits_per_sample() const
//{ return ( av_format_context_->input_format_context->streams[index_]->codec->bits_per_raw_sample ); }
//int AvIoStream::width() const
//{ return ( av_format_context_->input_format_context->streams[index_]->codec->width ); }
//int AvIoStream::height() const
//{ return ( av_format_context_->input_format_context->streams[index_]->codec->height ); }


//AvIoStream& AvIoStream::read ( char* s, std::streamsize n ) {

//    if( !errc_ ) {
//        av_format_context_->read();

////        /** Initialize temporary storage for one input frame. */
////        std::shared_ptr< AVFrame > input_frame = std::shared_ptr< AVFrame > ( av_frame_alloc(),
////        [] ( AVFrame* p ) { if ( p ) { av_frame_free ( &p ); } } );
////        if ( ! input_frame ) { errc_ = make_error_code( ENOMEM ); return *this; }

////        /** Decode one frame worth of audio samples. */
////        if ( ( errc_ = av_format_context_->decode_audio_frame ( input_frame.get(), &data_present_, &finished_ ) ) )
////        { std::cout << "E:" << errc_.message() << std::endl; return *this; }

////        /* If we are at the end of the file and there are no more samples */
////        if ( finished_ && !data_present_ )
////        { return *this; }


//    //    /** Initialize temporary storage for one input frame. */
//    //    std::shared_ptr< AVFrame > input_frame = std::shared_ptr< AVFrame > ( av_frame_alloc(),
//    //    [] ( AVFrame* p ) { if ( p ) { av_frame_free ( &p ); } } );
//    //    if ( ! input_frame ) { errc_ = make_error_code( ENOMEM ); return *this; }

//    //    /* Decode the audio frame stored in the temporary packet. */
//    //    if ( ( _error = avcodec_decode_audio4 ( av_format_context_->input_codec_context.get(), input_frame.get(),
//    //                                           &data_present_, av_format_context_->input_packet.get() ) ) < 0 )
//    //    { errc_ = make_error_code( ENOMEM ); return *this; }

//    //    std::cout << "Packet Size: " << input_frame->pkt_size << std::endl;

//    //    /**
//    //     * If the decoder has not been flushed completely, we are not finished,
//    //     * so that this function has to be called again.
//    //     */
//    //    if ( finished_ && data_present_ )
//    //    { finished_ = 0; }
//    }
//    return *this;
//}
//}//namespace libav
