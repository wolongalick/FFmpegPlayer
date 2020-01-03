//
// Created by Administrator on 2019/12/25.
//

#ifndef FFMPEGPLAYER_WLAUDIO_H
#define FFMPEGPLAYER_WLAUDIO_H


#include "WlQueue.h"
#include "cxw_log.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
};

class WlAudio {
public:
    int streamIndex = -1;//流索引
    AVCodecParameters *codecpar = nullptr;
    AVCodecContext *avCodecContext = nullptr;
    WlQueue *queue = nullptr;
    WlPlaystatus *playstatus = nullptr;

    pthread_t thread_play;
    AVPacket *avPacket = nullptr;
    AVFrame *avFrame = nullptr;
    int result = -1;
    uint8_t *buffer = nullptr;
    int data_size = 0;

public:
    WlAudio(WlPlaystatus *playstatus);

    ~WlAudio();

    void play();

    int resampleAudio();

    void ff();
};


#endif //FFMPEGPLAYER_WLAUDIO_H
