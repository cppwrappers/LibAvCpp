#ifndef PACKET_H
#define PACKET_H

#include <memory>
#include <system_error>

#include "codec.h"
#include "frame.h"

/** @brief libavcpp namespace. */
namespace av {
struct __av_packet;
struct __av_frame;

class Packet {
public:
    friend class Frame;
    friend class Format;

    Packet() {}
    Packet( std::shared_ptr< __av_packet > packet ) : packet_( packet ) {}
//    Packet( Format& format_context );

//    AVBufferRef * 	buf
//        A reference to the reference-counted buffer where the packet data is stored. More...

//    int64_t 	pts
//        Presentation timestamp in AVStream->time_base units; the time at which the decompressed packet will be presented to the user. More...

//    int64_t 	dts
//        Decompression timestamp in AVStream->time_base units; the time at which the packet is decompressed. More...

//    uint8_t * 	data

//    int 	size

    int stream_index();

//    int 	flags
//        A combination of AV_PKT_FLAG values. More...

//    AVPacketSideData * 	side_data
//        Additional packet data that can be provided by the container. More...

//    int 	side_data_elems

//    int 	duration
//        Duration of this packet in AVStream->time_base units, 0 if unknown. More...

//    attribute_deprecated void(* 	destruct )(struct AVPacket *)

//    attribute_deprecated void * 	priv

    /** @brief byte position in stream, -1 if unknown */
    int64_t pos();

//    int64_t 	convergence_duration
//        Time difference in AVStream->time_base units from the pts of this packet to the point at which the output from the decoder has converged independent from the availability of previous frames. More...


    /**
     * @brief decode a frame from the assoissiated packet.
     * @param codec the codec to use.
     * @param frame the frame to store the decoded media data.
     * @return
     */
    Packet& decode( Codec& codec, Frame& frame );

    /**
     * @brief checks if an error has occurred.
     * Returns true if an error has occurred on the associated packet.
     * @return true if an error has occurred, false otherwise.
     */
    bool operator!() const;
    /**
     * @brief checks if no error has occurred i.e. I/O operations are available
     * @return
     */
    bool good();
    /**
     * @brief checks if end-of-file has been reached
     * @return
     */
    bool eof();
    /**
     * @brief checks if an error has occurred
     * @return
     */
    bool fail();
    /**
     * @brief return the actual error state of the assiciated packet.
     * @return return the error state as std::error_code.
     */
    std::error_code errc ();

private:
    std::error_code errc_;
    std::shared_ptr< __av_packet > packet_;
};
}//namespace av
#endif // PACKET_H
