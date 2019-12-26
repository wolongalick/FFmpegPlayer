//
// Created by Administrator on 2019/12/26.
//

#include "WlQueue.h"
#include "cxw_log.h"
#include "WlPlaystatus.h"

WlQueue::WlQueue(WlPlaystatus *playstatus) {
    this->playstatus = playstatus;
    pthread_mutex_init(&mutexPacket, nullptr);
    pthread_cond_init(&condPacket, nullptr);
}

WlQueue::~WlQueue() {

    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);


}

int WlQueue::putAvPacket(AVPacket *avPacket) {

    //加锁
    pthread_mutex_lock(&mutexPacket);

    //添加到队列
    queuePacket.push(avPacket);

    LOGI("放入一个AVPacket到队列里面,个数为:%d", queuePacket.size());

    //发送信号
    pthread_cond_signal(&condPacket);

    //解锁
    pthread_mutex_unlock(&mutexPacket);

    return 0;
}

int WlQueue::getAvPacket(AVPacket *dstAvPacket) {

    pthread_mutex_lock(&mutexPacket);

    while (playstatus != nullptr && !playstatus->exit) {

        if (queuePacket.size()>0) {
            AVPacket *&srcAVPacket = queuePacket.front();

            //拷贝AVPacket
            int result = av_packet_ref(dstAvPacket, srcAVPacket);
            if (result == 0) {
                //等于0代表成功
                queuePacket.pop();
            } else {
                //不等于0代表失败

            }

            av_packet_free(&srcAVPacket);
            av_free(srcAVPacket);
            srcAVPacket = nullptr;

            LOGI("从队列里面取出一个AVPacket,还剩%d个", queuePacket.size());
            break;
        } else {
            LOGI("等待中");
            pthread_cond_wait(&condPacket, &mutexPacket);
        }
    }

    pthread_mutex_unlock(&mutexPacket);


    return 0;
}

int WlQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);

    return size;
}
