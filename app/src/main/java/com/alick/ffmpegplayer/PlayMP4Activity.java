package com.alick.ffmpegplayer;

import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

import com.alick.myplayer.utils.T;


public class PlayMP4Activity extends AppCompatActivity {

    private EditText etFilePath;
    private MyVideoView myVideoView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play_mp4);

        etFilePath=findViewById(R.id.et_filePath);
        myVideoView=findViewById(R.id.videoView);
    }

    public void clickParseFile(View view) {
        String filePath = etFilePath.getText().toString().trim();
        if(TextUtils.isEmpty(filePath)){
            T.show(PlayMP4Activity.this,"文件路径不能为空");
            return;
        }
//        myVideoView.player(filePath);
        String result = parseFile(filePath);
        T.show(PlayMP4Activity.this,result);
    }

    private native String parseFile(String filePath);

    public void clear(View view) {
        etFilePath.setText(null);
    }
}
