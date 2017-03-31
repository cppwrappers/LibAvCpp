#ifndef LIBAV_H
#define LIBAV_H

#include <iostream>
#include <string>
#include <system_error>

namespace av {

class __av;

class LibAv {
public:
    LibAv();

    std::error_code open( std::istream& input_stream );
    std::error_code open( std::string& filename );

    std::istream& read( char* s, std::streamsize count );
    std::streamsize readsome( char* s, std::streamsize count );
    std::streamsize gcount() const;
    std::istream::pos_type tellg();
    std::istream& seekg( std::istream::pos_type pos );
    std::istream& seekg( std::istream::off_type off, std::ios_base::seekdir dir);
    bool operator!() const;

private:

};
}//namespace av
#endif // LIBAV_H
