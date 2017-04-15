#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <memory>

#include "_constants.h"
#include "option.h"

/** @brief libavcpp namespace. */
namespace av {
///@cond DOC_INTERNAL
struct __av_resample_context;
///@endcond DOC_INTERNAL

class Resample {
public:
    Resample( int source_channels, SampleFormat source_sample_fmt, int source_sample_rate,
              int target_channels, SampleFormat target_sample_fmt, int target_sample_rate,
              options_t options );
    ~Resample();
private:
    std::shared_ptr< __av_resample_context > resample_context_;
};
typedef std::shared_ptr< Resample > resample_ptr;
}//namespace av
#endif // RESAMPLE_H
