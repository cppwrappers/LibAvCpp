#include "audiofifo.h"

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

namespace av {
AudioFifo::AudioFifo ( const SampleFormat& sample_fmt, int channels ) : sample_format_ ( sample_fmt ) {
    /** Create the FIFO buffer based on the specified output sample format. */
    if ( ! ( fifo_ = av_audio_fifo_alloc ( static_cast< AVSampleFormat > ( sample_fmt ), channels, 1 ) ) ) {
        error_ = make_error_code ( ENOMEM );
    }
}
AudioFifo::~AudioFifo() {
    if ( fifo_ )
    { av_audio_fifo_free ( fifo_ ); }
}

AudioFifo& AudioFifo::write ( Resample& resample, Frame& frame ) {

    int _error;
    //Initialize the temporary storage for the converted input samples.
    uint8_t* converted_input_samples = nullptr;

    if ( ! ( converted_input_samples = static_cast< uint8_t* > ( calloc ( frame.channels(), sizeof ( *converted_input_samples ) ) ) ) )
    { error_ = make_error_code ( ENOMEM ); return *this; }

    else {
        //Allocate memory for the samples of all channels in one consecutive
        //block for convenience.
        if ( ( _error = av_samples_alloc ( &converted_input_samples, NULL,
                                           frame.channels(),
                                           frame.nb_samples(),
                                           static_cast< AVSampleFormat > ( sample_format_ ), 0 ) ) < 0 ) {
            av_freep ( & ( converted_input_samples ) [0] );
            free ( converted_input_samples );
            converted_input_samples = nullptr;
            error_ = make_error_code ( _error );
            return *this;
        }
    }

    //Convert the input samples to the desired output sample format.
    //This requires a temporary storage provided by converted_input_samples.
    if ( ( _error = swr_convert ( resample.resample_context_,
                                  &converted_input_samples,
                                  frame.nb_samples(),
                                  ( const uint8_t** ) frame.nb_extended_buf(), frame.nb_samples() ) ) < 0 ) {
        error_ = make_error_code ( _error );
        return *this;
    }

    //Make the FIFO as large as it needs to be to hold both, the old and the new samples.
    if ( ( _error = av_audio_fifo_realloc ( fifo_, av_audio_fifo_size ( fifo_ ) + frame.nb_samples() ) ) < 0 )
    { error_ = make_error_code ( _error ); return *this; }

    //Store the new samples in the FIFO buffer. */
    auto _nb_samples = frame.nb_samples();

    _error = av_audio_fifo_write ( fifo_, ( void ** ) &converted_input_samples, _nb_samples );

    if ( _error < _nb_samples )
    { error_ = make_error_code ( _error ); return *this; }

    return *this;
}
AudioFifo& AudioFifo::read ( Frame& frame, const int frame_size ) {

//    const int frame_size = FFMIN(av_audio_fifo_size( fifo_ ), codec.frame_size() );
//    Frame output_frame( codec, frame_size );

    if ( av_audio_fifo_read ( fifo_, ( void ** ) frame.frame_->data, frame_size ) < frame_size )
    { error_ = make_error_code ( AVERROR_EXIT ); }

    return *this;
}
int AudioFifo::nb_buffers()
{ /*return fifo_->nb_buffers;*/ }
int AudioFifo::nb_samples()
{ /*return ( fifo_->nb_samples );*/ }
int AudioFifo::allocated_samples()
{}
int channels()
{}
SampleFormat AudioFifo::sample_fmt()
{}
int AudioFifo::sample_size()
{}
int AudioFifo::audio_fifo_size()
{ return av_audio_fifo_size ( fifo_ ); }

bool AudioFifo::operator!() const
{ return !error_; }
bool AudioFifo::good()
{ return !!error_; }
bool AudioFifo::eof()
{ return error_ == AV_EOF; }
bool AudioFifo::fail()
{ return !error_ && error_ != AV_EOF; }
std::error_code AudioFifo::errc ()
{ return error_; }

}//namespace av
