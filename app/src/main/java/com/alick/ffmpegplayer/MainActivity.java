package com.alick.ffmpegplayer;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import com.alick.ffmpegplayer.utils.T;


public class MainActivity extends AppCompatActivity {

    private EditText etFilePath;
    private MyVideoView myVideoView;
    private static final int REQUEST_CODE=1000;
    private String[] permissionGroup=new String[]{Manifest.permission.READ_EXTERNAL_STORAGE,Manifest.permission.WRITE_EXTERNAL_STORAGE};

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etFilePath=findViewById(R.id.et_filePath);
        myVideoView=findViewById(R.id.videoView);
    }

    public void clickParseFile(View view) {
        ActivityCompat.requestPermissions(this,permissionGroup,REQUEST_CODE);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(requestCode==REQUEST_CODE){
            for (int result:grantResults) {
                if(result== PackageManager.PERMISSION_DENIED){
                    T.show(MainActivity.this,"请授予读写存储卡权限");
                    return;
                }
            }
            preParseFile();
        }
    }

    private void preParseFile(){
        String filePath = etFilePath.getText().toString().trim();
        if(TextUtils.isEmpty(filePath)){
            T.show(MainActivity.this,"文件路径不能为空");
            return;
        }
//        myVideoView.player(filePath);
        String result = parseFile(filePath);
        T.show(MainActivity.this,result);
    }

    private native String parseFile(String filePath);

    public void clear(View view) {
        etFilePath.setText(null);
    }
}
