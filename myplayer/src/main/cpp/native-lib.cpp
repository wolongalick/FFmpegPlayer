#include <jni.h>
#include <string>
#include "cxw_log.h"


pthread_t pthread;


void *normalCallback(void *data){
    LOGI("create normal thread from C++");
    pthread_exit(&pthread);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_alick_ffmpegplayer_MainActivity_parseFile(JNIEnv *env, jobject thiz, jstring file_path) {
    std::string hello = "Hello from C++";



    pthread_create(&pthread, NULL,normalCallback, NULL);





    return env->NewStringUTF(hello.c_str());
}