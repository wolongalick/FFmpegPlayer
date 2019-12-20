#include <jni.h>
#include <string>
#include <queue>
#include <unistd.h>
#include "cxw_log.h"


pthread_t pthread;
pthread_t produc;
pthread_t custom;
pthread_mutex_t mutex;
pthread_cond_t cond;

std::queue<int> queue;

bool exit= false;

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


extern "C"
JNIEXPORT jstring JNICALL
Java_com_alick_ffmpegplayer_MainActivity_parseFile(JNIEnv *env, jobject thiz, jstring file_path) {
    std::string hello = "Hello from C++";
//    pthread_create(&pthread, NULL,normalCallback, NULL);


    for (int i = 0; i < 10; ++i) {
        queue.push(i);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&produc, NULL, producCallBack, NULL);
    pthread_create(&custom, NULL, customCallBack, NULL);


    return env->NewStringUTF(hello.c_str());
}