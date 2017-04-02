//#ifndef AVIOSTREAM_H
//#define AVIOSTREAM_H

//#include <iostream>
//#include <memory>
//#include <mutex>

//#include "codec.h"
//#include "mediatype.h"

//namespace av {

//class __AVFormatContext;

//class AvIoStream {
//public:
//    static const int MAX_AUDIO_FRAME_SIZE;
//    static const int NPOS;
//    AvIoStream ( const unsigned int index, std::shared_ptr< __AVFormatContext > av_format_context );

//    unsigned int index() const;
//    MediaType::Enum type() const;
//    Codec::Enum codec() const;
//    int bitrate() const;
//    int samplerate() const;
//    int channels() const;
//    int bits_per_sample() const;
//    int width() const;
//    int height() const;

//    friend std::ostream& operator<< ( std::ostream& stream, const AvIoStream& av_io_stream ) {
//        stream << av_io_stream.index_ << ":" << av_io_stream.type() << ":" << av_io_stream.codec() << " (" << av_io_stream.bitrate() << " kb/s, " <<
//               av_io_stream.samplerate() << " hz, " << av_io_stream.channels() << " channel(s), " <<
//               av_io_stream.bits_per_sample() << " bps, " <<
//               av_io_stream.width() << "x" << av_io_stream.height() << " px)";
//        return stream;
//    }

//    AvIoStream& read (char* s, std::streamsize n);

//private:
//    const unsigned int index_;
//    int finished_ = 0, data_present_ = 0;
//    std::once_flag _read_flag;
//    std::error_code errc_;
//    std::shared_ptr< __AVFormatContext > av_format_context_ = nullptr;
//};
//typedef std::shared_ptr< AvIoStream > stream_ptr;
//}//namespace libav
//#endif // AVIOSTREAM_H
