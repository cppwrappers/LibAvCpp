# libavcpp - C++ Wrapper for ffmpeg library

![ffmpeg logo](https://lh5.googleusercontent.com/-LX53aOM1JLI/AAAAAAAAAAI/AAAAAAAAAGo/cBe-_Z4t4EI/s0-c-k-no-ns/photo.jpg)
libavcpp is a wrapper for the ffmpeg libraries.

##Inlcude in your project:


##Usage:


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

mstch is licensed under the [MIT license](https://github.com/no1msd/mstch/blob/master/LICENSE).
