#include <iostream>

#include <ao.h>

#include "../libavcpp.h"

int main( int /*argc*/, char* argv[] ) {

    //open the audiofile and get codec
    av::Format _format( argv[1] );
    if( !!_format ) {
        std::cerr << _format.errc().message() << std::endl;
        return _format.errc().value();
    }
    auto codec = _format.find_codec( av::CODEC_TYPE::AUDIO );
    std::cout << _format.metadata() << std::endl;

    //initialize ao
    ao_initialize();
    int driver = ao_default_driver_id();

    ao_sample_format sample_format;
    sample_format.bits = codec->bits_per_sample();
    sample_format.channels = codec->channels();
    sample_format.rate = codec->sample_rate();
    sample_format.byte_format = AO_FMT_NATIVE;
    sample_format.matrix = 0;
    ao_device* device = ao_open_live(driver, &sample_format, NULL);

    //play
    av::Packet _packet;
    while( !_format.read( _format.find_codec( av::CODEC_TYPE::AUDIO ), _packet ) ) {
        av::Frame _frame;
        std::error_code _decode_error;
        if( ! ( _decode_error = _format.decode( _packet, _frame ) ) ) {
            ao_play(device, (char*)_frame.extended_data()[0], _frame.linesize() );
        }
        if( !!_decode_error ) std::cout << "error decoding: " << _decode_error.message() << std::endl;
    }

    if( _format.errc() ) std::cerr << "Format::" << _format.errc().message() << std::endl;
    ao_shutdown();
    return 0;
}
