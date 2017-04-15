#include "__avformat.h"

#include "averrc.h"

namespace av {

//template< class... Options >
//av_codec_context_ptr __avformat::make_output_codec_context ( std::error_code* errc, format_context_ptr output_format_context, Codec::CODEC codec, Options... options ) {

//    av_codec_context_ptr _output_codec_context = nullptr;
//    /* Find the encoder to be used by its name. */
//    AVCodec* _output_codec = nullptr;

//    if ( ! ( _output_codec = avcodec_find_encoder ( av_codec_id ( codec ) ) ) ) {
//        *errc = make_error_code ( AV_ENCODER_NOT_FOUND );
//        return _output_codec_context;
//    }

//    /** Create a new audio stream in the output file container. */
//    AVStream *_stream = nullptr;
//    if ( ! ( _stream = avformat_new_stream ( output_format_context.get(), _output_codec ) ) ) {
//        *errc = make_error_code ( ENOMEM );
//        return _output_codec_context;
//    }

//    /* Set the basic encoder parameters. */
////    output_codec_context->channels       = codec.channels();
////    output_codec_context->channel_layout = av_get_default_channel_layout ( codec.channels() );
////    output_codec_context->sample_rate    = codec.samplerate();
////    output_codec_context->sample_fmt     = _output_codec_context->sample_fmts[0];
////    output_codec_context->bit_rate       = codec.bitrate();

//    _output_codec_context = _stream->codec;
//    /* Allow the use of the experimental AAC encoder */
//    _output_codec_context->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

//    /* Set the sample rate for the container. */
//    _stream->time_base.den = _output_codec_context->sample_rate;
//    _stream->time_base.num = 1;

//    /* Set global headers */
//    if ( output_format_context->oformat->flags & AVFMT_GLOBALHEADER )
//    { _output_codec_context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; }

//    /* Open the encoder for the audio stream to use it later. */
//    int error;
//    __AvOptions<Options...> _options( options... );
//    if ( ( error = avcodec_open2 ( _output_codec_context, _output_codec, _options.get() ) ) < 0 ) {
//        *errc = make_error_code ( error );
//        return _output_codec_context;
//    }
//    return _output_codec_context;
//}
//resample_context_ptr __avformat::make_resampler_context( std::error_code* errc, av_codec_context_ptr input_codec_context, av_codec_context_ptr output_codec_context ) {

//    resample_context_ptr _resample_context;

//    /* Create a resampler context for the conversion. */
//    SwrContext *re_context;
//    re_context = swr_alloc_set_opts ( nullptr /*existing context*/,
//                                      av_get_default_channel_layout ( output_codec_context->channels ),
//                                      output_codec_context->sample_fmt,
//                                      output_codec_context->sample_rate,
//                                      av_get_default_channel_layout ( input_codec_context->channels ),
//                                      input_codec_context->sample_fmt,
//                                      input_codec_context->sample_rate,
//                                      0 /*offset*/, nullptr /*parent log context*/ );

//    if ( !re_context ) {
//        *errc = make_error_code ( ENOMEM );
//        return _resample_context;
//    }

//    int _error;
//    if ( ( _error = swr_init ( re_context ) ) < 0 ) {
//        swr_free ( &re_context );
//        *errc = make_error_code ( _error );
//        return _resample_context;
//    }

//    return resample_context_ptr( re_context,
//    [] ( SwrContext* p ) { if ( p ) { swr_free ( &p ); } } );
//}
//fifo_ptr __avformat::make_fifo( std::error_code* errc, av_codec_context_ptr output_codec_context ) {

//    fifo_ptr _fifo = nullptr;
//    /** Create the FIFO buffer based on the specified output sample format. */
//    AVAudioFifo* __fifo = NULL;
//    if ( ! ( __fifo = av_audio_fifo_alloc ( output_codec_context->sample_fmt,
//                                           output_codec_context->channels, 1 ) ) ){
//        *errc = make_error_code ( ENOMEM );
//        return _fifo;
//    }

//    _fifo = fifo_ptr ( __fifo,
//        [] ( AVAudioFifo* p ) { if ( p ) { av_audio_fifo_free ( p ); } } );

//    return _fifo;
//}
//void __avformat::init_converted_samples( std::error_code* errc, uint8_t ***converted_input_samples, int frame_size, av_codec_context_ptr output_codec_context ) {
//    int error;

//    /**
//     * Allocate as many pointers as there are audio channels.
//     * Each pointer will later point to the audio samples of the corresponding
//     * channels (although it may be NULL for interleaved formats).
//     */
//    if (!(*converted_input_samples = (uint8_t**)calloc(output_codec_context->channels,
//                                            sizeof(**converted_input_samples)))) {
//        fprintf(stderr, "Could not allocate converted input sample pointers\n");
//        *errc = make_error_code( ENOMEM );
//        return;
//    }

//    /**
//     * Allocate memory for the samples of all channels in one consecutive
//     * block for convenience.
//     */
//    if ((error = av_samples_alloc(*converted_input_samples, NULL,
//                                  output_codec_context->channels,
//                                  frame_size,
//                                  output_codec_context->sample_fmt, 0)) < 0) {
//        av_freep(&(*converted_input_samples)[0]);
//        free(*converted_input_samples);
//        *errc = make_error_code( ENOMEM );
//        return;
//    }
//}
///**
// * Convert the input audio samples into the output sample format.
// * The conversion happens on a per-frame basis, the size of which is specified
// * by frame_size.
// */
//std::error_code __avformat::convert_samples(const uint8_t **input_data,
//                           uint8_t **converted_data, const int frame_size,
//                           SwrContext *resample_context) {
//    int error;

//    /** Convert the samples using the resampler. */
//    if ((error = swr_convert(resample_context,
//                             converted_data, frame_size,
//                             input_data    , frame_size)) < 0) {
//        return make_error_code( error );
//    }
//    return std::error_code ();
//}

//std::error_code __avformat::add_samples_to_fifo (
//        AVAudioFifo *fifo,
//        uint8_t **converted_input_samples,
//        const int frame_size ) {

//    int error;

//    if ( ( error = av_audio_fifo_realloc ( fifo, av_audio_fifo_size ( fifo ) + frame_size ) ) < 0 )
//    { return make_error_code ( error ); }

//    if ( av_audio_fifo_write ( fifo, ( void ** ) converted_input_samples, frame_size ) < frame_size )
//    { return make_error_code ( error ); }

//    return std::error_code ();
//}
///**
// * Load one audio frame from the FIFO buffer, encode and write it to the
// * output file.
// */
//void __avformat::load_encode_and_write( std::error_code* errc, AVAudioFifo *fifo, av_codec_context_ptr output_codec_context ) {
//    /** Temporary storage of the output samples of the frame written to the file. */
//    AVFrame *output_frame;
//    /**
//     * Use the maximum number of possible samples per frame.
//     * If there is less than the maximum possible frame size in the FIFO
//     * buffer use this number. Otherwise, use the maximum possible frame size
//     */
//    const int frame_size = FFMIN(av_audio_fifo_size(fifo),
//                                 output_codec_context->frame_size);
//    int data_written;

//    /** Initialize temporary storage for one output frame. */
//    init_output_frame( errc, &output_frame, frame_size, output_codec_context );
//    if ( errc ) { return; }

//    /**
//     * Read as many samples from the FIFO buffer as required to fill the frame.
//     * The samples are stored in the frame temporarily.
//     */
//    int _error;
//    if ( ( _error = av_audio_fifo_read(fifo, reinterpret_cast< void ** >( output_frame->data ), frame_size ) ) < 0 ) {
//        av_frame_free(&output_frame);
//        *errc = make_error_code( _error );
//        return;
//    }

//    /** Encode one frame worth of audio samples. */
//    encode_audio_frame( errc, output_frame, output_codec_context, &data_written );
//    if ( errc ) { return; }

//    av_frame_free(&output_frame);
//}
///**
// * Initialize one input frame for writing to the output file.
// * The frame will be exactly frame_size samples large.
// */
//void __avformat::init_output_frame( std::error_code* errc, AVFrame **frame, int frame_size, av_codec_context_ptr output_codec_context ) {
//    int error;

//    /** Create a new frame to store the audio samples. */
//    if (!(*frame = av_frame_alloc())) {
//        *errc = make_error_code( ENOMEM );
//        return;
//    }

//    /**
//     * Set the frame's parameters, especially its size and format.
//     * av_frame_get_buffer needs this to allocate memory for the
//     * audio samples of the frame.
//     * Default channel layouts based on the number of channels
//     * are assumed for simplicity.
//     */
//    (*frame)->nb_samples     = frame_size;
//    (*frame)->channel_layout = output_codec_context->channel_layout;
//    (*frame)->format         = output_codec_context->sample_fmt;
//    (*frame)->sample_rate    = output_codec_context->sample_rate;

//    /**
//     * Allocate the samples of the created frame. This call will make
//     * sure that the audio frame can hold as many samples as specified.
//     */
//    if ((error = av_frame_get_buffer(*frame, 0)) < 0) {
//        av_frame_free(frame);
//        *errc = make_error_code( error );
//        return;
//    }
//}
///** Global timestamp for the audio frames */
//static int64_t pts = 0;

///** Encode one frame worth of audio to the output file. */
//void __avformat::encode_audio_frame( std::error_code* errc, AVFrame *frame, av_codec_context_ptr output_codec_context, int *data_present ) {
//    /** Packet used for temporary storage. */
//    AVPacket output_packet;

//    av_init_packet(&output_packet);
//    /** Set the packet data and size so that it is recognized as being empty. */
//    output_packet.data = NULL;
//    output_packet.size = 0;

//    /** Set a timestamp based on the sample rate for the container. */
//    if (frame) {
//        frame->pts = pts;
//        pts += frame->nb_samples;
//    }

//    /**
//     * Encode the audio frame and store it in the temporary packet.
//     * The output audio stream encoder is used to do this.
//     */
//    int error;
//    if ((error = avcodec_encode_audio2( output_codec_context, &output_packet,
//                                       frame, data_present)) < 0) {
//        av_free_packet(&output_packet);
//        *errc = make_error_code( error );
//        return;
//    }

//    /** Write one audio frame from the temporary packet to the output file. */
//    if (*data_present) {
//        if ((error = av_write_frame(output_format_context.get(), &output_packet)) < 0) {
//            av_free_packet(&output_packet);
//            *errc = make_error_code( error );
//            return;
//        }
//        av_free_packet(&output_packet);
//    }
//}


//void __avformat::write_output_metadata ( Metadata& metadata ) {
//    /** Write the header of the output file container. */
//    AVDictionary *d = output_format_context->metadata;

//    for ( auto _tag :metadata.tag_names() ) {
//        av_dict_set ( &d, Metadata::name ( _tag ).c_str(), metadata.get ( _tag ).c_str(), 0 );
//    }

//    av_dict_set ( &d, "duration", NULL, 0 );
//    output_format_context->metadata = d;
//}
//std::error_code __avformat::write_header () {
//    AVDictionary *format_opts = nullptr;

//    av_dict_set ( &format_opts, "id3v2_version", "3", 0 );
//    av_dict_set ( &format_opts, "write_id3v1", "1", 0 );

//    int _error;

//    if ( ( _error = avformat_write_header ( output_format_context.get(), &format_opts ) ) < 0 ) {
//        av_dict_free ( &format_opts );
//        return make_error_code ( _error );
//    }

//    av_dict_free ( &format_opts );
//    return std::error_code ();
//}
//std::error_code __avformat::write_trailer () {
//    int _error;

//    if ( ( _error = av_write_trailer ( output_format_context.get() ) ) < 0 )
//    { return std::error_code ( _error, av_category ); }

//    return std::error_code ();
//}
//AVCodecID __avformat::av_codec_id ( Codec::CODEC codec ) {
//    switch ( codec ) {
//    case Codec::CODEC::AAC:
//        return AV_CODEC_ID_AAC;

//    case Codec::CODEC::FLAC:
//        return AV_CODEC_ID_FLAC;

//    case Codec::CODEC::MP3:
//        return AV_CODEC_ID_MP3;

//    case Codec::CODEC::VORBIS:
//        return AV_CODEC_ID_VORBIS;

//    case Codec::CODEC::NONE:
//        break;
//    }
//    return AV_CODEC_ID_NONE;
//}
}//namespace av
