#include "packet.h"

#include "__avformat.h"
#include "averrc.h"

namespace av {

int Packet::stream_index ()
{ return packet_->packet_->stream_index; }

Packet& Packet::decode( Codec& codec, Frame& frame ) {

    frame.frame_->decode( codec.codec_context_->codec_context_, packet_->packet_ );
//    frame.frame_ = std::make_shared< __av_frame >(
//        codec.codec_context_->codec_context_, packet_->packet_ );

    if( !frame.frame_->data_present )
    { errc_ = make_error_code( AV_EOF ); }
    if( frame.frame_->error < 0 )
    { errc_ = make_error_code( frame.frame_->error ); }

    return *this;
}

int64_t Packet::pos() {
    return packet_->packet_->pos;
}

bool Packet::operator!() const
{ return ( !errc_ ); }
bool Packet::good()
{ return errc_.value() == 0; }
bool Packet::eof()
{ return errc_.value() == AV_EOF; }
bool Packet::fail()
{ return !errc_ && errc_.value() != AV_EOF; }
std::error_code Packet::errc ()
{ return errc_; }

}//namespace av
