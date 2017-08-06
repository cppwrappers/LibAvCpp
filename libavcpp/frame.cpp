#include "frame.h"

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

#include "averrc.h"
#include "codec.h"

namespace av {

Frame::Frame () {
    if ( ! ( frame_ = av_frame_alloc() ) ) {
        throw make_error_code ( ENOMEM );
    }
}
Frame::Frame ( Codec& codec, const int framesize ) {

    if ( ! ( frame_ = av_frame_alloc() ) )
    { throw make_error_code ( ENOMEM ); }

    frame_->nb_samples     = framesize;
    frame_->channel_layout = codec.channel_layout();
    frame_->format         = codec.sample_fmt();
    frame_->sample_rate    = codec.sample_rate();

    if ( av_frame_get_buffer ( frame_, 0 ) < 0 )
    { throw make_error_code ( ENOMEM ); }
}

Frame::~Frame() {
    if ( frame_ )
    { av_frame_free ( &frame_ ); }
}

uint8_t** Frame::data( int index ) {
    av_frame_make_writable( frame_ );
    return &frame_->data[index];
}
int Frame::format() const
{ return frame_->format; }
int Frame::nb_samples() const
{ return frame_->nb_samples; }
uint32_t Frame::linesize() const
{ return static_cast< uint32_t > ( *frame_->linesize ); }
int64_t Frame::pts()
{ return frame_->pts; }
void Frame::pts ( int64_t _pts )
{ frame_->pts = _pts ; }


uint8_t** Frame::extended_data() const
{ return frame_->extended_data; }
int Frame::nb_extended_buf() const
{ return frame_->nb_extended_buf; }
int64_t Frame::pkt_pos() {
    return frame_->pkt_pos;
}

int Frame::channels() {
    return av_frame_get_channels ( frame_ );
}

}//namespace av
