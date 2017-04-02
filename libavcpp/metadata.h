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
    enum Enum {
        TITLE, ALBUM, ARTIST, COMPOSER, PERFORMER, COMMENT, YEAR, TRACK, DISC, GENRE, PUBLISHER
    };
    /**
     * @brief set metadata
     * @param name the key (ffmpeg key)
     * @param value the value
     */
    void set ( const char* name, const char* value );
    /**
     * @brief name
     * @param tag get the name for Metadata Enum.
     * @return the ffmpeg key name.
     */
    static std::string name ( Enum tag );
    /**
     * @brief get metadata value.
     * @param key the key
     * @return the value.
     */
    std::string& get ( const Enum& key );
    /**
     * @brief get defined tag metadata tag names.
     * @return vector of defined tag names.
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
