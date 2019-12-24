package com.alick.ffmpegplayer;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceView;

/**
 * Created by LC on 2017/11/8.
 */

class MyVideoView extends SurfaceView {
    public MyVideoView(Context context) {
        this(context, null);
    }

    public MyVideoView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public MyVideoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        getHolder().setFormat(PixelFormat.RGBA_8888);
    }

    public void player(final String input) {
        new Thread() {
            @Override
            public void run() {
                super.run();

                //绘制功能 不需要交给SurfaveView        VideoView.this.getHolder().getSurface()
                render(input,getHolder().getSurface());

            }
        }.start();

    }

    public native void render(String input, Surface surface);




}
