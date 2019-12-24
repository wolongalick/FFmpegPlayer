//
// Created by Administrator on 2019/12/24.
//

#include "CallJava.h"

CallJava::CallJava(JavaVM *javaVm, JNIEnv *jniEnv, jobject *jobj) {
    this->javaVm=javaVm;
    this->jniEnv=jniEnv;
    this->jobj=*jobj;
}
