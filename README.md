# libavcpp #
## C++ Wrapper for FFmpeg library

[![Build Status](https://travis-ci.org/cppwrappers/LibAvCpp.svg?branch=master)](https://travis-ci.org/cppwrappers/LibAvCpp)

## Inlcude in your project:


## Usage:


###Open a file and get stream information:

```c++
#include "../libavcpp/format.h"

Format _format;
std::error_code _code = _format.open( " ... filename ... " );
av::Metadata _metadata = _format.metadata();


if( ! _code ) {
    std::vector< av::Stream > _streams = _format.streams();
}
```

### License

LibAvCpp is licensed under the [GNU Lesser General Public License v3.0](https://www.gnu.org/licenses/lgpl-3.0.txt).
