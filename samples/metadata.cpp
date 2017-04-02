#include <iostream>

#include "libavcpp.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Please provide a file path as argument(s)" << std::endl;
        return -1;
    }

    av::Format format;
    std::error_code _errc;
    _errc = format.open( argv[1] );
    if( !_errc ) {
        av::Metadata metadata = format.metadata();
        std::cout << argv[1] << " (" <<
            av::Format::time_to_string( format.playtime() ) << " sec/" <<
            format.streams().size() << " streams)\n";

        for( auto& __stream : format.streams() ) {
            std::cout  << __stream << "\n";
        }
        std::cout << metadata << std::endl;

    } else std::cerr << _errc.message();
    return 0;
}
