#ifndef AUDIOFIFO_H
#define AUDIOFIFO_H

#include <memory>
#include <system_error>

#include "_constants.h"
#include "codec.h"
#include "frame.h"
#include "resample.h"

///@cond DOC_INTERNAL
extern "C" {
    typedef class AVAudioFifo;
}
///@endcond DOC_INTERNAL

namespace av {

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
        const SampleFormat& sample_fmt  /** @param sample_fmt sample format*/,
        const int channels              /** @param channels */
    );
    ~AudioFifo();

    AudioFifo(const AudioFifo&) = delete;
    AudioFifo& operator=(const AudioFifo&) = delete;
    AudioFifo(AudioFifo&&) = delete;
    AudioFifo& operator=(AudioFifo&&) = delete;

    /** @brief resample and write frame to fifo. */
    AudioFifo& write(
        Resample& resample /** @param resample the resample context. */,
        Frame& frame /** @param frame the frame to write */
    );
    /** @brief read from the fifo. */
    AudioFifo& read( Codec& codec, std::function< void( Frame& frame ) > fnc );

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

    /**
     * @brief checks if an error has occurred.
     * Returns true if an error has occurred on the associated format context.
     * @return true if an error has occurred, false otherwise.
     */
    bool operator!() const;
    /**
     * @brief checks if no error has occurred i.e. I/O operations are available
     * @return
     */
    bool good();
    /**
     * @brief checks if end-of-file has been reached
     * @return
     */
    bool eof();
    /**
     * @brief checks if an error has occurred
     * @return
     */
    bool fail();
    /**
     * @brief return the actual error state of the assiciated format context.
     * @return return the error state as std::error_code.
     */
    std::error_code errc ();

private:
    AVAudioFifo* fifo_ = nullptr;
    const SampleFormat& sample_format_;
    std::error_code error_;
};
}//namespace av
#endif // AUDIOFIFO_H
