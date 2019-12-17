#include <jni.h>
#include <string>
#include <android/log.h>
#include <queue>


extern "C" {
#include <libavformat/avformat.h>
#include "cxw_str.h"
}

#define LOG_TAG "cxwFFmpeg"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__);


static double r2d(AVRational avRational) {
    LOGI("num:%d,den:%d", avRational.num, avRational.den);
    return avRational.num == 0 || avRational.den == 0 ? 0 : (double) avRational.num /
                                                            (double) avRational.den;
}

/**
 * 获取当前毫秒
 * @return
 */
long long getNowMs() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    LOGI("tv_sec:%ld,tv_usec:%ld",tv.tv_sec,tv.tv_usec);
    int sec = tv.tv_sec;//100小时
    long long t = sec * 1000 + tv.tv_usec / 1000;
    return t;
}




