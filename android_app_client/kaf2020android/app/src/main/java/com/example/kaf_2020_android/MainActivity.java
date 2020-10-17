package com.example.kaf_2020_android;

import android.Manifest;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.InputType;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    private final int REQUEST_PERMISSION_EXTERNAL_STORAGE_STATE=1;

    /* access modifiers changed from: protected */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.authenticate);

        Intent cur_intent = MainActivity.this.getIntent();
        // cur_intent.putExtra("username", "attacker");
        // cur_intent.putExtra("password", "l33t@ttack3r");
        cur_intent.putExtra("username", "1");
        cur_intent.putExtra("password", "1");
        // Test if user is logged in already
        if (cur_intent.hasExtra("username") && cur_intent.hasExtra("password"))
            switchToHomeActivity(cur_intent.getStringExtra("username"), cur_intent.getStringExtra("password"));

    }

    public static boolean is_numeric(String str) {
        try {
            Integer.parseInt(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }


    public void switchToHomeActivity(String name,String password) {
        Intent i = new Intent(MainActivity.this, HomeActivity.class);
        i.putExtra("username", name);
        i.putExtra("password", password);
        startActivity(i);
    }

    public void showToast(final String desc) {
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(MainActivity.this, desc, Toast.LENGTH_LONG).show();
            }
        });
    }
}