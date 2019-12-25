//
// Created by Administrator on 2019/12/25.
//

#ifndef FFMPEGPLAYER_WLAUDIO_H
#define FFMPEGPLAYER_WLAUDIO_H


extern "C" {
#include <libavcodec/avcodec.h>
};

class WlAudio {
public:
    int streamIndex = -1;//流索引
    AVCodecParameters *codecpar;
    AVCodecContext *avCodecContext = NULL;

public:
    WlAudio();

    ~WlAudio();

};


#endif //FFMPEGPLAYER_WLAUDIO_H
