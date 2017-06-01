/*          Copyright Etienne Knecht 2017 - 2019.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef PACKET_H
#define PACKET_H

#include <memory>

///@cond DOC_INTERNAL
extern "C" {
struct AVPacket;
}
///@endcond DOC_INTERNAL

/** \brief av namespace. */
namespace av {

/**
@brief This structure stores compressed data.
<p>It is typically exported by demuxers and then passed as input to decoders, or received as output from encoders and then passed to muxers.<br/>
For video, it should typically contain one compressed frame. For audio it may contain several compressed frames.<br/></p>

<p><strong>AVPacket is one of the few structs in FFmpeg, whose size is a part of public ABI. Thus it may be allocated on stack and no new fields
can be added to it without libavcodec and libavformat major bump.<br/>
The semantics of data ownership depends on the buf or destruct (deprecated) fields. If either is set, the packet data is dynamically allocated
and is valid indefinitely until av_free_packet() is called (which in turn calls av_buffer_unref()/the destruct callback to free the data).
If neither is set, the packet data is typically backed by some static buffer somewhere and is only valid for a limited time (e.g. until the
next read call when demuxing).<br/>
The side data is always allocated with av_malloc() and is freed in av_free_packet().</strong></p>
 */
class Packet {
public:

    Packet();
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;
    Packet(Packet&&) = delete;
    Packet& operator=(Packet&&) = delete;
    ~Packet();

    /**
     \brief Presentation timestamp in AVStream->time_base units; the time at which the decompressed packet will be presented to the user.
     <p>Can be AV_NOPTS_VALUE if it is not stored in the file. pts MUST be larger or equal to dts as presentation cannot happen before
        decompression, unless one wants to view hex dumps. Some formats misuse the terms dts and pts/cts to mean something different.
        Such timestamps must be converted to true pts/dts before they are stored in AVPacket.</p>
     */
    int64_t pts();

    /**
     \brief Decompression timestamp in AVStream->time_base units; the time at which the packet is decompressed.
     <p>Can be AV_NOPTS_VALUE if it is not stored in the file.</p>
     */
    int64_t dts();

    /** \brief Frame raw data. */
    uint8_t* data();

    /** \brief Frame raw data size. */
    int size();

    //TODO get codec directly
    /** \brief the stream index for this packt. */
    size_t stream_index();

    /** \brief A combination of AV_PKT_FLAG values. */
    int flags();

    /**
     \brief Additional packet data that can be provided by the container.
     <p>Packet can contain several types of side information.</p>
     */
//TODO    AVPacketSideData * 	side_data


    /** \brief side data elemnt count. */
//TODO    int 	side_data_elems

    /**
     \brief Duration of this packet in AVStream->time_base units, 0 if unknown.
     <p>Equals next_pts - this_pts in presentation order.</p>
    */
    int duration();

    /** \brief byte position in stream, -1 if unknown */
    int64_t pos();

    /**
     \brief Time difference in AVStream->time_base units from the pts of this packet to the point at which the output from the decoder has converged independent from the availability of previous frames.
     <p>That is, the frames are virtually identical no matter if decoding started from the very first frame or from this keyframe. Is AV_NOPTS_VALUE if unknown.
        This field is not the display duration of the current packet. This field has no meaning if the packet does not have AV_PKT_FLAG_KEY set.<br/>
        The purpose of this field is to allow seeking in streams that have no keyframes in the conventional sense. It corresponds to the recovery point SEI
        in H.264 and match_time_delta in NUT. It is also essential for some types of subtitle streams to ensure that all subtitles are correctly displayed after seeking.</p>
     * @return
     */
    int64_t convergence_duration();

private:
    friend class Format;
    AVPacket* packet_;
};
}//namespace av
#endif // PACKET_H
