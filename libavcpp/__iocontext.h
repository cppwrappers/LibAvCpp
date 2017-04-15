#ifndef IOCONTEXT_H
#define IOCONTEXT_H

#include <iostream>
#include <string>

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

class IoContext {
public:
    IoContext() {}
    IoContext & operator= ( const IoContext& ) = delete;
    IoContext ( const IoContext& ) = delete;
    ~IoContext();

    std::error_code init_input_format_context ( AVFormatContext *input_context, std::iostream* input_stream );
    std::error_code init_output_format_context ( AVFormatContext *output_context );

    std::string output; //TODO

    std::istream* datafile;
    AVIOContext *av_io_context_;

private:
    const size_t bufferSize = 32 * 1024;
    unsigned char* buffer;
};
}//namespace av
#endif // IOCONTEXT_H
