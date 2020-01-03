//
// Created by Administrator on 2019/12/25.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    queue = new WlQueue(playstatus);


    //44100:代表采样率
    //2:代表声道数
    //16:代表16位(需要除以8,转换成字节)
    buffer = static_cast<uint8_t *>(av_malloc(44100 * 2 * (16 / 8)));
}

WlAudio::~WlAudio() {

}


/**
 *  方法名为decodPlay
 * @param data  参数data为void *类型
 * @return 返回值为void *类型
 */
void *decodPlay(void *data) {

    WlAudio *wlAudio = static_cast<WlAudio *>(data);

    wlAudio->resampleAudio();

    pthread_exit(&wlAudio->thread_play);
}

void WlAudio::play() {
    pthread_create(&thread_play, nullptr, decodPlay, this);
}

int WlAudio::resampleAudio() {

    while (playstatus != nullptr && !playstatus->exit) {
        //如果没退出
        avPacket = av_packet_alloc();

        if (queue->getAvPacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = nullptr;
            continue;
        }

        result = avcodec_send_packet(avCodecContext, avPacket);
        if (result != 0) {
            LOGE("avcodec_send_packet失败:%s", av_err2str(result));
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = nullptr;
            continue;
        }

        avFrame = av_frame_alloc();
        result = avcodec_receive_frame(avCodecContext, avFrame);

        if (result == 0) {
            //成功
            if (avFrame->channels > 0 && avFrame->channel_layout == 0) {
                avFrame->channel_layout = av_get_default_channel_layout(avFrame->channels);
            } else if (avFrame->channels == 0 && avFrame->channel_layout > 0) {
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }

            SwrContext *swr_ctx = nullptr;
            swr_ctx = swr_alloc_set_opts(
                    swr_ctx,//
                    AV_CH_LAYOUT_STEREO,
                    AV_SAMPLE_FMT_S16,
                    avFrame->sample_rate,
                    avFrame->channel_layout,
                    static_cast<AVSampleFormat>(avFrame->format),
                    avCodecContext->sample_rate,
                    0,
                    nullptr
            );

            if (!swr_ctx || swr_init(swr_ctx) < 0) {
                //失败
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = nullptr;
                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = nullptr;
                if (swr_ctx != nullptr) {
                    swr_free(&swr_ctx);
                    swr_ctx = nullptr;
                }
                continue;
            }

            //每个通道输出的样本数量
            int nb = swr_convert(
                    swr_ctx,
                    &buffer,
                    avFrame->nb_samples,
                    (const uint8_t **)(avFrame->data),
                    avFrame->nb_samples
            );

            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            data_size = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);

            LOGE("data size is %d", data_size);

            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = nullptr;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = nullptr;

            av_free(swr_ctx);
            swr_ctx = nullptr;

            result = swr_init(swr_ctx);
            if (result != 0) {
                LOGE("swr_init failed");
            } else {
                LOGI("swr_init success");
            }

        } else {
            //失败
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = nullptr;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = nullptr;
            continue;
        }

    }


    return 0;
}


