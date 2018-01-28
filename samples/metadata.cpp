#include <iostream>

#include "../libavcpp.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Please provide a file path as argument(s)" << std::endl;
        return -1;
    }

    av::Format format( argv[1] );
    if( !format ) {
        av::Metadata metadata = format.metadata();
        std::cout << argv[1] << " (" <<
            av::time_to_string( format.playtime() ) << " sec/" <<
            /* format.find().size() << " streams )*/ "\n";

        for( auto& __stream : format ) {
            std::cout  << __stream << "\n";
        }
        std::cout << metadata << std::endl;

    } else std::cerr << format.errc().message();
    return 0;
}
