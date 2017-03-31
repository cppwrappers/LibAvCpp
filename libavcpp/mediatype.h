#ifndef MEDIATYPE_H
#define MEDIATYPE_H

#include <array>
#include <iostream>
#include <string>

namespace libav {
struct MediaType {
    enum Enum {
        UNKNOWN = -1,
        VIDEO = 0,
        AUDIO = 1,
        DATA = 2,
        SUBTITLE = 3,
        ATTACHMENT = 4,
        NB = 5
    };
    static std::string name ( Enum type );
    friend std::ostream& operator<< ( std::ostream& stream, const Enum& type ) {
        stream << MediaType::name ( type );
        return stream;
    }
private:
    static std::array< std::string, 6 > type_names_;
};
}//namespace libav
#endif // MEDIATYPE_H
