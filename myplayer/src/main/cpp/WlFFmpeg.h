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
    WlCallJava *callJava = nullptr;
    const char *url = nullptr;
    pthread_t decodeThread;
    AVFormatContext *pFormatCtx = nullptr;
    WlAudio *wlAudio = nullptr;
    WlPlaystatus *playstatus = nullptr;

public:
    WlFFmpeg(WlPlaystatus *wlPlaystatus,WlCallJava *callJava, const char *url);

    ~WlFFmpeg();

    void prepared();

    void decodeFFmpegThread();

    void start();

};


#endif //FFMPEGPLAYER_WLFFMPEG_H
