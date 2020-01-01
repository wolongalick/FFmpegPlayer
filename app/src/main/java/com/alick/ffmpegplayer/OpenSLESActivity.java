package com.alick.ffmpegplayer;

import android.os.Bundle;
import android.os.Environment;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.alick.myplayer.utils.T;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class OpenSLESActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_sles);
    }


    public void playPcmByOpenSL_ES(View view) {

        final String fileName="nfs.pcm";
        try {
            File pcmFile=new File(Environment.getExternalStorageDirectory(),fileName);

            if(pcmFile.isFile() && pcmFile.exists()){
                ByOpenSL_ES(pcmFile.getAbsolutePath());
            }else {
                InputStream inputStream = getAssets().open(fileName);
                FileOutputStream outputStreamWriter=null;
                try {
                    outputStreamWriter   =new FileOutputStream(pcmFile);
                    byte[] buffer=new byte[2048];
                    int size;
                    while ((size=inputStream.read(buffer))>0){
                        outputStreamWriter.write(buffer,0,size);
                        outputStreamWriter.flush();
                    }
                } finally {
                    inputStream.close();
                    if(outputStreamWriter!=null){
                        outputStreamWriter.close();
                    }
                }

                if(pcmFile.isFile()&& pcmFile.exists()){
                    ByOpenSL_ES(pcmFile.getAbsolutePath());
                }else {
                    T.show(OpenSLESActivity.this,"nfs.pcm文件不存在");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


    public native void ByOpenSL_ES(String url);
}
