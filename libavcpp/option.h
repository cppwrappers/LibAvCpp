#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <vector>

#include <memory>

///@cond DOC_INTERNAL
extern "C" {
struct AVDictionary;
}
///@endcond DOC_INTERNAL

/** @brief libavcpp namespace. */
namespace av {
class Option {
public:
    Option();
    enum TYPE {STRING, INT };
    Option( const std::string& key, const std::string& val ) : type_(STRING), key_(key), str_val_(val) {}
    Option( const std::string& key, const int& val ) : type_(INT), key_(key), int_val_(val) {}

    TYPE type() const
    {return type_;}

    const char* key() const
    {return key_.c_str();}

    const char* c_str() const
    {return str_val_.c_str();}

    int c_int() const
    {return int_val_;}

private:
    TYPE type_;
    std::string key_, str_val_;
    int int_val_;

    friend struct Codec;
    friend struct Resample;
    static std::shared_ptr< AVDictionary > make_options( const std::vector< Option >& options );
};
typedef std::vector< Option > options_t;
}//namespace av
#endif // OPTION_H
