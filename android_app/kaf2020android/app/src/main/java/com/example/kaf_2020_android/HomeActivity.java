package com.example.kaf_2020_android;

import androidx.appcompat.app.AppCompatActivity;

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
import android.util.Base64;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.util.Objects;

import static android.os.StrictMode.*;

public class HomeActivity extends AppCompatActivity {
    public static final int PICK_IMAGE = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.home);

        ThreadPolicy policy = new ThreadPolicy.Builder().permitAll().build();
        setThreadPolicy(policy);

        final ServerIntegrate server = new ServerIntegrate();

        final String username = getIntent().getStringExtra("username");
        final String password = getIntent().getStringExtra("password");
        TextView username_view = (TextView) findViewById(R.id.username_text);
        username_view.setText(String.format("Welcome %s!", username));

        ImageView profile_pic = (ImageView) findViewById(R.id.profile_pic);
        profile_pic.setClickable(true);

        profile_pic.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                startActivityForResult(Intent.createChooser(intent, "Select Picture"), PICK_IMAGE);
            }
        });

        final Button send_image_button = (Button) findViewById(R.id.send_image);
        send_image_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent current_intent = HomeActivity.this.getIntent();
                final String b64_image = current_intent.getStringExtra("send_image");
                if (b64_image == null) {
                    showToast("No image chosen");
                    return;
                }
                String users;

                try {
                    users = server.getUserList(username, password);
                } catch (IOException | InterruptedException e) {
                    e.printStackTrace();
                    return;
                }

                final String[] userList = users.split(",");
                AlertDialog.Builder builder = new AlertDialog.Builder(HomeActivity.this);
                builder.setTitle("Send to user");
                builder.setItems(userList, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            server.sendPicture(username, password, userList[which], b64_image);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                });
                builder.show();
            }
        });

        /*
        Get image from server
         */
        /*
        new Thread(new Runnable() {
            public void run() {
                try {
                    String base64Pic = server.getPicture(username, password);

                    if (base64Pic != null) {
                        byte[] decoded = Base64.decode(base64Pic, Base64.DEFAULT);
                        Bitmap bmp = BitmapFactory.decodeByteArray(decoded, 0, decoded.length);
                        changeProfilePic(bmp);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();
        */
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == PICK_IMAGE && resultCode == Activity.RESULT_OK) {
            if (data == null)
                return;

            Uri uri = data.getData();
            assert uri != null;

            String[] filePathColumn = {MediaStore.Images.Media.DATA};
            Cursor cursor = getContentResolver().query(uri, filePathColumn, null, null, null);

            assert cursor != null;

            cursor.moveToFirst();
            int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
            String picturePath = cursor.getString(columnIndex);
            cursor.close();

            File image = new File(picturePath);
            BufferedInputStream bufStream;
            try {
                 bufStream = new BufferedInputStream(new FileInputStream(image));
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                return;
            }
            byte[] imageBuf =  new byte[(int) image.length()];

            try {
                int res = bufStream.read(imageBuf);
                if (res == -1) {
                    Toast.makeText(HomeActivity.this, "Couldn't load image (too big)", Toast.LENGTH_LONG).show();
                    return;
                }

                final Bitmap bmp = BitmapFactory.decodeByteArray(imageBuf, 0, imageBuf.length);
                changeProfilePic(bmp);
                HomeActivity.this.getIntent().putExtra("send_image", Base64.encodeToString(imageBuf, Base64.DEFAULT));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void showToast(final String desc) {
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(HomeActivity.this, desc, Toast.LENGTH_LONG).show();
            }
        });
    }

    public void changeProfilePic(final Bitmap bmp) {
        runOnUiThread(new Runnable() {
            public void run() {
                ImageView profile_pic = (ImageView) findViewById(R.id.profile_pic);
                profile_pic.setImageBitmap(bmp);
            }
        });
    }
}