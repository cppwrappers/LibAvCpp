#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include "codec.h"
#include "mediatype.h"
#include "metadata.h"

namespace libav {

/**
 * @brief set/get the media information and metada
 */
struct MediaInfo {
    MediaType::Enum type;
    Codec::Enum codec;
    int bitrate;
    int samplerate;
    int channels;
    int bits_per_sample;
    int width;
    int height;

    friend std::ostream& operator<< ( std::ostream& stream, const MediaInfo& media_info ) {
        stream << media_info.type << ":" << media_info.codec << " (" << media_info.bitrate << " kb/s, " <<
               media_info.samplerate << " hz, " << media_info.channels << " channel(s), " <<
               media_info.bits_per_sample << " bps, " <<
               media_info.width << "x" << media_info.height << " px) " << media_info.playtime;
        return stream;
    }

};
}//namespace libav
#endif // MEDIAINFO_H
