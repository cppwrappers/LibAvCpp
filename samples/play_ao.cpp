#include <iostream>

#include <ao.h>

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

void die(const char* message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);

}
std::string message ( int ev ) {
    static char error_buffer[255];
    av_strerror ( ev, error_buffer, sizeof ( error_buffer ) );
    return error_buffer;
}

int main( int argc, char* argv[] ) {
    // Initialize FFmpeg
    av_register_all();
    // To initalize libao for playback
    ao_initialize();
    int driver = ao_default_driver_id();

    AVFrame* frame = avcodec_alloc_frame();
    if (!frame)
    {
        std::cout << "Error allocating the frame" << std::endl;
        return 1;
    }

    // you can change the file name "01 Push Me to the Floor.wav" to whatever the file is you're reading, like "myFile.ogg" or
    // "someFile.webm" and this should still work
    AVFormatContext* formatContext = NULL;
    if (avformat_open_input(&formatContext, argv[1], NULL, NULL) != 0)
    {
        av_free(frame);
        std::cout << "Error opening the file" << std::endl;
        return 1;
    }

    if (avformat_find_stream_info(formatContext, NULL) < 0)
    {
        av_free(frame);
 //TODO       av_close_input_file(formatContext);
        std::cout << "Error finding the stream info" << std::endl;
        return 1;
    }

    AVStream* audioStream = NULL;
    // Find the audio stream (some container files can have multiple streams in them)
    for (unsigned int i = 0; i < formatContext->nb_streams; ++i)
    {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audioStream = formatContext->streams[i];
            break;
        }
    }

    if (audioStream == NULL)
    {
        av_free(frame);
//TODO        av_close_input_file(formatContext);
        std::cout << "Could not find any audio stream in the file" << std::endl;
        return 1;
    }

    AVCodecContext* codecContext = audioStream->codec;

    codecContext->codec = avcodec_find_decoder(codecContext->codec_id);
    if (codecContext->codec == NULL)
    {
        av_free(frame);
//TODO        av_close_input_file(formatContext);
        std::cout << "Couldn't find a proper decoder" << std::endl;
        return 1;
    }
    else if (avcodec_open2(codecContext, codecContext->codec, NULL) != 0)
    {
        av_free(frame);
//TODO        av_close_input_file(formatContext);
        std::cout << "Couldn't open the context with the decoder" << std::endl;
        return 1;
    }

    std::cout << "This stream has " << codecContext->channels << " channels and a sample rate of " << codecContext->sample_rate << "Hz" << std::endl;
    std::cout << "The data is in the format " << av_get_sample_fmt_name(codecContext->sample_fmt) << std::endl;

            ao_sample_format sample_format;
            sample_format.bits = codecContext->bits_per_raw_sample;
            sample_format.channels = codecContext->channels;
            sample_format.rate = codecContext->sample_rate;
            sample_format.byte_format = AO_FMT_NATIVE;
            sample_format.matrix = 0;

            ao_device* device = ao_open_live(driver, &sample_format, NULL);

    AVPacket packet;
    av_init_packet(&packet);

    // Read the packets in a loop
    while (av_read_frame(formatContext, &packet) == 0)
    {
        if (packet.stream_index == audioStream->index)
        {
            // Try to decode the packet into a frame
            int frameFinished = 0;
            avcodec_decode_audio4(codecContext, frame, &frameFinished, &packet);

            // Some frames rely on multiple packets, so we have to make sure the frame is finished before
            // we can use it
            if (frameFinished) {
                std::cout << "play frame:" << *frame->linesize << std::endl;

                ao_play(device, (char*)frame->extended_data[0], (uint32_t)*frame->linesize);
                // frame now has usable audio data in it. How it's stored in the frame depends on the format of
                // the audio. If it's packed audio, all the data will be in frame->data[0]. If it's in planar format,
                // the data will be in frame->data and possibly frame->extended_data. Look at frame->data, frame->nb_samples,
                // frame->linesize, and other related fields on the FFmpeg docs. I don't know how you're actually using
                // the audio data, so I won't add any junk here that might confuse you. Typically, if I want to find
                // documentation on an FFmpeg structure or function, I just type "<name> doxygen" into google (like
                // "AVFrame doxygen" for AVFrame's docs)
            }
        }

        // You *must* call av_free_packet() after each call to av_read_frame() or else you'll leak memory
        av_free_packet(&packet);
    }

    // Some codecs will cause frames to be buffered up in the decoding process. If the CODEC_CAP_DELAY flag
    // is set, there can be buffered up frames that need to be flushed, so we'll do that
    if (codecContext->codec->capabilities & CODEC_CAP_DELAY)
    {
        av_init_packet(&packet);
        // Decode all the remaining frames in the buffer, until the end is reached
        int frameFinished = 0;
        while (avcodec_decode_audio4(codecContext, frame, &frameFinished, &packet) >= 0 && frameFinished)
        {
        }
    }

    // Clean up!
    av_free(frame);
    avcodec_close(codecContext);
//TODO    av_close_input_file(formatContext);
    ao_shutdown();
}


//int main( int argc, char* argv[] ) {

//    if (argc < 2) {
//        std::cerr << "Please provide file(s) path as argument(s)" << std::endl;
//        return -1;
//    }

//    // To initalize libao for playback
//    ao_initialize();
//    int driver = ao_default_driver_id();

//    //Initialize Format Context
//    libav::FormatContext _format_context;
//    std::error_code _errc;
//    if( ( _errc = _format_context.input( argv[1] ) ) ) {
//        std::cerr << _errc.message() << std::endl;
//        return -1;
//    }

//    //load metadata
//    //search the audio stream
//    int stream_number = 0;
//    for( unsigned int i=0; i<_format_context.count(); ++i ) {
//        if( _format_context.stream( i )->type() == libav::MediaType::AUDIO ) {
//            stream_number = i;
//            break;
//        }
//    }
//    if( stream_number != libav::AvIoStream::NPOS ) {

//        auto _stream = _format_context.stream( stream_number );

//        // The format of the decoded PCM samples
//        ao_sample_format sample_format;
//        sample_format.bits = _stream->bits_per_sample();
//        sample_format.channels = _stream->channels();
//        sample_format.rate = _stream->samplerate();
//        sample_format.byte_format = AO_FMT_NATIVE;
//        sample_format.matrix = 0;

//        ao_device* device = ao_open_live(driver, &sample_format, NULL);

//        std::cout << "Playing: " << argv[1] << " (" <<
//                     _format_context.duration() << ")\n" <<
//                     *_stream << "\n" <<
//                     _format_context.metadata() << std::endl;

//        int buffer_size = libav::AvIoStream::MAX_AUDIO_FRAME_SIZE;
//        int8_t buffer[libav::AvIoStream::MAX_AUDIO_FRAME_SIZE];

////        while (1) {
//            buffer_size = libav::AvIoStream::MAX_AUDIO_FRAME_SIZE;

//            std::cout << "start read buffer" << std::endl;
//            _stream->read( (char*)buffer, buffer_size );
//            std::cout << "end read buffer" << std::endl;
////            // Read one packet into `packet`
////            if (av_read_frame(container, &packet) < 0) {
////            break;  // End of stream. Done decoding.
////            }

////            // Decodes from `packet` into the buffer
////            if (avcodec_decode_audio3(codec_context, (int16_t*)buffer, &buffer_size, &packet) < 1) {
////            break;  // Error in decoding
////            }

//            // Send the buffer contents to the audio device
////            ao_play(device, (char*)buffer, buffer_size);
////        }
//    }





////    const char* input_filename = argv[1];

////    // This call is necessarily done once in your app to initialize
////    // libavformat to register all the muxers, demuxers and protocols.
////    av_register_all();

////    // A media container
////    AVFormatContext* container = 0;

////    if (avformat_open_input(&container, input_filename, NULL, NULL) < 0) {
////        die("Could not open file");
////    }

////    if (av_find_stream_info(container) < 0) {
////        die("Could not find file info");
////    }

////    int stream_id = -1;

////    // To find the first audio stream. This process may not be necessary
////    // if you can gurarantee that the container contains only the desired
////    // audio stream
////    int i;
////    for (i = 0; i < container->nb_streams; i++) {
////        if (container->streams[i]->codec->codec_type == CODEC_TYPE_AUDIO) {
////            stream_id = i;
////            break;
////        }
////    }

////    if (stream_id == -1) {
////        die("Could not find an audio stream");
////    }

////    // Extract some metadata
////    AVDictionary* metadata = container->metadata;

////    const char* artist = av_dict_get(metadata, "artist", NULL, 0)->value;
////    const char* title = av_dict_get(metadata, "title", NULL, 0)->value;

////    fprintf(stdout, "Playing: %s - %s\n", artist, title);






//    //    // Find the apropriate codec and open it
////    AVCodecContext* codec_context = container->streams[stream_id]->codec;

////    AVCodec* codec = avcodec_find_decoder(codec_context->codec_id);

////    if (!avcodec_open(codec_context, codec) < 0) {
////        die("Could not find open the needed codec");
////    }

////    // To initalize libao for playback
////    ao_initialize();

////    int driver = ao_default_driver_id();

////    // The format of the decoded PCM samples
////    ao_sample_format sample_format;
////    sample_format.bits = 16;
////    sample_format.channels = 2;
////    sample_format.rate = 44100;
////    sample_format.byte_format = AO_FMT_NATIVE;
////    sample_format.matrix = 0;

////    ao_device* device = ao_open_live(driver, &sample_format, NULL);

////    AVPacket packet;
////    int buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
////    int8_t buffer[AVCODEC_MAX_AUDIO_FRAME_SIZE];

////    while (1) {

////        buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;

////        // Read one packet into `packet`
////        if (av_read_frame(container, &packet) < 0) {
////            break;  // End of stream. Done decoding.
////        }

////        // Decodes from `packet` into the buffer
////        if (avcodec_decode_audio3(codec_context, (int16_t*)buffer, &buffer_size, &packet) < 1) {
////            break;  // Error in decoding
////        }

////        // Send the buffer contents to the audio device
////        ao_play(device, (char*)buffer, buffer_size);
////    }

// //   av_close_input_file(container);
//    ao_shutdown();

////    fprintf(stdout, "Done playing. Exiting...");

//    return 0;
//}
