//
// Created by Administrator on 2019/12/25.
//


#include "WlFFmpeg.h"

WlFFmpeg::WlFFmpeg(WlCallJava *callJava, const char *url) {
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

    int result = avformat_open_input(&pFormatCtx, url, NULL, NULL);
    if (result != 0) {
        //返回值不为0说明失败了
        LOGE("avformat_open_input失败:%s", av_err2str(result));
        return;
    }

    result = avformat_find_stream_info(pFormatCtx, NULL);
    if(result<0){
        //返回值小于0说明失败了
        LOGE("avformat_find_stream_info失败:%s", av_err2str(result));
        return;
    }

    for (int i = 0; i < pFormatCtx->nb_streams; ++i) {
        if(pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO){

            if(wlAudio==NULL){
                wlAudio=new WlAudio();
                wlAudio->streamIndex=i;
                wlAudio->codecpar=pFormatCtx->streams[i]->codecpar;
            }
        }
    }

    AVCodec *avCodec=avcodec_find_decoder(wlAudio->codecpar->codec_id);
    if(!avCodec){
        LOGE("avcodec_find_decoder失败");
        return;
    }


    result=avcodec_parameters_to_context(wlAudio->avCodecContext,wlAudio->codecpar);

    if(result<0){
        //小于0代表失败
        LOGE("avcodec_parameters_to_context失败:%s",av_err2str(result));
        return;
    }

    result = avcodec_open2(wlAudio->avCodecContext, avCodec, NULL);
    if(result!=0){
        LOGE("avcodec_open2失败:%s",av_err2str(result));
        return;
    }

    callJava->onCallPrepared(CHILD_THREAD);

    wlAudio->avCodecContext=avcodec_alloc_context3(avCodec);



}
