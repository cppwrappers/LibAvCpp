/*          Copyright Etienne Knecht 2017 - 2019.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
extern "C" {
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavcodec/avcodec.h"
#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
# include <stdint.h>
#endif
}

#include "packet.h"

namespace av {

Packet::Packet () : packet_( new AVPacket() ) {
    av_init_packet ( packet_ );
    packet_->data = NULL;
    packet_->size = 0;
}
Packet::~Packet() {
    av_free_packet ( packet_ );
    delete packet_;
}

int64_t Packet::pts()
{ return packet_->pts; }
int64_t Packet::dts()
{ return packet_->dts; }
uint8_t* Packet::data()
{ return packet_->data; }
int Packet::size()
{ return packet_->size; }
size_t Packet::stream_index ()
{ return static_cast< size_t >( packet_->stream_index ); }
int Packet::flags()
{ return packet_->flags; }
int Packet::duration()
{ return packet_->duration; }
int64_t Packet::pos()
{ return packet_->pos; }
int64_t Packet::convergence_duration()
{ return packet_->convergence_duration; }

}//namespace av
