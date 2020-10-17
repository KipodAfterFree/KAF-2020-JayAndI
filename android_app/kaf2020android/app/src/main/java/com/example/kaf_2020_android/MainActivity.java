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

        readStoragePermission();

        final EditText username = (EditText) findViewById(R.id.input_name);
        final EditText password_text = (EditText) findViewById(R.id.input_password);
        final ServerIntegrate server = new ServerIntegrate();
        final Button login_button = (Button) findViewById(R.id.login_button);
        final Button register_button = (Button) findViewById(R.id.register_button);

        login_button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                final String password = password_text.getText().toString();
                final String name = username.getText().toString();
                if (password.length() == 0 || name.length() == 0) {
                    Toast.makeText(MainActivity.this, "Password and email can't be empty!", Toast.LENGTH_LONG).show();
                } else {
                    new Thread(new Runnable() {
                        public void run() {
                            try {
                                if (server.login(name, password)) {
                                    MainActivity.this.showToast("Login has succeeded!");
                                    Intent i = new Intent(MainActivity.this, HomeActivity.class);
                                    i.putExtra("username", name);
                                    i.putExtra("password", password);
                                    startActivity(i);
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
                age_input.setInputType(InputType.TYPE_CLASS_NUMBER);
                AlertDialog show = new AlertDialog.Builder(MainActivity.this).setTitle("Confirm age").setView(age_input).setPositiveButton("Confirm", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialogInterface, int i) {
                        final String age = age_input.getText().toString();
                        if (!MainActivity.is_numeric(age)) {
                            MainActivity.this.showToast("Age must be numeric");
                            return;
                        }
                        final String password = password_text.getText().toString();
                        final String email = username.getText().toString();
                        new Thread(new Runnable() {
                            public void run() {
                                try {
                                    if (server.register(email, password, age)) {
                                        MainActivity.this.showToast("Registration was successful!");
                                    } else {
                                        MainActivity.this.showToast("Registration has failed!");
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

    @Override
    public void onRequestPermissionsResult(
            int requestCode,
            String permissions[],
            int[] grantResults) {
        switch (requestCode) {
            case REQUEST_PERMISSION_EXTERNAL_STORAGE_STATE:
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(MainActivity.this, "Permission Granted!", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(MainActivity.this, "Permission Denied!", Toast.LENGTH_SHORT).show();
                }
        }
    }

    private void showExplanation(String title,
                                 String message,
                                 final String permission,
                                 final int permissionRequestCode) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle(title)
                .setMessage(message)
                .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        requestPermission(permission, permissionRequestCode);
                    }
                });
        builder.create().show();
    }

    private void requestPermission(String permissionName, int permissionRequestCode) {
        ActivityCompat.requestPermissions(this,
                new String[]{permissionName}, permissionRequestCode);
    }

    private void readStoragePermission() {
        int permissionCheck = ContextCompat.checkSelfPermission(
                this, Manifest.permission.READ_EXTERNAL_STORAGE);
        if (permissionCheck != PackageManager.PERMISSION_GRANTED) {
            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.READ_EXTERNAL_STORAGE)) {
                showExplanation("Permission Needed", "Rationale", Manifest.permission.READ_EXTERNAL_STORAGE, REQUEST_PERMISSION_EXTERNAL_STORAGE_STATE);
            } else {
                requestPermission(Manifest.permission.READ_EXTERNAL_STORAGE, REQUEST_PERMISSION_EXTERNAL_STORAGE_STATE);
            }
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