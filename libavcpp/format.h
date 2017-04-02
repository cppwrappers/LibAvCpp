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
    enum Mode { READ, WRITE };
    Format();
    ~Format();

    std::error_code open( const std::string& name, Mode mode = READ );
    std::error_code transcode( Stream target_stream );
    std::vector< Stream > streams() const;
    av::Metadata metadata() const ;
    av::Frame get();
    uint64_t playtime() const;

    /**
     * @brief convert playtime in millisecond to hh:mm:ss.milliseconds
     * @param playtime millisecond
     * @return formated string
     */
    static std::string time_to_string( int playtime );

private:
    std::once_flag _register_flag, codec_flag;
    std::shared_ptr< __Format > format_ = nullptr;
};
}//namespace av
#endif // FORMAT_H
