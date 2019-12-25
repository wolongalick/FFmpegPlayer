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
            }
        });
    }




    public void playPcm(View view) {
        //世间美好与你环环相扣MP3
        wlPlayer.setSource("http://m7.music.126.net/20191224212757/47d12664c1c1c12f08c1e12bcbc7eab1/ymusic/0459/045c/520c/330c359473365e50a368ef0d43bc612f.mp3");
        wlPlayer.prepare();
    }
}
