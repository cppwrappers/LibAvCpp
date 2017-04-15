#include "frame.h"

#include "__avformat.h"

namespace av {
Frame::Frame () {
    frame_ = std::make_shared<__av_frame >();
}
int Frame::format() const {
    return frame_->frame_->format;
}
int Frame::nb_samples() const {
    return frame_->frame_->nb_samples;
}
uint32_t Frame::linesize() const {
    return static_cast< uint32_t >( *frame_->frame_->linesize );
}
uint8_t** Frame::extended_data() const {
    return frame_->frame_->extended_data;
}
int64_t Frame::pkt_pos() {
    return frame_->frame_->pkt_pos;
}

}//namespace av
