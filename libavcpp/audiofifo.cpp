#include "audiofifo.h"

//extern "C" {
//#include "libavformat/avformat.h"
//#include "libavformat/avio.h"
//#include "libavcodec/avcodec.h"
//#include "libavutil/audio_fifo.h"
//#include "libavutil/avassert.h"
//#include "libavutil/avstring.h"
//#include "libavutil/frame.h"
//#include "libavutil/opt.h"
//#include "libswresample/swresample.h"
//#ifdef __cplusplus
//#define __STDC_CONSTANT_MACROS
//#ifdef _STDINT_H
//#undef _STDINT_H
//#endif
//# include <stdint.h>
//#endif
//}

#include "__avformat.h"

namespace av {
AudioFifo::AudioFifo( SampleFormat sample_fmt, int channels ) {
    fifo_ = std::make_shared< __av_fifo >( static_cast< AVSampleFormat >( sample_fmt ), channels );
}

void AudioFifo::write( Resample& resample, Frame& frame ) {
//    fifo_->write( xxx );
}
void read( Codec& codec, Frame& frame ) {
//    const int frame_size = FFMIN( av_audio_fifo_size( fifo ), output_codec_context->frame_size );
//    int data_written;
}

int AudioFifo::nb_buffers()
{ /*return fifo_->fifo_->nb_buffers;*/ }
int AudioFifo::nb_samples()
{ /* return ( fifo_->fifo_ ); */ }
int AudioFifo::allocated_samples()
{}
int channels()
{}
SampleFormat AudioFifo::sample_fmt()
{}
int AudioFifo::sample_size()
{}

}//namespace av
