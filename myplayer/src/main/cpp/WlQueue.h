//
// Created by Administrator on 2019/12/26.
//

#ifndef FFMPEGPLAYER_WLQUEUE_H
#define FFMPEGPLAYER_WLQUEUE_H


#include <queue>
#include <pthread.h>
#include "WlPlaystatus.h"

extern "C" {
#include <libavcodec/avcodec.h>
};

class WlQueue {

public:


    std::queue<AVPacket *> queuePacket;

    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    WlPlaystatus *playstatus = nullptr;

public:
    WlQueue(WlPlaystatus *playstatus);

    ~WlQueue();

    int putAvPacket(AVPacket *avPacket);

    int getAvPacket(AVPacket *avPacket);

    int getQueueSize();
};


#endif //FFMPEGPLAYER_WLQUEUE_H
