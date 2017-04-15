#ifndef AUDIOFIFO_H
#define AUDIOFIFO_H

#include <memory>
#include <system_error>

#include "_constants.h"
#include "codec.h"
#include "frame.h"
#include "resample.h"

namespace av {
///@cond DOC_INTERNAL
struct __av_fifo;
///@endcond DOC_INTERNAL

/** @brief context for an Audio FIFO Buffer.
<ul>
<li>operates at the sample level rather than the byte level.</li>
<li>supports multiple channels with either planar or packed sample format.</li>
<li>automatic reallocation when writing to a full buffer.</li>
</ul>
 */
class AudioFifo {
public:
    /**
     * @brief allocate an AudioFifo buffer.
     */
    AudioFifo(
        SampleFormat sample_fmt /** @param sample_fmt sample format*/,
            int channels        /** @param channels */
    );
    ~AudioFifo() {}

    AudioFifo(const AudioFifo&) = default;
    AudioFifo& operator=(const AudioFifo&) = default;
    AudioFifo(AudioFifo&&) = default;
    AudioFifo& operator=(AudioFifo&&) = default;

    /** @brief resample and write frame to fifo. */
    void write(
        Resample& resample /** @param resample the resample context. */,
        Frame& frame /** @param frame the frame to write */
    );
    /** @brief read and decode from the fifo. */
    void read(
        Codec& codec, /** @param codec the encoder.*/
        Frame& frame  /** @param the frame object to write media data to. */
    );

//TODO    AVFifoBuffer ** 	buf
//        single buffer for interleaved, per-channel buffers for planar

    /** @brief nb_buffers number of buffers. */
    int nb_buffers();
    /** @brief number of samples currently in the FIFO. */
    int nb_samples();
    /** @brief current allocated size, in samples. */
    int allocated_samples();
    /** @brief number of channels. */
    int channels();
    /** @brief sample format. */
    SampleFormat sample_fmt();
    /** @brief size, in bytes, of one sample in a buffer. */
    int sample_size();

private:
    std::shared_ptr< __av_fifo > fifo_ = nullptr;
};
}//namespace av
#endif // AUDIOFIFO_H
