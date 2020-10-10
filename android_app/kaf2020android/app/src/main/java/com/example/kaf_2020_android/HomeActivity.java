package com.example.kaf_2020_android;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class HomeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.home);

        String username = getIntent().getStringExtra("username");
        TextView username_view = (TextView) findViewById(R.id.username_text);
        username_view.setText(String.format("Welcome %s!", username));
    }
}