package com.alick.myplayer;

import android.text.TextUtils;

import com.alick.myplayer.listener.OnPreparedListener;
import com.alick.myplayer.utils.BLog;

/**
 * @author 崔兴旺
 * @createTime 2019/12/24 13:51
 * @description
 */
public class WlPlayer {
    private String source;
    private OnPreparedListener onPreparedListener;

    public WlPlayer() {

    }

    public void setSource(String source) {
        this.source = source;
    }

    public void prepare(){
        if(TextUtils.isEmpty(source)){
            BLog.i("source不能为null");
            return;
        }

        new Thread(){
            @Override
            public void run() {
                super.run();
                n_prepare(source);

            }
        }.start();

    }

    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        this.onPreparedListener = onPreparedListener;
    }

    public void start(){
        if(TextUtils.isEmpty(source)){
            if(TextUtils.isEmpty(source)){
                BLog.i("source不能为null");
                return;
            }
        }

        new Thread(){
            @Override
            public void run() {
                n_start();
            }
        }.start();
    }

    public void onCallPrepared(){
        if(onPreparedListener!=null){
            onPreparedListener.onPrepared();
        }
    }

    public native void n_prepare(String source);


    public native void n_start();

}
