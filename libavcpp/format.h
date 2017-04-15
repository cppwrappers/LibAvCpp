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

/** @brief libavcpp namespace. */
namespace av {

///@cond DOC_INTERNAL
class IoContext;
class Codec;
class Packet;

class CodecIterator : public std::iterator<std::input_iterator_tag, int> {
  int* p;
public:
  CodecIterator(int* x) :p(x) {}
  CodecIterator(const CodecIterator& mit) : p(mit.p) {}
  CodecIterator& operator++() {++p;return *this;}
  CodecIterator operator++(int) {CodecIterator tmp(*this); operator++(); return tmp;}
  bool operator==(const CodecIterator& rhs) {return p==rhs.p;}
  bool operator!=(const CodecIterator& rhs) {return p!=rhs.p;}
  int& operator*() {return *p;}
};
///@endcond DOC_INTERNAL

/** @brief format of mediafile.

<p>Format reads or writes media data using a path on the filesystem or with std::iostream.</p>

<h4>open a file from local filesystem and get the stream codecs of the media format:</h4>

<code>
#include "libavcpp.h"

int main ( int argc, char *argv[] ) {
    av::Format format( FILE|STREAM );
    if( !format ) {
        std::cout << format.errc().message() << std::endl;
        return format.errc().value();
    }

    for( auto& __stream : _format.find() ) {
        if(__stream.codec_type() == CODEC_TYPE::AUDIO ) {
            ... select stream ...
        }
    }
}
</code>

<p>the input is opened at construction time. the state of the associated input can be retrieved with the !operator or with the relative function. the
streams of the format can be accessed with the find method. By passing a CODEC_TYPE to the results will be filtered by this type.</p>

<h4>reading and retrieve the format packets:</h4>

<code>
av::Packet packet;

while( !!format.read( packet ) ) {
    ... packet ...
}
</code>
*/
class Format {
public:

    /** @brief The i/o mode */
    enum Mode { READ, WRITE };

    /** @brief open mediafile with a file by path */
    Format( const std::string&              /** @param filename the path to the file to open */
            filename, Mode mode = READ,     /** @param mode     set the format i/o mode to READ or WRITE. */
            options_t options = options_t() /** @param options  set the AV option for the format. */
    );
    /** @brief open mediafile with a std::istream */
    Format( std::iostream& stream,          /** @param filename the media data stream to open */
            Mode mode = READ,               /** @param mode     set the format i/o mode to READ or WRITE. */
            options_t options = options_t() /** @param options  set the AV option for the format. */
    );

    Format(const Format&) = default;
    Format& operator=(const Format&) = default;
    Format(Format&&) = default;
    Format& operator=(Format&&) = default;

    /** @brief DTOR */
    ~Format();

    std::vector< std::shared_ptr< Codec > >::iterator begin();
    std::vector< std::shared_ptr< Codec > >::iterator end();

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
    /**
     * @brief read a packet from the associated format context.
     * @param packet the packet to associate the data to.
     * @return self for checking state.
     */
    Format& read( Packet& packet );
//    /**
//     * @brief decode a frame from the assoissiated packet.
//     * @param codec the codec to use.
//     * @param frame the frame to store the decoded media data.
//     * @return
//     */
//    Format& decode( Packet& packet, Codec&, Frame& );

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
    friend class Codec;
    std::error_code errc_;
    std::vector< std::shared_ptr< Codec > > codecs_;
    std::shared_ptr< __av_format_context > format_context_ = nullptr;

    void load_codecs();
};
}//namespace av
#endif // FORMAT_H
