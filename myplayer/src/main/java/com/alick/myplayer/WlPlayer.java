package com.alick.myplayer;

/**
 * @author 崔兴旺
 * @createTime 2019/12/24 13:51
 * @description
 */
public class WlPlayer {
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("libavcodec-57.so");
        System.loadLibrary("libavdevice-57.so");
        System.loadLibrary("libavfilter-6.so");
        System.loadLibrary("libavformat-57.so");
        System.loadLibrary("libavutil-55.so");
        System.loadLibrary("libpostproc-54.so");
        System.loadLibrary("libswresample-2.so");
        System.loadLibrary("libswscale-4.so");
    }

    private String source;

    public WlPlayer() {

    }

    public void setSource(String source) {
        this.source = source;
    }

    public void prepare(){

    }




}
