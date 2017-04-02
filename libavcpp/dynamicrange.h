#ifndef DYNAMICRANGE_H
#define DYNAMICRANGE_H

#include <system_error>

#define MAX_CHANNELS 32
#define MAX_FRAGMENTS 32768 // more than 24h
// The length of the window over which the RMS and peak are calculated.
// Specified in milliseconds. Don't change this!
#define FRAGMENT_LENGTH 3000

namespace av {
typedef double sample;

struct dr_meter {
    sample *rms_values[MAX_CHANNELS];
    sample *peak_values[MAX_CHANNELS];

    sample sum[MAX_CHANNELS];
    sample peak[MAX_CHANNELS];

    int channels;
    int sample_rate;
    int sample_fmt;
    int sample_size;

    size_t fragment; // The index of the current fragment
    size_t fragment_size; // The size of a fragment in samples
    size_t fragment_read; // The number of samples scanned so far
    bool fragment_started;
};

class DynamicRange {
public:
    DynamicRange();
    void meter_init(struct dr_meter *self);
    std::error_code meter_start(struct dr_meter *self, int channels, int sample_rate, int sample_fmt);

};
}//namespace av
#endif // DYNAMICRANGE_H
