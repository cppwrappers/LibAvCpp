#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <string>
#include <iostream>
#include <vector>

namespace av {

/**
 * @brief The Metadata struct
 *
 * <p>Media tag description and comparison table from http://age.hobba.nl/audio/tag_frame_reference.html</p>
 *
 * @htmlinclude tag_frame_reference.html
 */
struct Metadata {
public:
    /** @brief enum with metadata elements for read and write access. */
    enum Enum {
        TITLE, ALBUM, ARTIST, COMPOSER, PERFORMER, COMMENT, YEAR, TRACK, DISC, GENRE, PUBLISHER
    };
    /**
     * @brief set metadata with value by key.
     * @param name the metadata key
     * @param value the value for the metadata
     */
    void set ( const char* name, const char* value );
    /**
     * @brief get the metadata key type as string.
     * @param tag the key to get as string.
     * @return the ffmpeg key as string.
     */
    static std::string name ( Enum tag );
    /**
     * @brief get the metadata value by key.
     * @param key the key for the metadata element.
     * @return the value referenced by key.
     */
    std::string& get ( const Enum& key );
    /**
     * @brief get keys of valid metadata elments.
     * @return the keys of the defined metadata elements
     */
    std::vector< Enum > tag_names ();

    friend std::ostream& operator<< ( std::ostream& stream, const Metadata& meta_data ) {
        for ( auto& __tag : meta_data.tags ) {
            stream << name ( __tag.first ) << ": \t\t" << __tag.second << "\n";
        }
        return stream;
    }
private:
    std::map< Enum, std::string > tags;
    std::vector < std::string > skip_list { { "LC", "ITUNESCOMPILATION", "TAGGING_TIME" } };
};
}//namespace libav
#endif // METADATA_H
