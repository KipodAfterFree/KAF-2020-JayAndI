package com.example.kaf_2020_android;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    /* access modifiers changed from: protected */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.authenticate);
        final EditText email_text = (EditText) findViewById(R.id.input_email);
        final EditText password_text = (EditText) findViewById(R.id.input_password);
        final ServerIntegrate server = new ServerIntegrate();
        final Button login_button = (Button) findViewById(R.id.login_button);
        final Button register_button = (Button) findViewById(R.id.register_button);

        login_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                final String password = password_text.getText().toString();
                final String email = email_text.getText().toString();
                if (password.length() == 0 || email.length() == 0) {
                    Toast.makeText(MainActivity.this, "Password and email can't be empty!", 1).show();
                } else {
                    new Thread(new Runnable() {
                        public void run() {
                            try {
                                if (server.login(email, password)) {
                                    MainActivity.this.showToast("Login has succeeded!");
                                } else {
                                    MainActivity.this.showToast("Login has failed!");
                                }
                            } catch (IOException e) {
                                MainActivity.this.showToast("Failed to connect to the server, make sure your internet is working!");
                                e.printStackTrace();
                            }
                        }
                    }).start();
                }
            }
        });
        register_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                final EditText age_input = new EditText(MainActivity.this);
                age_input.setHint("Age");
                AlertDialog show = new AlertDialog.Builder(MainActivity.this).setTitle("Confirm age").setView(age_input).setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialogInterface, int i) {
                        final String age = age_input.getText().toString();
                        if (!MainActivity.is_numeric(age)) {
                            MainActivity.this.showToast("Age must be numeric");
                            return;
                        }
                        final String password = password_text.getText().toString();
                        final String email = email_text.getText().toString();
                        new Thread(new Runnable() {
                            public void run() {
                                try {
                                    if (server.register(email, password, age)) {
                                        MainActivity.this.showToast("Login was successful!");
                                        setContentView(R.layout.home);
                                    } else {
                                        MainActivity.this.showToast("Login has failed!");
                                    }
                                } catch (IOException e) {
                                    MainActivity.this.showToast("Failed to connect to the server, make sure your internet is working!");
                                    e.printStackTrace();
                                }
                            }
                        }).start();
                    }
                }).setNegativeButton("Cancel", (DialogInterface.OnClickListener) null).show();
            }
        });
    }

    public static boolean is_numeric(String str) {
        try {
            Integer.parseInt(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    public void showToast(final String desc) {
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(MainActivity.this, desc, Toast.LENGTH_LONG).show();
            }
        });
    }
}