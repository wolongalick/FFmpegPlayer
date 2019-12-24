package com.alick.ffmpegplayer;

import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.alick.myplayer.WlPlayer;

public class PlayPcmActivity extends AppCompatActivity {
    private WlPlayer wlPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play_pcm);
        wlPlayer=new WlPlayer();

    }




    public void playPcm(View view) {
        wlPlayer.setSource("");
        wlPlayer.prepare();

    }
}
