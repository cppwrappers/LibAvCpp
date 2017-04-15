#ifndef __AVFORMAT_H
#define __AVFORMAT_H

#include <memory>
#include <system_error>

#include "codec.h"
#include "metadata.h"
#include "__iocontext.h"


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
#include "option.h"

namespace av {

typedef AVFormatContext* av_format_context_ptr;
typedef AVCodecContext* av_codec_context_ptr;
typedef AVPacket* av_packet_ptr;
typedef SwrContext* av_resample_context_ptr;
typedef AVFrame* av_frame_ptr;
typedef AVAudioFifo* av_fifo_ptr;

struct __av_options {
public:
    __av_options( options_t options ) {
        for( auto __option : options ) {
            switch( __option.type() ) {
            case Option::INT:
                av_dict_set_int( &opts, __option.key(), __option.c_int(), 0 );
                break;
            case Option::STRING:
                av_dict_set( &opts, __option.key(), __option.c_str(), 0 );
                break;
            }
        }
    }
    ~__av_options() {
        av_dict_free( &opts );
    }
    AVDictionary** get()
    { return &opts; }

private:
    AVDictionary *opts = NULL;
};

struct __av_format_context {
    __av_format_context( av_format_context_ptr format_context ) : format_context_( format_context ) {}
    __av_format_context( std::error_code* errc, const std::string& filename, bool write, options_t options ) {
        if( write ) {
            //open file for writing
            int error;

            /** Open the output file to write to it. */
            AVIOContext* _output_io_context = nullptr;

            if ( ( error = avio_open ( &_output_io_context, filename.c_str(), AVIO_FLAG_WRITE ) ) < 0 )
            { *errc = make_error_code ( error ); return; }

            if ( ! ( format_context_  = avformat_alloc_context() ) )
            { *errc = make_error_code ( error ); return; }

            /** Associate the output file (pointer) with the container format context. */
            format_context_->pb = _output_io_context ;

            /** Guess the desired container format based on the file extension. */
            if ( ! ( format_context_ ->oformat = av_guess_format ( NULL/*short name*/, filename.c_str(), NULL/*mime_type*/ ) ) )
            { *errc = make_error_code ( error ); return; }

            av_strlcpy ( format_context_->filename, filename.c_str(), sizeof ( format_context_->filename ) );

        } else {
            //open file for reading
            int _error;
            if ( ( _error = avformat_open_input ( &format_context_, filename.c_str(), NULL /*fmt*/, NULL /*TODO options*/ ) ) != 0 )
            { *errc = av::make_error_code ( _error ); return; }
            if ( ( _error = avformat_find_stream_info ( format_context_, NULL /*TODO options*/ ) ) < 0 )
            { *errc = av::make_error_code ( _error ); return; };
        }
    }

    __av_format_context( std::error_code* errc, std::iostream* stream, const std::string& codec, bool write, options_t options ) {
        if( write ) {
            //open stream for writing
            io_context_ = std::make_shared< IoContext >();
            format_context_ = avformat_alloc_context();
            format_context_->pb = io_context_->av_io_context_;
            format_context_->flags |= AVFMT_FLAG_CUSTOM_IO;

            AVOutputFormat* of = av_guess_format ( codec.c_str(), NULL, NULL );

            if ( of == NULL ) { make_error_code ( AV_ENCODER_NOT_FOUND );  }

            format_context_->oformat = of;

            if ( of->audio_codec == AV_CODEC_ID_NONE ) { make_error_code ( AV_ENCODER_NOT_FOUND );  }

            io_context_->init_output_format_context ( format_context_ );

        } else {
            //open stream for reading
            io_context_ = std::make_shared< IoContext >();
            format_context_ = avformat_alloc_context();
            io_context_->init_input_format_context ( format_context_, stream );

            int _error = 0;
            if ( ( _error = avformat_open_input ( &format_context_, "", nullptr, nullptr ) ) != 0 )
            { *errc = make_error_code ( _error ); return; };

            if ( ( _error = avformat_find_stream_info ( format_context_, nullptr ) ) < 0 )
            { *errc = make_error_code ( _error ); return; };

    //    //TODO only in debug mode
    //        av_dump_format(input_format_context_->format_context_.get(), 0, filename.c_str(), 0);
    //    //TODO only in debug mode
        }
    }

    ~__av_format_context() {
        if ( format_context_ )
        { avformat_close_input ( &format_context_ ); }
    }

    std::vector< std::shared_ptr< __codec_context > > codecs() const {

        std::vector< std::shared_ptr< __codec_context > > _codecs;
        for(unsigned short i=0; i<format_context_->nb_streams; i++) {
//            if( codec_type == CODEC_TYPE::NONE ||
//                codec_type == format_context_->format_context_->streams[i]->codec->codec_type ) { //TODO CONVERT ONE
                std::shared_ptr< __codec_context > _codec =
                    std::make_shared< __codec_context >( format_context_->streams[i]->codec );
                _codecs.push_back( _codec );
//            }
        }
        return _codecs;
    }

    AVPacket* read_packet( int* err ) {
        AVPacket* packet = new AVPacket; //make global for reuse
        av_init_packet ( packet );
        packet->data = NULL;
        packet->size = 0;
        *err = av_read_frame( format_context_, packet );
        return packet;
    }

    av_format_context_ptr format_context_ = nullptr;
    std::shared_ptr < IoContext > io_context_ = nullptr;
};
struct __av_packet {
    __av_packet( AVPacket* packet ) : packet_( packet ) {}
    ~__av_packet() {
        av_free_packet ( packet_ );
        delete packet_;
    }
    AVPacket* packet_;
};
struct __codec_context {
    __codec_context( av_codec_context_ptr codec ) : codec_context_( codec ) {}
    __codec_context( std::error_code* errc, av_codec_context_ptr output_codec, options_t options ) {
        AVCodec* codec_ = nullptr;
        codec_context_ = output_codec;

        if ( ! ( codec_ = avcodec_find_decoder ( codec_context_->codec_id ) ) ) {
            *errc = make_error_code ( AV_DECODER_NOT_FOUND );
            return;
        }
        int error;
        __av_options _av_dict ( options );
        if ( ( error = avcodec_open2 ( codec_context_, codec_, _av_dict.get() ) ) < 0 ) {
            *errc = make_error_code ( error );
            return;
        }
    }
    __codec_context( std::error_code* errc, av_format_context_ptr output_format_context, AVCodecID codec, options_t options ) {

            /* Find the encoder to be used by its name. */
            AVCodec* _output_codec = nullptr;
            if ( ! ( _output_codec = avcodec_find_encoder ( codec ) ) ) {
                *errc = make_error_code ( AV_ENCODER_NOT_FOUND );
                return;
            }

            /* Create a new audio stream in the output file container. */
            AVStream *_stream = nullptr;
            if ( ! ( _stream = avformat_new_stream ( output_format_context, _output_codec ) ) ) {
                *errc = make_error_code ( ENOMEM );
                return;
            }
            codec_context_ = _stream->codec;
            codec_context_->sample_fmt     = _output_codec->sample_fmts[0];
            codec_context_ = _stream->codec;
            /* Allow the use of the experimental AAC encoder */
            codec_context_->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;
            /* Set the sample rate for the container. */
            _stream->time_base.den = codec_context_->sample_rate;
            _stream->time_base.num = 1;

            /* Set global headers */
            if ( output_format_context->oformat->flags & AVFMT_GLOBALHEADER )
            { codec_context_->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; }

            /* Open the encoder for the audio stream to use it later. */
            int error;
            __av_options _options( options );
            if ( ( error = avcodec_open2 ( codec_context_, _output_codec, _options.get() ) ) < 0 ) {
                *errc = make_error_code ( error );
                return;
            }
        }

    ~__codec_context() {}
    av_codec_context_ptr codec_context_;
};
struct __av_frame {
    __av_frame () {

        if ( ! ( frame_ = av_frame_alloc() ) ) {
            error = AVERROR(ENOMEM);
//        } else {
//            error = avcodec_decode_audio4( codec_context, frame_, &data_present, packet );
        }

//        /**
//         * If the decoder has not been flushed completely, we are not finished,
//         * so that this function has to be called again.
//         */
//        if (*finished && *data_present)
//            *finished = 0;
//        av_free_packet(&input_packet);

    }
    ~__av_frame() {
        //TODO free?
    }

    void decode( av_codec_context_ptr codec_context, av_packet_ptr packet ) {
        error = avcodec_decode_audio4( codec_context, frame_, &data_present, packet );
    }

    int error;
    int data_present = 0;
    av_frame_ptr frame_;
};
struct __av_resample_context {
    __av_resample_context( int source_channels, AVSampleFormat source_sample_fmt, int source_sample_rate,
                           int target_channels, AVSampleFormat target_sample_fmt, int target_sample_rate,
                           options_t& options ) {

        __av_options _opt( options );
        resample_context_ = swr_alloc_set_opts(NULL,
                                              av_get_default_channel_layout(source_channels),
                                              source_sample_fmt,
                                              source_sample_rate,
                                              av_get_default_channel_layout(target_channels),
                                              target_sample_fmt,
                                              target_sample_rate,
                                              0, _opt.get() );
        if ( !resample_context_ )
        { error = AVERROR(ENOMEM); }

        //TODO remove
        av_assert0( source_sample_rate == target_sample_rate );

        /** Open the resampler with the specified parameters. */
        if ( ( error = swr_init( resample_context_ ) ) < 0) {
            swr_free( &resample_context_ );
        }
    }
    ~__av_resample_context() {
        if( resample_context_ )
        { swr_free( &resample_context_ ); }
    }

    int error;
    av_resample_context_ptr resample_context_ = nullptr;
};
struct  __av_fifo {
    __av_fifo( AVSampleFormat sample_fmt, int channels ) {
        /** Create the FIFO buffer based on the specified output sample format. */
        AVAudioFifo* __fifo = NULL;
        if ( ! ( __fifo = av_audio_fifo_alloc ( sample_fmt, channels, 1 ) ) ) {
            error = ENOMEM;
        }
    }

    ~__av_fifo() {
        if( fifo_ )
        { av_audio_fifo_free( fifo_ ); }
    }

    int write( av_resample_context_ptr resample_context,
               uint8_t **out, int out_count,
               const uint8_t **in , int in_count ) {
        /** Convert the samples using the resampler. */
        if ((error = swr_convert(resample_context,
                                 out, out_count,
                                 in, in_count)) < 0 ) {
            return error;
        }

    }

    int error;
    av_fifo_ptr fifo_ = nullptr;
};
}//namespace av
#endif // __AVFORMAT_H
