#include <iostream>

#include <ao.h>

#include "../libavcpp.h"

int main( int /*argc*/, char* argv[] ) {

    ao_initialize();
    int driver = ao_default_driver_id();

    av::Format format( argv[1] );
    if( !!format ) {
        std::cerr << format.errc().message() << std::endl;
        return format.errc().value();
    }
    std::cout << "play: " << argv[1] << std::endl;

    av::Codec codec;
    for( auto __codec : format ) {
        std::cout << "CODEC:" << *__codec;
        if( __codec->codec_type() == av::CODEC_TYPE::AUDIO ) {
            std::cout << " * ";
            codec = av::Codec( __codec->index(), *__codec );
        }
        std::cout << std::endl;
    }
    std::cout << format.metadata() << std::endl;

    ao_sample_format sample_format;
    sample_format.bits = codec.bits_per_sample();
    sample_format.channels = codec.channels();
    sample_format.rate = codec.samplerate();
    sample_format.byte_format = AO_FMT_NATIVE;
    sample_format.matrix = 0;

    ao_device* device = ao_open_live(driver, &sample_format, NULL);

    av::Packet packet;
    av::Frame frame;
    while( !format.read( packet ) ) {
        if( packet.stream_index() == codec.index() ) {
            if( !packet.decode( codec, frame ) ) {
                ao_play(device, (char*)frame.extended_data()[0], frame.linesize() );
            }
            if( packet.errc() ) std::cerr << "Packet::" << packet.errc().message() << std::endl;
        }
    }
    if( format.errc() ) std::cerr << "Format::" << format.errc().message() << std::endl;
    ao_shutdown();
    return 0;
}
