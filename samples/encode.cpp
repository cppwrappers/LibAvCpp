
#include <cmath>
#include <iostream>

#include <ao.h>

#include "../libavcpp.h"

int main( int argc, char* argv[] ) {

    if( argc != 2 ) {
        std::cout << "pass output filename for mp3 file." << std::endl;
        return -1;
    }
    std::cout << "open output: " << argv[1] << std::endl;
    av::Format _format( argv[1], av::Format::WRITE );
    av::Codec _dest_codec( _format, av::CODEC::MP2,
        av::Options(
            std::vector< std::pair< std::string, std::string > >( {
              std::pair< std::string, std::string >( {"b", "64000" } ),
              std::pair< std::string, std::string >( { "ar", "44100" } ),
              std::pair< std::string, std::string >( { "ac", "2" } )
              /*TODO , { "channel_layout", 2 }*/
            } )
        ) );
    if( ! _dest_codec.sample_fmt( av::SampleFormat::SAMPLE_FMT_S16) ) {
        std::cerr << "can not set sample format." << std::endl;
        return -1;
    }
    av::Frame _output_frame( _dest_codec, _dest_codec.frame_size() );
    av::Packet _dest_packet;

    /* encode a single tone sound */
    float t = 0;
    float tincr = 2 * 3.14 * 440.0 / _dest_codec.sample_rate();
    uint16_t *samples;

    for (int i = 0; i < 200; i++) {
        /* make sure the frame is writable -- makes a copy if the encoder
         * kept a reference internally */
        //TODO ret = av_frame_make_writable(frame);
        //if (ret < 0)
        //    exit(1);

        samples = (uint16_t*)_output_frame.data(0);

        for (int j = 0; j < _dest_codec.frame_size(); j++) {
            samples[2*j] = (int)(sin(t) * 10000);

            for (int k = 1; k < _dest_codec.channels(); k++)
                samples[2*j + k] = samples[2*j];
            t += tincr;
        }
        _format.encode( _dest_codec, _output_frame, _dest_packet );
        _format.write( _dest_packet );
    }
    //flush encoder
    _format.encode( _dest_codec, _output_frame, _dest_packet );
    _format.write( _dest_packet );

    return 0;
}
