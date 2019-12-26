//
// Created by Administrator on 2019/12/24.
//

#ifndef FFMPEGPLAYER_WLCALLJAVA_H
#define FFMPEGPLAYER_WLCALLJAVA_H


#include <jni.h>
#include <cwchar>
#include "cxw_log.h"

#define MAIN_THREAD 0
#define CHILD_THREAD 1


class WlCallJava {
public:

    JavaVM *javaVm = nullptr;
    JNIEnv *jniEnv = nullptr;
    jobject jobj = nullptr;

    jmethodID jmid_prepared;

public:
    WlCallJava(JavaVM *javaVm, JNIEnv *jniEnv, jobject *jobj);

    ~WlCallJava();

    void onCallPrepared(int type);

};


#endif //FFMPEGPLAYER_WLCALLJAVA_H
