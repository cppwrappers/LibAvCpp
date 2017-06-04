#include "resample.h"

#include "averrc.h"
#include <iostream>

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

namespace av {
Resample::Resample ( int source_channels, SampleFormat source_sample_fmt, int source_sample_rate,
                     int target_channels, SampleFormat target_sample_fmt, int target_sample_rate,
                     options_t options ) {

    //create resample context
    auto _options = Option::make_options ( options );
    resample_context_ = swr_alloc_set_opts ( NULL,
                        av_get_default_channel_layout ( source_channels ),
                        static_cast< AVSampleFormat > ( source_sample_fmt ),
                        source_sample_rate,
                        av_get_default_channel_layout ( target_channels ),
                        static_cast< AVSampleFormat > ( target_sample_fmt ),
                        target_sample_rate,
                        0, _options.get() );

    if ( !resample_context_ )
    { throw make_error_code ( ENOMEM ); }

    //TODO remove
    av_assert0 ( source_sample_rate == target_sample_rate );

    //Open the resampler with the specified parameters.
    int error;

    if ( ( error = swr_init ( resample_context_ ) ) < 0 ) {
        swr_free ( &resample_context_ );
        //TODO do something with error
        { throw make_error_code ( ENOMEM ); }
    }
}

Resample::~Resample() {
    if ( resample_context_ )
    { swr_free ( &resample_context_ ); }

    if ( converted_input_samples ) {
        av_freep ( & ( *converted_input_samples ) [0] );
        free ( *converted_input_samples );
    }
}

int Resample::alloc_buffer ( int channels, int frame_size, SampleFormat sample_fmt ) {
    /* allocate memory for resampling */
//    if( converted_input_samples ) {
//        av_freep(&(*converted_input_samples)[0]);
//        free(*converted_input_samples);
//    }

    //Allocate as many pointers as there are audio channels.
    //Each pointer will later point to the audio samples of the corresponding
    //channels (although it may be NULL for interleaved formats).
    if ( ! ( converted_input_samples = ( uint8_t** ) calloc ( channels, sizeof ( **converted_input_samples ) ) ) )
    { error = ENOMEM; }

    else {
        //Allocate memory for the samples of all channels in one consecutive
        //block for convenience.
        if ( ( error = av_samples_alloc ( converted_input_samples, NULL,
                                          channels,
                                          frame_size,
                                          static_cast< AVSampleFormat > ( sample_fmt ), 0 ) ) < 0 ) {
            av_freep ( & ( *converted_input_samples ) [0] );
            free ( *converted_input_samples );
            converted_input_samples = nullptr;
        }
    }

    return error;
}
}//namespace av
