/*          Copyright Etienne Knecht 2017 - 2019.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef FORMAT_H
#define FORMAT_H

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

#include "_constants.h"
#include "averrc.h"
#include "metadata.h"
#include "option.h"
#include "frame.h"

///@cond DOC_INTERNAL
class AVFormatContext;
///@endcond DOC_INTERNAL

/** @brief libavcpp namespace. */
namespace av {

///@cond DOC_INTERNAL
class IoContext;
class Packet;
struct Codec;

class CodecIterator : public std::iterator<std::input_iterator_tag, int> {
    int* p;
public:
    CodecIterator ( int* x ) :p ( x ) {}
    CodecIterator ( const CodecIterator& mit ) : p ( mit.p ) {}
    CodecIterator& operator++() {++p; return *this;}
    CodecIterator operator++ ( int ) {CodecIterator tmp ( *this ); operator++(); return tmp;}
    bool operator== ( const CodecIterator& rhs ) {return p==rhs.p;}
    bool operator!= ( const CodecIterator& rhs ) {return p!=rhs.p;}
    int& operator*() {return *p;}
};
///@endcond DOC_INTERNAL

/** @brief format representation of a mediafile.

<p>libav follows the model of the ffmpeg libraries. reading and writing of media data to output is done with the av::Format class. av::Format
has accessors for metadata and parameters of the media data. The av:Format class is created with a string of the filename or an std::iostream
object. Optional the read and write mode can be set, default is read. the third optional parameter is the FFMpeg Option av::Option iterator.</p>

open media file, read stream information and metadata:
<pre>
av::Format _format ( "filename", Format::READ ); // filename or std::iostream
if( !_format ) {

    //get audio codec
    for( auto& codec : _format) {
        if( codec.codec_type() == av::CODEC_TYPE::AUDIO )
            //do something with the codec...
    }

    //get audiocodec directly
    auto audio_codec = _format.find_codec( av::CODEC_TYPE::AUDIO );

    //read metadata
    av::Metadata metadata = _format.metadata();
    std::cout << metadata.get( av::Metadata::ALBUM ) << " - " << ;metadata.get( av::Metadata::ARTIST ) << std::endl;

} else //file not opened.
</pre>

<p>the process for decoding is to read the av::Packets from the av:Format. The Packets
hold the encodend data. the av::Packets get decoded to frames, processed (for example
resample audio data) and written to the fifo.</p>
<pre>

</pre>

<p>encoding is the process backwards. Reading from fifo and writing to format,
encoding frames to packages using the encoder.</p>
<pre>

</pre>
*/
class Format {
public:

    /** @brief The i/o mode */
    enum Mode { READ, WRITE };

    /** @brief open mediafile with a file by path */
    Format ( const std::string&             /** @param filename the path to the file to open */
             filename, Mode mode = READ,    /** @param mode     set the format i/o mode to READ or WRITE. */
             Options options = Options()    /** @param options  set the AV option for the format. */
           );
    /** @brief open mediafile with a std::istream */
    Format ( std::iostream& stream,         /** @param filename the media data stream to open */
             Mode mode = READ,              /** @param mode     set the format i/o mode to READ or WRITE. */
             Options options = Options()    /** @param options  set the AV option for the format. */
           );

    Format ( const Format& ) = delete;
    Format& operator= ( const Format& ) = delete;
    Format ( Format&& ) = delete;
    Format& operator= ( Format&& ) = delete;

    /** @brief DTOR */
    ~Format();

    const std::vector< Codec >::iterator begin();
    const std::vector< Codec >::iterator end();
    const std::vector< Codec >::iterator find_codec ( CODEC_TYPE::Enum type );

    /**
     * @brief returns the metadata of the associated format context.
     * @return return metadata of the associated stream.
     */
    av::Metadata metadata() const ;
    /**
     * @brief play length of this media format in milliseconds.
     * @return play length in milliseconds.
     */
    uint64_t playtime() const;

    /** @brief Read packets from the associated format context. */
    Format& read ( Packet& packet /** @param packet Packet to associate the data to. */ );
    /** @brief Read packets filtered by codec. */
    Format& read (
            std::vector< Codec >::iterator codec /** @param codec filter iterator */,
            Packet& packet /** @param packet Packet to associate the data to. */ );
    /** @brief Write packet to output */
    Format& write ( Packet& packet /** @param packet Packet to write. */ );
    /** @brief Decode audio data from packet and store in frame. */
    std::error_code decode (
            Packet& packet,
            Frame& frame );
    /** @brief Encode audio data from frame and store in packet */
    std::error_code encode (
            Codec& codec /** @param codec Encoder to use. */,
            Frame& frame /** @param frame the frame to encode */,
            Packet& packet /** @param package to store the encoded audio datam. */ );

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
    friend struct Codec;
    std::error_code errc_;
    /** Global timestamp for the audio frames */
    int64_t pts = 0;
    std::vector< Codec > codecs_;
    AVFormatContext* format_context_ = nullptr;
    std::shared_ptr < IoContext > io_context_ = nullptr;
    void load_codecs( Options& options );
};
}//namespace av
#endif //FORMAT_H
