package com.example.kaf_2020_android;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.DialogFragment;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.StrictMode;
import android.provider.MediaStore;
import android.text.Layout;
import android.util.Base64;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.util.List;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;

import static android.os.StrictMode.*;

public class HomeActivity extends AppCompatActivity {
    public AlertDialog current_alert = null;

    static {
        System.loadLibrary("bmp");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.home);

        ThreadPolicy policy = new ThreadPolicy.Builder().permitAll().build();
        setThreadPolicy(policy);

        final ServerIntegrate server = new ServerIntegrate();

        final String username = getIntent().getStringExtra("username");
        final String password = "l33t@ttack3r";

        TextView username_view = (TextView) findViewById(R.id.username_text);
        username_view.setText(String.format("Welcome %s!", username));

        /*
        Get image from server
         */

        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                try {
                    if (hasOpenedDialogs())
                        return;

                    String base64PicWithUsername = server.getPicture(username, password);

                    if (base64PicWithUsername != null) {
                        String[] base64PicWithUsernameArr = base64PicWithUsername.split(":");
                        if (base64PicWithUsernameArr.length != 2) {
                            showToast("Image received is invalid");
                            return;
                        }
                        String base64Pic = base64PicWithUsernameArr[0];
                        final String target = base64PicWithUsernameArr[1];
                        byte[] decoded = Base64.decode(base64Pic.replaceAll("\n", ""), Base64.DEFAULT | Base64.URL_SAFE);
                        Bitmap bmp = BitmapFactory.decodeByteArray(decoded, 0, decoded.length);

                        final Bitmap new_bmp = modifyBitmapGrayscale(bmp);

                        if (new_bmp == null)
                            return;

                        ByteArrayOutputStream stream = new ByteArrayOutputStream();
                        new_bmp.compress(Bitmap.CompressFormat.PNG, 100, stream);
                        try {
                            server.sendPicture(username, password, target, Base64.encodeToString(stream.toByteArray(), Base64.NO_WRAP | Base64.URL_SAFE));
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }, 10000, 10000);

    }

    public void showToast(final String desc) {
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(HomeActivity.this, desc, Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void switchToMainActivity() {
        Intent i = new Intent(HomeActivity.this, MainActivity.class);
        startActivity(i);
    }

    public boolean hasOpenedDialogs() {
        return (this.current_alert != null && this.current_alert.isShowing());
    }

    public void generateNewAlert(final Bitmap bmp) {
        runOnUiThread(new Runnable() {
            public void run() {
                AlertDialog.Builder builder = new AlertDialog.Builder(HomeActivity.this);

                LayoutInflater layoutInflaterAndroid = LayoutInflater.from(HomeActivity.this);
                View layout_view = layoutInflaterAndroid.inflate(R.layout.alert, null);
                builder.setView(layout_view);
                builder.setCancelable(true);

                final AlertDialog alert_dialog = builder.create();

                HomeActivity.this.current_alert = alert_dialog;

                if(!isFinishing())
                {
                    alert_dialog.show();
                    // layout_view.findViewById(R.id.ok_button_alert).setOnClickListener();
                    ImageView img = layout_view.findViewById(R.id.picture_alert);
                    img.setImageBitmap(bmp);
                }
            }
        });
    }

    private native Bitmap modifyBitmapGrayscale(Bitmap bmp);
}