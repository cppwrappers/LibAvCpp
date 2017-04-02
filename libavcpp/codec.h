#ifndef CODEC_H
#define CODEC_H

#include <array>
#include <iostream>
#include <string>

namespace av {

/**
 * @brief The Codec struct
 */
struct Codec {
    enum Enum {
        NONE = -1, MP3=0, FLAC=1, AAC=2, VORBIS=3
    };
    static std::string name ( Enum codec );
    friend std::ostream& operator<< ( std::ostream& stream, const Codec::Enum& codec ) {
        stream << Codec::name ( codec );
        return stream;
    }
private:
    static std::array< std::string, 4 > codec_names_;
};
}//namespace libav
#endif // CODEC_H
