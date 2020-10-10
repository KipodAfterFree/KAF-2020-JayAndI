package com.example.kaf_2020_android;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.IOException;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.authenticate);

        Button login_button = (Button)findViewById(R.id.login_button);

        final EditText email_text = (EditText)findViewById(R.id.input_email);
        final EditText password_text = (EditText)findViewById(R.id.input_password);

        login_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String password = password_text.getText().toString();
                String email = email_text.getText().toString();
                if (password.length() == 0 || email.length() == 0) {
                    Toast.makeText(MainActivity.this, "Password and email can't be empty!", Toast.LENGTH_LONG).show();
                    return;
                }

                String login_status;

                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            sendRequest("https://postb.in/1602108915311-2249859930016");
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }).start();


                Toast.makeText(MainActivity.this, "Failed to login", Toast.LENGTH_LONG).show();
            }
        });
    }

    String sendRequest(String url) throws IOException {
        OkHttpClient client = new OkHttpClient();

        Request request = new Request.Builder()
                .url(url)
                .build();

        try (Response response = client.newCall(request).execute()) {
            return response.body().string();
        }
    }
}