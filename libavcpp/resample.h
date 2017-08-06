#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <memory>

#include "_constants.h"
#include "option.h"

///@cond DOC_INTERNAL
extern "C" {
    struct SwrContext;
}
///@endcond DOC_INTERNAL

/** @brief libavcpp namespace. */
namespace av {
struct Resample {
public:
    Resample ( int source_channels, SampleFormat source_sample_fmt, int source_sample_rate,
               int target_channels, SampleFormat target_sample_fmt, int target_sample_rate,
               Options options );
    ~Resample();

    Resample ( const Resample& ) = delete;
    Resample& operator= ( const Resample& ) = delete;
    Resample ( Resample&& ) = default;
    Resample& operator= ( Resample&& ) = default;

    int alloc_buffer ( int channels, int frame_size, SampleFormat sample_fmt );

private:
    friend class AudioFifo;
    int error;
    SwrContext* resample_context_;
    uint8_t **converted_input_samples = nullptr;
};
}//namespace av
#endif //RESAMPLE_H
