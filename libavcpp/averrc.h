#ifndef AVERRC_H
#define AVERRC_H

#include <system_error>

namespace libav {

//Taken from ffmpeg to save include.
#define AV_MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define AV_FFERRTAG(a, b, c, d) (-(int)AV_MKTAG(a, b, c, d))

enum av_errc {
    UNKNOWN,
    AV_BSF_NOT_FOUND       = AV_FFERRTAG ( 0xF8,'B','S','F' ), ///< Bitstream filter not found
    AV_BUG                 = AV_FFERRTAG ( 'B','U','G','!' ), ///< Internal bug, also see AVERROR_BUG2
    AV_BUFFER_TOO_SMALL    = AV_FFERRTAG ( 'B','U','F','S' ), ///< Buffer too small
    AV_DECODER_NOT_FOUND   = AV_FFERRTAG ( 0xF8,'D','E','C' ), ///< Decoder not found
    AV_DEMUXER_NOT_FOUND   = AV_FFERRTAG ( 0xF8,'D','E','M' ), ///< Demuxer not found
    AV_ENCODER_NOT_FOUND   = AV_FFERRTAG ( 0xF8,'E','N','C' ), ///< Encoder not found
    AV_EOF                 = AV_FFERRTAG ( 'E','O','F',' ' ), ///< End of file
    AV_EXIT                = AV_FFERRTAG ( 'E','X','I','T' ), ///< Immediate exit was requested; the called function should not be restarted
    AV_EXTERNAL            = AV_FFERRTAG ( 'E','X','T',' ' ), ///< Generic error in an external library
    AV_FILTER_NOT_FOUND    = AV_FFERRTAG ( 0xF8,'F','I','L' ), ///< Filter not found
    AV_INVALIDDATA         = AV_FFERRTAG ( 'I','N','D','A' ), ///< Invalid data found when processing input
    AV_MUXER_NOT_FOUND     = AV_FFERRTAG ( 0xF8,'M','U','X' ), ///< Muxer not found
    AV_OPTION_NOT_FOUND    = AV_FFERRTAG ( 0xF8,'O','P','T' ), ///< Option not found
    AV_PATCHWELCOME        = AV_FFERRTAG ( 'P','A','W','E' ), ///< Not yet implemented in FFmpeg, patches welcome
    AV_PROTOCOL_NOT_FOUND  = AV_FFERRTAG ( 0xF8,'P','R','O' ), ///< Protocol not found
    AV_STREAM_NOT_FOUND    = AV_FFERRTAG ( 0xF8,'S','T','R' ), ///< Stream not found
    AV_BUG2                = AV_FFERRTAG ( 'B','U','G',' ' ),
    AV_UNKNOWN             = AV_FFERRTAG ( 'U','N','K','N' ), ///< Unknown error, typically from an external library
    AV_EXPERIMENTAL        = ( -0x2bb2afa8 ), ///< Requested feature is flagged experimental. Set strict_std_compliance if you really want to use it.
    AV_INPUT_CHANGED       = ( -0x636e6701 ), ///< Input changed between calls. Reconfiguration is required. (can be OR-ed with AVERROR_OUTPUT_CHANGED)
    AV_OUTPUT_CHANGED      = ( -0x636e6702 ), ///< Output changed between calls. Reconfiguration is required. (can be OR-ed with AVERROR_INPUT_CHANGED)
    /* HTTP & RTSP errors */
    AV_HTTP_BAD_REQUEST    = AV_FFERRTAG ( 0xF8,'4','0','0' ),
    AV_HTTP_UNAUTHORIZED   = AV_FFERRTAG ( 0xF8,'4','0','1' ),
    AV_HTTP_FORBIDDEN      = AV_FFERRTAG ( 0xF8,'4','0','3' ),
    AV_HTTP_NOT_FOUND      = AV_FFERRTAG ( 0xF8,'4','0','4' ),
    AV_HTTP_OTHER_4XX      = AV_FFERRTAG ( 0xF8,'4','X','X' ),
    AV_HTTP_SERVER_ERROR   = AV_FFERRTAG ( 0xF8,'5','X','X' )
};
}//namespace libav

namespace std {
template<> struct is_error_condition_enum<libav::av_errc> : public true_type {};
}//namespace std

namespace libav {
class av_category_t : public std::error_category {
public:
    virtual const char* name() const noexcept;
    virtual std::error_condition default_error_condition ( int ev ) const noexcept;
    virtual bool equivalent ( const std::error_code& code, int condition ) const noexcept;
    virtual std::string message ( int ev ) const;
} static av_category;

inline std::error_condition make_error_condition ( av_errc e ) {
    return std::error_condition ( static_cast<int> ( e ), av_category );
}
inline std::error_code make_error_code ( int error ) {
    switch ( error ) {
    case UNKNOWN:
    case AV_BSF_NOT_FOUND:
    case AV_BUG:
    case AV_BUFFER_TOO_SMALL:
    case AV_DECODER_NOT_FOUND:
    case AV_DEMUXER_NOT_FOUND:
    case AV_ENCODER_NOT_FOUND:
    case AV_EOF:
    case AV_EXIT:
    case AV_EXTERNAL:
    case AV_FILTER_NOT_FOUND:
    case AV_INVALIDDATA:
    case AV_MUXER_NOT_FOUND:
    case AV_OPTION_NOT_FOUND:
    case AV_PATCHWELCOME:
    case AV_PROTOCOL_NOT_FOUND:
    case AV_STREAM_NOT_FOUND:
    case AV_BUG2:
    case AV_UNKNOWN:
    case AV_EXPERIMENTAL:
    case AV_INPUT_CHANGED:
    case AV_OUTPUT_CHANGED:

    /* HTTP & RTSP errors */
    case AV_HTTP_BAD_REQUEST:
    case AV_HTTP_UNAUTHORIZED:
    case AV_HTTP_FORBIDDEN:
    case AV_HTTP_NOT_FOUND:
    case AV_HTTP_OTHER_4XX:
    case AV_HTTP_SERVER_ERROR:
        return std::error_code ( error, av_category );
    }

    return std::error_code ( abs ( error ), std::generic_category() );
}
}//namespace libav
#endif // AVERRC_H
