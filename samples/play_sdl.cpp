#include <iostream>

#include <SDL.h>
#include <SDL_thread.h>

#include "../libavcpp/format.h"
#include "../libavcpp/stream.h"
#include "../libavcpp/codec.h"
#include "../libavcpp/codectype.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavformat/avio.h"

#include "libavcodec/avcodec.h"

#include "libavutil/audio_fifo.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/frame.h"
#include "libavutil/opt.h"
#include "libswresample/swresample.h"
#include <libswscale/swscale.h>

#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
# include <stdint.h>
#endif
}

int main( int argc, char* argv[] ) {

    int frameFinished;

    av::Format _format;
    _format.open( argv[1] );

    av::Stream video_stream;
    for( auto& stream : _format.streams() ) {
        if( stream.codec_type == av::CodecType::VIDEO ) {
            video_stream = stream;
        }
    }

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
      fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
      exit(1);
    }

    SDL_Surface *screen;
    screen = SDL_SetVideoMode(video_stream.width, video_stream.height, 0, 0);
    if(!screen) {
      fprintf(stderr, "SDL: could not set video mode - exiting\n");
      exit(1);
    }

    SDL_Overlay     *bmp = NULL;
    bmp = SDL_CreateYUVOverlay(video_stream.width, video_stream.height,
                               SDL_YV12_OVERLAY, screen);

    // initialize SWS context for software scaling
    struct SwsContext *sws_ctx = NULL;
    sws_ctx = sws_getContext(video_stream.width,
                             video_stream.height,
                 static_cast< AVPixelFormat >( video_stream.pixel_format ),
                 video_stream.width,
                 video_stream.height,
                 PIX_FMT_YUV420P,
                 SWS_BILINEAR,
                 NULL,
                 NULL,
                 NULL
                 );


    if(frameFinished) {
        SDL_LockYUVOverlay(bmp);

        AVPicture pict;
        pict.data[0] = bmp->pixels[0];
        pict.data[1] = bmp->pixels[2];
        pict.data[2] = bmp->pixels[1];

        pict.linesize[0] = bmp->pitches[0];
        pict.linesize[1] = bmp->pitches[2];
        pict.linesize[2] = bmp->pitches[1];

        // Convert the image into YUV format that SDL uses
//        sws_scale(sws_ctx, (uint8_t const * const *)pFrame->data,
//              pFrame->linesize, 0, video_stream.height,
//              pict.data, pict.linesize);

//        SDL_UnlockYUVOverlay(bmp);
      }


    return 0;
}
