#ifndef FORMAT_H
#define FORMAT_H

#include <mutex>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "averrc.h"
#include "codec.h"
#include "codectype.h"
#include "metadata.h"
#include "stream.h"
#include "frame.h"

namespace av {
struct __Format;
class Format {
public:
    Format();
    ~Format();

    std::error_code open( const std::string& name );
    std::vector< Stream > streams();
    libav::Metadata metadata();
    av::Frame get();

private:
    std::once_flag _register_flag, codec_flag;
    std::shared_ptr< __Format > format_ = nullptr;
};
}//namespace av
#endif // FORMAT_H
