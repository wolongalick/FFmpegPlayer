package com.alick.ffmpegplayer;

import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

public class OpenSLESActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_sles);
    }


    public void playPcmByOpenSL_ES(View view) {

    }


    public native void ByOpenSL_ES(String url);
}
