#ifndef STREAM_H
#define STREAM_H

#include "codec.h"
#include "codectype.h"

namespace av {
class Stream {
public:
    Stream() {}
    Stream(unsigned int index, CodecType::Enum codec_type, av::Codec::Enum codec, int bitrate,
           int samplerate, int channels, int bits_per_sample, int width, int height, int pixel_format ) :
            index_(index), codec_type_(codec_type), codec_(codec), bitrate_(bitrate), samplerate_(samplerate),
            channels_(channels), bits_per_sample_(bits_per_sample), width_(width), height_(height), pixel_format_( pixel_format ) {}
    ~Stream() {}

    unsigned int index() const
    { return index_; }
    CodecType::Enum codec_type() const
    { return codec_type_; }
    void codec_type( CodecType::Enum  codec_type )
    { codec_type_ = codec_type; }
    Codec::Enum codec() const
    { return codec_; }
    void codec( Codec::Enum  codec )
    { codec_ = codec; }
    int bitrate() const
    { return bitrate_; }
    void bitrate( int  bitrate )
    { bitrate_ = bitrate; }
    int samplerate() const
    { return samplerate_; }
    void samplerate( int  samplerate )
    { samplerate = samplerate; }
    int channels() const
    { return channels_; }
    void channels( int  channels )
    { channels_ = channels; }
    int bits_per_sample() const
    { return bits_per_sample_; }
    void bits_per_sample( int  bits_per_sample )
    { bits_per_sample = bits_per_sample; }
    int width() const
    { return width_; }
    void width( int  width )
    { width_ = width; }
    int height() const
    { return height_; }
    void height( int  height )
    { height_ = height; }
    int pixel_format() const
    { return pixel_format_; }
    void pixel_format( int  pixel_format )
    { pixel_format_ = pixel_format; }

    friend std::ostream& operator<< ( std::ostream& stream, const Stream& av_io_stream ) {
        stream << av_io_stream.index_ << ":" << av_io_stream.codec_type_ << ":" << av_io_stream.codec_ << " (" << av_io_stream.bitrate_ << " kb/s, " <<
               av_io_stream.samplerate_ << " hz, " << av_io_stream.channels_ << " channel(s), " <<
               av_io_stream.bits_per_sample_ << " bps, " <<
               av_io_stream.width_ << "x" << av_io_stream.height_ << " px)";
        return stream;
    }

private:
    unsigned int index_;
    CodecType::Enum codec_type_;
    Codec::Enum codec_;
    int bitrate_;
    int samplerate_;
    int channels_;
    int bits_per_sample_;
    int width_;
    int height_;
    int pixel_format_;
};
}//namespace av
#endif // STREAM_H
