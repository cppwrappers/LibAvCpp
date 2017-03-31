# libavcpp - C++ Wrapper for ffmpeg library

![ffmpeg logo](https://lh5.googleusercontent.com/-LX53aOM1JLI/AAAAAAAAAAI/AAAAAAAAAGo/cBe-_Z4t4EI/s0-c-k-no-ns/photo.jpg)
libavcpp is a wrapper for the ffmpeg libraries.

## Inlcude in your project:


## Usage:


Open a file and get stream information:

```c++
#include "../libavcpp/format.h"

Format _format;
std::error_code _code = _format.open( " ... filename ... " );
av::Metadata _metadata = _format.metadata();


if( ! _code ) {
    std::vector< av::Stream > _streams = _format.streams();
}
```

## License

LibAvCpp is licensed under the [GNU Lesser General Public License v3.0](https://www.gnu.org/licenses/lgpl-3.0.txt).
