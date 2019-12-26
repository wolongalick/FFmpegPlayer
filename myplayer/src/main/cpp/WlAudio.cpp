//
// Created by Administrator on 2019/12/25.
//

#include "WlAudio.h"

WlAudio::WlAudio(WlPlaystatus *playstatus) {
    this->playstatus=playstatus;
    queue=new WlQueue(playstatus);
}

WlAudio::~WlAudio() {

}

