//
// Created by Administrator on 2019/12/25.
//

#ifndef FFMPEGPLAYER_WLAUDIO_H
#define FFMPEGPLAYER_WLAUDIO_H


#include "WlQueue.h"

extern "C" {
#include <libavcodec/avcodec.h>
};

class WlAudio {
public:
    int streamIndex = -1;//流索引
    AVCodecParameters *codecpar = nullptr;
    AVCodecContext *avCodecContext = nullptr;
    WlQueue *queue = nullptr;
    WlPlaystatus *playstatus = nullptr;

public:
    WlAudio(WlPlaystatus *playstatus);

    ~WlAudio();

};


#endif //FFMPEGPLAYER_WLAUDIO_H
