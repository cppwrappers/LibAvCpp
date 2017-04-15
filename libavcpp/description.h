#ifndef DESCRIPTION_H
#define DESCRIPTION_H

/** @brief the av namespace
<h4>FFmpeg wrappers for C++</h4>

all classes are wrappers around the libav c classes and functions.
the wrappres manages the assiciation with the libav class and
handle the memory management using RAII priniples.

read from file:

Format(FILE_NAME) >>Codec(Packet) >>AudioFifo(Resample, Frame)
Audiofifo>> Codec >>Format(Frame)

1) open Format with filename or input stream.
<pre>

</pre>

 */
namespace av {}
#endif // DESCRIPTION_H
