//
// Created by Administrator on 2019/12/16.
//
#include <android/log.h>

#ifndef FFMPEGPLAYER_CXW_LOG_H
#define FFMPEGPLAYER_CXW_LOG_H


#define LOG_TAG "cxwFFmpeg"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);

#endif //FFMPEGPLAYER_CXW_LOG_H
