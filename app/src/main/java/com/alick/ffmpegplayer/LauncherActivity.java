package com.alick.ffmpegplayer;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.alick.myplayer.utils.T;

public class LauncherActivity extends AppCompatActivity {
    private static final int      REQUEST_CODE    = 1000;
    private              String[] permissionGroup = new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE};
    private boolean hasPermission=true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lanucher);
        ActivityCompat.requestPermissions(this, permissionGroup, REQUEST_CODE);
    }

    public void gotoPlayPcm(View view) {
        if(!hasPermission){
            ActivityCompat.requestPermissions(this, permissionGroup, REQUEST_CODE);
            return;
        }
        startActivity(new Intent(this, PlayPcmActivity.class));
    }

    public void gotoPlayMP4(View view) {
        if(!hasPermission){
            ActivityCompat.requestPermissions(this, permissionGroup, REQUEST_CODE);
            return;
        }
        startActivity(new Intent(this, PlayMP4Activity.class));
    }


    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_CODE) {
            for (int result : grantResults) {
                if (result == PackageManager.PERMISSION_DENIED) {
                    T.show(LauncherActivity.this, "请授予读写存储卡权限");
                    hasPermission=false;
                    return;
                }
            }
        }
    }

}
