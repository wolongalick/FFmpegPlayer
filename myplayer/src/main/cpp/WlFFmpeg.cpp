//
// Created by Administrator on 2019/12/25.
//


#include "WlFFmpeg.h"

WlFFmpeg::WlFFmpeg(WlPlaystatus *wlPlaystatus, WlCallJava *callJava, const char *url) {
    this->playstatus = wlPlaystatus;
    this->callJava = callJava;
    this->url = url;
}


WlFFmpeg::~WlFFmpeg() {

}

void *decodeFFmpeg(void *data) {
    WlFFmpeg *wlFFmpeg = static_cast<WlFFmpeg *>(data);

    wlFFmpeg->decodeFFmpegThread();

    pthread_exit(&(wlFFmpeg->decodeThread));

}

void WlFFmpeg::prepared() {

    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);

}

void WlFFmpeg::decodeFFmpegThread() {

    av_register_all();
    avformat_network_init();

    pFormatCtx = avformat_alloc_context();

    int result = avformat_open_input(&pFormatCtx, url, NULL, nullptr);
    if (result != 0) {
        //返回值不为0说明失败了
        LOGE("avformat_open_input失败:%s", av_err2str(result));
        return;
    }

    result = avformat_find_stream_info(pFormatCtx, nullptr);
    if (result < 0) {
        //返回值小于0说明失败了
        LOGE("avformat_find_stream_info失败:%s", av_err2str(result));
        return;
    }

    LOGI("流个数:%d", pFormatCtx->nb_streams);

    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        AVCodecParameters *pParameters = pFormatCtx->streams[i]->codecpar;
        AVMediaType avMediaType = pParameters->codec_type;
        LOGI("avMediaType类型:%d", avMediaType);
        if (avMediaType == AVMEDIA_TYPE_AUDIO) {
            if (wlAudio == nullptr) {
                wlAudio = new WlAudio(this->playstatus);
                wlAudio->streamIndex = i;
                wlAudio->codecpar = pParameters;
                LOGI("wlAudio初始化成功");
            } else {
                LOGI("wlAudio不为null,不需要初始化");
            }
            break;
        }
    }

    AVCodec *avCodec = avcodec_find_decoder(wlAudio->codecpar->codec_id);
    if (!avCodec) {
        LOGE("avcodec_find_decoder失败");
        return;
    }

    wlAudio->avCodecContext = avcodec_alloc_context3(avCodec);

    if (!wlAudio->avCodecContext) {
        LOGE("avcodec_alloc_context3失败");
        return;
    }

    result = avcodec_parameters_to_context(wlAudio->avCodecContext, wlAudio->codecpar);

    if (result < 0) {
        //小于0代表失败
        LOGE("avcodec_parameters_to_context失败:%s", av_err2str(result));
        return;
    }

    result = avcodec_open2(wlAudio->avCodecContext, avCodec, nullptr);
    if (result != 0) {
        LOGE("avcodec_open2失败:%s", av_err2str(result));
        return;
    }

    callJava->onCallPrepared(CHILD_THREAD);

    wlAudio->avCodecContext = avcodec_alloc_context3(avCodec);


}

void WlFFmpeg::start() {
    if (wlAudio == nullptr) {
        LOGE("wlAudio为null");
        return;
    }

    wlAudio->play();


    int count = 0;

    while (playstatus != nullptr && !playstatus->exit) {
        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(pFormatCtx, avPacket) == 0) {
            //返回0代表成功
            if (avPacket->stream_index == wlAudio->streamIndex) {
                count++;
                wlAudio->queue->putAvPacket(avPacket);
            } else {
                //如果不是对应的流,也将avPacket释放
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = nullptr;
            }
        } else {
            //返回非0代表失败,要将avPacket释放
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = nullptr;

            while (playstatus != nullptr && !playstatus->exit) {
                if (wlAudio->queue->getQueueSize() > 0) {
                    continue;
                } else {
                    playstatus->exit = true;
                    break;
                }
            }
        }
    }
    LOGI("解码完成");
}