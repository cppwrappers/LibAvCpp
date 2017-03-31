#include <iostream>

#include "../libavcpp/formatcontext.h"
#include "../libavcpp/aviostream.h"

int main(int /*argc*/, char *argv[]) {

    libav::FormatContext _format_context;
    std::error_code _errc;
    if( ( _errc = _format_context.input( argv[1] ) ) ) {
        std::cerr << _errc.message() << std::endl;
        return -1;
    }

    std::cout << argv[1] << " (" <<
        _format_context.duration() << " sec/" <<
        _format_context.count() << " streams)\n";

    for( int i=0; i<_format_context.count(); ++i ) {
        auto _input_stream = _format_context.stream( i );
        std::cout  << *_input_stream << "\n";
    }
    std::cout << _format_context.metadata() << std::endl;
    return 0;
}
