#include <jni.h>
#include <string>
#include <queue>
#include <unistd.h>
#include "cxw_log.h"
#include "WlCallJava.h"
#include "WlFFmpeg.h"

//======================学习多线程同步-begin===========================
pthread_t pthread;
pthread_t produc;
pthread_t custom;
pthread_mutex_t mutex;
pthread_cond_t cond;

std::queue<int> queue;

void *normalCallback(void *data) {
    LOGI("create normal thread from C++");
    pthread_exit(&pthread);
}

void *producCallBack(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        queue.push(1);
        LOGI("生产者生产一个产品,通知消费者消费,产品数量为 %d", queue.size());
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(5);//睡眠5秒
    }

    pthread_exit(&produc);

}

void *customCallBack(void *data) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (queue.size() > 0) {
            queue.pop();
            LOGI("消费者消费产品,产品数量还剩余 %d", queue.size());
        } else {
            LOGI("没有产品可以消费,等待中...");
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        usleep(1000 * 500);//睡眠500毫秒
    }
    pthread_exit(&custom);
}
//======================学习多线程同步-end============================

JavaVM *javaVm = nullptr;
WlCallJava *callJava = nullptr;
WlFFmpeg *fFmpeg = nullptr;
WlPlaystatus *playstatus = nullptr;


extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    javaVm = vm;

    JNIEnv *env;

    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_4) != JNI_OK) {

        return result;
    }
    return JNI_VERSION_1_4;
}
}



extern "C"
JNIEXPORT jstring JNICALL
Java_com_alick_ffmpegplayer_PlayMP4Activity_parseFile(JNIEnv *env, jobject thiz,
                                                      jstring file_path) {
    std::string hello = "Hello from C++";
//    pthread_create(&pthread, NULL,normalCallback, nullptr);


    for (int i = 0; i < 10; ++i) {
        queue.push(i);
    }

    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);

    pthread_create(&produc, nullptr, producCallBack, nullptr);
    pthread_create(&custom, nullptr, customCallBack, nullptr);


    return env->NewStringUTF(hello.c_str());
}

/**
 * 准备播放
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_alick_myplayer_WlPlayer_n_1prepare(JNIEnv *env, jobject thiz, jstring source_) {
    const char *source = env->GetStringUTFChars(source_, 0);

    LOGI("url路径:%s", source);


    if (fFmpeg == NULL) {
        if (callJava == NULL) {
            callJava = new WlCallJava(javaVm, env, &thiz);
        }

        playstatus=new WlPlaystatus();

        fFmpeg = new WlFFmpeg(playstatus,callJava, source);
    }
    fFmpeg->prepared();

//    env->ReleaseStringUTFChars(source_,source);

}

/**
 * 开始播放
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_alick_myplayer_WlPlayer_n_1start(JNIEnv *env, jobject thiz) {
    if (fFmpeg == NULL) {
        LOGE("fFmpeg为null");
        return;
    }

    fFmpeg->start();

}