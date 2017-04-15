#ifndef CODEC_H
#define CODEC_H

#include <array>
#include <iostream>
#include <memory>
#include <string>

#include "_constants.h"
#include "format.h"

namespace av {
///@cond DOC_INTERNAL
struct __codec_context;
///@endcond DOC_INTERNAL

/** @brief The Codec struct */
struct Codec {

    Codec() : index_( -1 ) {}
    Codec( int index, std::shared_ptr< __codec_context > context ) : index_(index), codec_context_( context ) {}
    Codec( int index, Codec& codec, options_t options = options_t() );
    Codec( int index, Format& format_context, CODEC::Enum codec, options_t options );

    Codec(const Codec&) = default;
    Codec& operator=(const Codec&) = default;
    Codec(Codec&&) = default;
    Codec& operator=(Codec&&) = default;

    ~Codec() {}

    int index() const
    { return index_; }

    CODEC_TYPE::Enum codec_type() const;
    CODEC::Enum codec() const;
    int bitrate() const;
    int samplerate() const;
    int channels() const;
    int bits_per_sample() const;
    int width() const;
    int height() const;
    int pixel_format() const;
    SampleFormat sample_format() const;

    /**
     * @brief Checks whether the codec has no errors.
     * Returns true if an error has occurred on the associated codec context.
     */
    explicit operator bool() const;
    /**
     * @brief checks if an error has occurred.
     * Returns true if an error has occurred on the associated codec context.
     * @return true if an error has occurred, false otherwise.
     */
    bool operator!() const;
    /**
     * @brief checks if no error has occurred.
     * @return
     */
    bool good();
    /**
     * @brief checks if an error has occurred
     * @return
     */
    bool fail();
    /**
     * @brief return the actual error state of the assiciated codec context.
     * @return return the error state as std::error_code.
     */
    std::error_code errc ();

    /**
     * @brief write the codec definitions to the output stream.
     * @param stream the target output stream.
     * @param codec the codec to print
     * @return reference to output stream
     */
    friend std::ostream& operator<< ( std::ostream& stream, const Codec& codec ) {
        stream << name( codec.codec_type() ) << ":" << name( codec.codec() ) <<  " (" << codec.bitrate() << " kb/s, " <<
               codec.samplerate() << " hz, " << codec.channels() << " channel(s), " <<
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
    friend class Packet;
    int index_;
    std::error_code errc_;
    std::shared_ptr< __codec_context > codec_context_ = nullptr;
    static std::array< std::string, 4 > codec_names_;
    static std::array< std::string, 6 > codec_type_names_;
    static std::string name ( CODEC::Enum codec );
    static std::string name ( CODEC_TYPE::Enum codec );
};
}//namespace libav
#endif // CODEC_H
