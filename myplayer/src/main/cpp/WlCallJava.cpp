//
// Created by Administrator on 2019/12/24.
//

#include "WlCallJava.h"

WlCallJava::WlCallJava(JavaVM *javaVm, JNIEnv *jniEnv, jobject *jobj) {
    this->javaVm = javaVm;
    this->jniEnv = jniEnv;
    this->jobj = jniEnv->NewGlobalRef(*jobj);

    jclass jclazz = jniEnv->GetObjectClass(this->jobj);

    if (!jclazz) {
        LOGE("get jclass 失败");
        return;
    }

    jmid_prepared = jniEnv->GetMethodID(jclazz, "onCallPrepared", "()V");


}

WlCallJava::~WlCallJava() {

}

void WlCallJava::onCallPrepared(int type) {
    if (type == MAIN_THREAD) {
        jniEnv->CallVoidMethod(jobj,jmid_prepared);
    } else if (type == CHILD_THREAD) {
        JNIEnv *jniEnv;
        if(javaVm->AttachCurrentThread(&jniEnv,0)!=JNI_OK){
            LOGE("获取子线程jnienv失败");
            return;
        }

        jniEnv->CallVoidMethod(jobj,jmid_prepared);
        javaVm->DetachCurrentThread();

    }
}
