//
// Created by Administrator on 2019/12/24.
//

#ifndef FFMPEGPLAYER_CALLJAVA_H
#define FFMPEGPLAYER_CALLJAVA_H


#include <jni.h>
#include <cwchar>

class CallJava {
public:

    JavaVM *javaVm = NULL;
    JNIEnv *jniEnv = NULL;
    jobject jobj = NULL;

public:
    CallJava(JavaVM *javaVm,JNIEnv *jniEnv,jobject *jobj);

    ~CallJava();

};


#endif //FFMPEGPLAYER_CALLJAVA_H
