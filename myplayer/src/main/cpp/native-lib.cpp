#include <jni.h>
#include <string>
#include <queue>
#include <unistd.h>
#include "cxw_log.h"
#include "WlCallJava.h"
#include "WlFFmpeg.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

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

SLObjectItf engineObjectItf = nullptr;
SLEngineItf engineEngine = nullptr;
SLObjectItf outputMixObject = nullptr;
SLEnvironmentalReverbItf outputMixEnvironmentalReverb = nullptr;
SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
SLObjectItf pcmPlayerObject = nullptr;
SLPlayItf pcmPlayerPlay = nullptr;
SLAndroidSimpleBufferQueueItf pcmBufferQueue = nullptr;

FILE *pcmFile = nullptr;
void *buffer = nullptr;
uint8_t *out_buffer = nullptr;


SLuint32 getPcmData(void **pcm) {
    SLuint32 size = 0;
    while (!feof(pcmFile)) {
        size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        if (out_buffer == nullptr) {
            LOGE("读取结束");
            break;
        } else {
            LOGI("读取中...");
        }
        *pcm = out_buffer;
        break;
    }

    return size;

}

void pcmBufferCallback(SLAndroidSimpleBufferQueueItf buf, void *context) {
    SLuint32 size = getPcmData(&buffer);
    if (buffer != nullptr) {
        (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, size);
//        (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, 44100 * 2 * 2);
    }

}


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

        playstatus = new WlPlaystatus();

        fFmpeg = new WlFFmpeg(playstatus, callJava, source);
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

/**
 * 用OpenSL_ES播放
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_alick_ffmpegplayer_OpenSLESActivity_ByOpenSL_1ES(JNIEnv *env, jobject thiz, jstring url_) {
    // TODO: implement ByOpenSL_ES()

    const char *url = env->GetStringUTFChars(url_, nullptr);

    pcmFile = fopen(url, "r");

    if (pcmFile == nullptr) {
        return;
    }

    LOGI("打开文件成功:%s", url);

    out_buffer = static_cast<uint8_t *>(malloc(44100 * 2 * 2));

    SLresult result;
    //1.创建引擎对象
    LOGI("创建引擎对象");
    slCreateEngine(&engineObjectItf, 0, nullptr, 0, nullptr, nullptr);
    (*engineObjectItf)->Realize(engineObjectItf, SL_BOOLEAN_FALSE);
    (*engineObjectItf)->GetInterface(engineObjectItf, SL_IID_ENGINE, &engineEngine);

    //2.创建混音器
    LOGI("创建混音器");
    SLuint32 numInterfaces = 1;//混音器个数
    const SLInterfaceID mixIds[1] = {SL_IID_ENVIRONMENTALREVERB};//混音器id数组
    const SLboolean mixReq[1] = {SL_BOOLEAN_FALSE};//

    (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, numInterfaces, mixIds, mixReq);
    (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                     &outputMixEnvironmentalReverb);

    (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb,
                                                                      &reverbSettings);
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};

    // 第三步--------------------------------------------
    // 创建播放器
    LOGI("创建播放器");
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                            2};
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,//采样率
            SL_PCMSAMPLEFORMAT_FIXED_16,//采样位数
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN//结束标志
    };


    SLDataSource slDataSource = {&android_queue, &pcm};
    SLDataSink audioSnk = {&outputMix, nullptr};
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND, SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};


    //创建播放器(录音器)
    (*engineEngine)->CreateAudioPlayer(engineEngine, &pcmPlayerObject, &slDataSource, &audioSnk, 3,
                                       ids, req);
    //初始化播放器
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    //得到接口后调用  获取Player接口
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);


    //第四步---------------------------------------
    // 创建缓冲区和回调函数
    LOGI("创建缓冲区和回调函数");

    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);

    //4.设置缓冲队列和回调函数
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, pcmBufferCallback, nullptr);

    //5.设置播放状态
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);

    //6.启动回调函数
    pcmBufferCallback(pcmBufferQueue, nullptr);

}