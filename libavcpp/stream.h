#ifndef STREAM_H
#define STREAM_H

#include "codec.h"
#include "codectype.h"

namespace av {
class Stream {
public:
    Stream() {}
    Stream(unsigned int index, CodecType::Enum codec_type, libav::Codec::Enum codec, int bitrate,
           int samplerate, int channels, int bits_per_sample, int width, int height, int pixel_format ) :
            index(index), codec_type(codec_type), codec(codec), bitrate(bitrate), samplerate(samplerate),
            channels(channels), bits_per_sample(bits_per_sample), width(width), height(height), pixel_format( pixel_format ) {}
    ~Stream() {}

    unsigned int index;
    CodecType::Enum codec_type;
    libav::Codec::Enum codec; //TODO namespace
    int bitrate;
    int samplerate;
    int channels;
    int bits_per_sample;
    int width;
    int height;
    int pixel_format;

    friend std::ostream& operator<< ( std::ostream& stream, const Stream& av_io_stream ) {
        stream << av_io_stream.index << ":" << av_io_stream.codec_type << ":" << av_io_stream.codec << " (" << av_io_stream.bitrate << " kb/s, " <<
               av_io_stream.samplerate << " hz, " << av_io_stream.channels << " channel(s), " <<
               av_io_stream.bits_per_sample << " bps, " <<
               av_io_stream.width << "x" << av_io_stream.height << " px)";
        return stream;
    }

private:

};
}//namespace av
#endif // STREAM_H
