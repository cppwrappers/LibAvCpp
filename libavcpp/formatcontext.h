#ifndef FORMATCONTEXT_H
#define FORMATCONTEXT_H

#include <map>
#include <memory>
#include <string>
#include <system_error>

#include "metadata.h"
#include "aviostream.h"

namespace libav {

class __AVFormatContext;

class FormatContext {
public:
    FormatContext();
//    FormatContext & operator= ( const FormatContext& ) = delete;
//    FormatContext ( const FormatContext& ) = delete;
//    FormatContext && operator= ( const FormatContext&& ) = delete;
//    FormatContext ( const FormatContext&& ) = delete;

//    FormatContext( auto options = std::map<std::string, std::string>() );

    ~FormatContext();

    std::error_code input( const std::string& filename );
    unsigned int count();
    Metadata metadata ();
    stream_ptr stream ( const int& index );
    int64_t duration ();

private:
    std::shared_ptr< __AVFormatContext > av_format_context_ = nullptr;
    std::map< std::string, std::string > options_;
};
}//namespace libav
#endif // FORMATCONTEXT_H
