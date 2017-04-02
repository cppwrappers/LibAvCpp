#ifndef __AVFORMATCONTEXT_H
#define __AVFORMATCONTEXT_H

#include <memory>
#include <mutex>

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

class __AVFormatContext {
public:
    std::once_flag _register_flag;
    __AVFormatContext() {
        std::call_once ( _register_flag, []() { av_register_all(); } );
    }

    std::shared_ptr< AVPacket > input_packet = nullptr;
    std::shared_ptr < AVFormatContext > input_format_context;
    std::error_code make_input_format_context ( const char* filename ) {
        int _error;
        /** Open the input file to read from it. */
        AVFormatContext* _input_format_context = nullptr;

        if ( ( _error = avformat_open_input ( &_input_format_context, filename, NULL, NULL ) ) < 0 )
        { return av::make_error_code ( _error ); }

        input_format_context = std::shared_ptr< AVFormatContext > ( _input_format_context,
        [] ( AVFormatContext* p ) { if ( p ) { avformat_close_input ( &p ); } } );

        /** Get information on the input file (number of streams etc.). */
        if ( ( _error = avformat_find_stream_info ( input_format_context.get(), NULL ) ) < 0 )
        { return av::make_error_code ( _error ); };

        return std::error_code ();
    }

    std::shared_ptr < AVCodecContext > input_codec_context;
    std::error_code make_input_codec_context ( const int stream_number ) {

        /** Find a decoder for the audio stream. */
        AVCodec* codec_ = nullptr;

        if ( ! ( codec_ = avcodec_find_decoder ( input_format_context->streams[stream_number]->codec->codec_id ) ) ) {
            return make_error_code ( AV_DECODER_NOT_FOUND );
        }

        int error;

        /** Open the decoder for the audio stream to use it later. */
        if ( ( error = avcodec_open2 ( input_format_context.get()->streams[stream_number]->codec, codec_, NULL ) ) < 0 ) {
            return make_error_code ( error );
        }

        //store the codec for direct access
        input_codec_context = std::shared_ptr< AVCodecContext > ( input_format_context->streams[stream_number]->codec,
        [] ( AVCodecContext* p ) { if ( p ) { avcodec_close ( p ); } } );

        return std::error_code ();
    }

    std::error_code decode_audio_frame ( AVFrame *frame, int *data_present, int *finished ) {

        int error;

        /** Packet used for temporary storage. */
        std::shared_ptr< AVPacket > input_packet =
            std::shared_ptr< AVPacket > ( new AVPacket(), [] ( AVPacket* p ) { if ( p ) { av_free_packet ( p ); } } );
        av_init_packet ( input_packet.get() );
        input_packet->data = NULL;
        input_packet->size = 0;

        /** Read one audio frame from the input file into a temporary packet. */
        if ( ( error = av_read_frame ( input_format_context.get(), input_packet.get() ) ) < 0 ) {
            /** If we are at the end of the file, flush the decoder below. */
            if ( error == AVERROR_EOF ) { *finished = 1; }
            else { return make_error_code ( error ); }
        }

        /**
         * Decode the audio frame stored in the temporary packet.
         * The input audio stream decoder is used to do this.
         * If we are at the end of the file, pass an empty packet to the decoder
         * to flush it.
         */
        if ( ( error = avcodec_decode_audio4 ( input_codec_context.get(), frame,
                                               data_present, input_packet.get() ) ) < 0 ) {
            return make_error_code ( error );
        }

        /**
         * If the decoder has not been flushed completely, we are not finished,
         * so that this function has to be called again.
         */
        if ( *finished && *data_present )
        { *finished = 0; }

        return std::error_code ();
    }
    std::error_code read_decode_convert_and_store ( int *finished ) {

        int data_present;
        std::error_code error;

        /** Initialize temporary storage for one input frame. */
        std::shared_ptr< AVFrame > input_frame = std::shared_ptr< AVFrame > ( av_frame_alloc(),
            [] ( AVFrame* p ) { if ( p ) { av_frame_free ( &p ); } } );
        if ( ! input_frame ) { return std::error_code ( ENOMEM, av_category ); }

        /** Decode one frame worth of audio samples. */
        if ( ( error = decode_audio_frame ( input_frame.get(), &data_present, finished ) ) )
        { return error; }

        /* If we are at the end of the file and there are no more samples */
        if ( *finished && !data_present )
        { std::cout << "EOF" << std::endl; return std::error_code (); }

        /** If there is decoded data, convert and store it */
        std::cout << "data present: " << data_present << std::endl;
        if ( data_present ) {
            std::cout << "data present, do something" << std::endl;
//            //TODO reuse or resize
//            sample_buffer = std::make_shared< sample_t > ( input_codec_context, output_codec_context, input_frame->nb_samples );

//            if ( sample_buffer->error ) {
//                return sample_buffer->error;
//            }

//            /* convert to destination format */
//            int ret;

//            if ( ( ret = swr_convert ( resample_context.get(),
//                                       sample_buffer->data,
//                                       sample_buffer->dst_nb_samples,
//                                       ( const uint8_t** ) input_frame->extended_data,
//                                       input_frame->nb_samples ) ) < 0 )
//            { return make_error_code ( ret ); }

//            /** Add the converted input samples to the FIFO buffer for later processing. */
//            if ( ( error = add_samples_to_fifo ( fifo.get(), sample_buffer->data, sample_buffer->dst_nb_samples ) ) )
//            { return error; }
        }

        return std::error_code();
    }
    std::error_code read() {

        const int output_frame_size = 192000; //output_codec_context->frame_size;
        int finished = 0;
        std::error_code _errc;
        //read
        //while ( av_audio_fifo_size ( input_codec_context->fifo.get() ) < output_frame_size ) {
            if ( ( _errc = read_decode_convert_and_store ( &finished ) ) )
            { std::cout << _errc.message() << std::endl; return _errc; }
            if ( finished ) { return std::error_code (); }
        //}
//        //write
//        while ( av_audio_fifo_size ( fifo.get() ) >= output_frame_size ||
//                ( finished && av_audio_fifo_size ( fifo.get() ) > 0 ) ) {

//            if ( ( _errc = load_encode_and_write() ) )
//            { return _errc; }
//        }
//        //flush
//        if ( finished ) {
//            int data_written;
//            do {
//                if ( ( _errc = encode_audio_frame ( NULL, &data_written ) ) )
//                { return _errc; }
//            } while ( data_written );

//            break;
//        }

        return std::error_code ();
    }
};
}//namespace av
#endif // __AVFORMATCONTEXT_H
