#ifndef CODEC_H
#define CODEC_H

#include <array>
#include <iostream>
#include <memory>
#include <string>

#include "_constants.h"
#include "format.h"

///@cond DOC_INTERNAL
extern "C" {
    struct AVCodecContext;
}
///@endcond DOC_INTERNAL

namespace av {

/** @brief The Codec struct */
struct Codec {

    /** create output codec */
    Codec ( Format& format_context, CODEC::Enum codec, Options options );

    Codec ( const Codec& codec ) = delete;
    Codec& operator= ( const Codec& codec ) = delete;
    Codec ( Codec&& );
    Codec& operator= ( Codec&& );
    ~Codec();

    CODEC_TYPE::Enum codec_type() const;
    CODEC::Enum codec() const;
    int bitrate() const;
    int sample_rate() const;
    int channels() const;
    int bits_per_sample() const;
    int width() const;
    int height() const;
    int pixel_format() const;
    SampleFormat sample_fmt() const;
    bool sample_fmt( SampleFormat format );

    int frame_size();
    /** \brief Audio channel layout.
    encoding: set by user.
    decoding: set by user, may be overwritten by libavcodec.
     */
    uint64_t channel_layout();

    /**
     * @brief write the codec definitions to the output stream.
     * @param stream the target output stream.
     * @param codec the codec to print
     * @return reference to output stream
     */
    friend std::ostream& operator<< ( std::ostream& stream, const Codec& codec ) {
        stream << name ( codec.codec_type() ) << ":" << name ( codec.codec() ) <<  " (" << codec.bitrate() << " kb/s, " <<
               codec.sample_rate() << " hz, " << codec.channels() << " channel(s), " <<
               codec.bits_per_sample() << " bps, " <<
               codec.width() << "x" << codec.height() << " px)";
        return stream;
    }
    friend std::ostream& operator<< ( std::ostream& stream, const CODEC::Enum& codec ) {
        stream << name ( codec );
        return stream;
    }
    friend std::ostream& operator<< ( std::ostream& stream, const CODEC_TYPE::Enum& codec_type ) {
        stream << name ( codec_type );
        return stream;
    }

private:
    friend class AudioFifo;
    friend class Format;

    std::error_code errc_;
    Codec ( AVCodecContext* codec, Options options );
    AVCodecContext* codec_context_ = nullptr;

    /* some helper methods */
    static std::array< std::string, 4 > codec_names_;
    static std::array< std::string, 6 > codec_type_names_;
    static std::string name ( CODEC::Enum codec );
    static std::string name ( CODEC_TYPE::Enum codec );
};
}//namespace libav
#endif // CODEC_H
