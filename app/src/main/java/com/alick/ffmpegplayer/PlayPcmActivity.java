package com.alick.ffmpegplayer;

import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.alick.myplayer.WlPlayer;
import com.alick.myplayer.listener.OnPreparedListener;
import com.alick.myplayer.utils.BLog;

public class PlayPcmActivity extends AppCompatActivity {
    private WlPlayer wlPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play_pcm);
        wlPlayer=new WlPlayer();
        wlPlayer.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared() {
                BLog.i("准备好了--->onPrepared()");
                wlPlayer.start();
            }
        });
    }

    public void playPcm(View view) {
        //世间美好与你环环相扣.MP3
//        wlPlayer.setSource("http://192.168.31.48:8080/examples/xingli.mp3");
//        wlPlayer.setSource("http://q33lsnest.bkt.clouddn.com/siben.mp3");
//        wlPlayer.setSource("http://q33lsnest.bkt.clouddn.com/Valve%20Studio%20Orchestra%20-%20music%20battle%20002.mp3");
        wlPlayer.setSource("http://q33lsnest.bkt.clouddn.com/Valve%20Studio%20Orchestra%20-%20hero%20select%20underscore%20loop.mp3");
        wlPlayer.prepare();
    }
}
