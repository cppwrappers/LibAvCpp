#include "resample.h"

#include "__avformat.h"

namespace av {
Resample::Resample( int source_channels, SampleFormat source_sample_fmt, int source_sample_rate,
          int target_channels, SampleFormat target_sample_fmt, int target_sample_rate,
          options_t options ) {

    resample_context_ = std::make_shared< __av_resample_context >(
        source_channels, static_cast< AVSampleFormat >( source_sample_fmt ), source_sample_rate,
        target_channels, static_cast< AVSampleFormat >( target_sample_fmt ), target_sample_rate,
        options );
}
Resample::~Resample() {}
}//namespace av
