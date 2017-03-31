#ifndef CODECTYPE_H
#define CODECTYPE_H

#include <array>
#include <iostream>
#include <string>

namespace av {

/**
 * @brief The Codec Enum
 */
struct CodecType {
    enum Enum {
        NONE = -1, AUDIO=0, VIDEO=1, DATA=2
    };
    static std::string name ( Enum codec );
    friend std::ostream& operator<< ( std::ostream& stream, const CodecType::Enum& codec_type ) {
        stream << CodecType::name ( codec_type );
        return stream;
    }
private:
    static std::array< std::string, 3 > codec_type_names_;
};
}//namespace av
#endif // CODECTYPE_H
