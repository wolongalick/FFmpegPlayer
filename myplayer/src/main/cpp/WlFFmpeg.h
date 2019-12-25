//
// Created by Administrator on 2019/12/25.
//

#ifndef FFMPEGPLAYER_WLFFMPEG_H
#define FFMPEGPLAYER_WLFFMPEG_H


#include <pthread.h>
#include "WlCallJava.h"
#include "WlAudio.h"

extern "C" {
#include <libavformat/avformat.h>
};

class WlFFmpeg {


public:
    WlCallJava *callJava = NULL;
    const char *url = NULL;
    pthread_t decodeThread;
    AVFormatContext *pFormatCtx = NULL;
    WlAudio *wlAudio;


public:
    WlFFmpeg(WlCallJava *callJava, const char *url);

    ~WlFFmpeg();

    void prepared();

    void decodeFFmpegThread();

};


#endif //FFMPEGPLAYER_WLFFMPEG_H
