#ifndef FRAME_H
#define FRAME_H

#include <memory>

namespace av {
///@cond DOC_INTERNAL
struct __av_frame;
///@endcond DOC_INTERNAL
class Frame {
public:
    Frame();
    Frame( std::shared_ptr< __av_frame > frame ) : frame_( frame ) {}
    ~Frame() {}

//    uint8_t* data [AV_NUM_DATA_POINTERS]
//        pointer to the picture/channel planes. More...

    /**
     * @brief For video, size in bytes of each picture line.
     * @return
     */
    uint32_t linesize() const;

    /**
     * @brief pointers to the data planes/channels.
     * @return
     */
    uint8_t ** 	extended_data() const;

//    int 	width
//        width and height of the video frame More...

//    int 	height

    /**
     * @brief number of audio samples (per channel) described by this frame.
     * @return
     */
    int nb_samples() const;

    /**
     * @brief format of the frame, -1 if unknown or unset Values correspond to enum AVPixelFormat for video frames, enum AVSampleFormat for audio)
     * @return
     */
    int format() const;

//    int 	key_frame
//        1 -> keyframe, 0-> not More...

//    enum AVPictureType 	pict_type
//        Picture type of the frame. More...

//    attribute_deprecated uint8_t * 	base [AV_NUM_DATA_POINTERS]

//    AVRational 	sample_aspect_ratio
//        Sample aspect ratio for the video frame, 0/1 if unknown/unspecified. More...

//    int64_t 	pts
//        Presentation timestamp in time_base units (time when frame should be shown to user). More...

//    int64_t 	pkt_pts
//        PTS copied from the AVPacket that was decoded to produce this frame. More...

//    int64_t 	pkt_dts
//        DTS copied from the AVPacket that triggered returning this frame. More...

//    int 	coded_picture_number
//        picture number in bitstream order More...

//    int 	display_picture_number
//        picture number in display order More...

//    int 	quality
//        quality (between 1 (good) and FF_LAMBDA_MAX (bad)) More...

//    attribute_deprecated int 	reference

//    attribute_deprecated int8_t * 	qscale_table
//        QP table. More...

//    attribute_deprecated int 	qstride
//        QP store stride. More...

//    attribute_deprecated int 	qscale_type

//    attribute_deprecated uint8_t * 	mbskip_table
//        mbskip_table[mb]>=1 if MB didn't change stride= mb_width = (width+15)>>4 More...

//    int16_t(*[2] 	motion_val )[2]
//        motion vector table More...

//    attribute_deprecated uint32_t * 	mb_type
//        macroblock type table mb_type_base + mb_width + 2 More...

//    attribute_deprecated short * 	dct_coeff
//        DCT coefficients. More...

//    attribute_deprecated int8_t * 	ref_index [2]
//        motion reference frame index the order in which these are stored can depend on the codec. More...

//    void * 	opaque
//        for some private data of the user More...

//    uint64_t 	error [AV_NUM_DATA_POINTERS]
//        error More...

//    attribute_deprecated int 	type

//    int 	repeat_pict
//        When decoding, this signals how much the picture must be delayed. More...

//    int 	interlaced_frame
//        The content of the picture is interlaced. More...

//    int 	top_field_first
//        If the content is interlaced, is top field displayed first. More...

//    int 	palette_has_changed
//        Tell user application that palette has changed from previous frame. More...

//    attribute_deprecated int 	buffer_hints

//    attribute_deprecated struct
//    AVPanScan * 	pan_scan
//        Pan scan. More...

//    int64_t 	reordered_opaque
//        reordered opaque 64bit (generally an integer or a double precision float PTS but can be anything). More...

//    attribute_deprecated void * 	hwaccel_picture_private

//    attribute_deprecated struct
//    AVCodecContext * 	owner

//    attribute_deprecated void * 	thread_opaque

//    uint8_t 	motion_subsample_log2
//        log2 of the size of the block which a single vector in motion_val represents: (4->16x16, 3->8x8, 2-> 4x4, 1-> 2x2) More...

//    int 	sample_rate
//        Sample rate of the audio data. More...

//    uint64_t 	channel_layout
//        Channel layout of the audio data. More...

//    AVBufferRef * 	buf [AV_NUM_DATA_POINTERS]
//        AVBuffer references backing the data for this frame. More...

//    AVBufferRef ** 	extended_buf
//        For planar audio which requires more than AV_NUM_DATA_POINTERS AVBufferRef pointers, this array will hold all the references which cannot fit into AVFrame.buf. More...

//    int 	nb_extended_buf
//        Number of elements in extended_buf. More...

//    AVFrameSideData ** 	side_data

//    int 	nb_side_data

//    int 	flags
//        Frame flags, a combination of AV_FRAME_FLAGS. More...

//    enum AVColorRange 	color_range
//        MPEG vs JPEG YUV range. More...

//    enum AVColorPrimaries 	color_primaries

//    enum AVColorTransferCharacteristic 	color_trc

//    enum AVColorSpace 	colorspace
//        YUV colorspace type. More...

//    enum AVChromaLocation 	chroma_location

//    int64_t 	best_effort_timestamp
//        frame timestamp estimated using various heuristics, in stream time base Code outside libavcodec should access this field using: av_frame_get_best_effort_timestamp(frame) More...

    /**
     * @brief reordered pos from the last AVPacket that has been input into the decoder Code outside libavcodec should access this field using: av_frame_get_pkt_pos(frame).
     * @return
     */
    int64_t pkt_pos();

//    int64_t 	pkt_duration
//        duration of the corresponding packet, expressed in AVStream->time_base units, 0 if unknown. More...

//    AVDictionary * 	metadata
//        metadata. More...

//    int 	decode_error_flags
//        decode error flags of the frame, set to a combination of FF_DECODE_ERROR_xxx flags if the decoder produced a frame, but there were errors during the decoding. More...

//    int 	channels
//        number of audio channels, only used for audio. More...

//    int 	pkt_size
//        size of the corresponding packet containing the compressed frame. More...

//    AVBufferRef * 	qp_table_buf
//        Not to be accessed directly from outside libavutil. More...

private:
    friend class Packet;
    std::shared_ptr< __av_frame > frame_ = nullptr;
};
typedef std::shared_ptr< Frame > frame_ptr;
}//namespace av
#endif // FRAME_H
