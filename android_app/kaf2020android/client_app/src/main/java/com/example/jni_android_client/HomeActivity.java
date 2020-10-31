package com.example.jni_android_client;

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
import android.provider.MediaStore;
import android.util.Base64;
import android.util.Log;
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
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

import static android.os.StrictMode.*;

public class HomeActivity extends AppCompatActivity {
    public static final int PICK_IMAGE = 1;
    public Bitmap default_img = null;
    public AlertDialog current_alert = null;
    public Timer new_pic_timer = null;
    static {
        System.loadLibrary("bmp");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.home);

        default_img = BitmapFactory.decodeResource(this.getResources(), R.drawable.default_img);

        ThreadPolicy policy = new ThreadPolicy.Builder().permitAll().build();
        setThreadPolicy(policy);

        final ServerIntegrate server = new ServerIntegrate();

        final String username = getIntent().getStringExtra("username");
        final String password = getIntent().getStringExtra("password");
        final Button send_image_button = (Button) findViewById(R.id.send_image);
        final Button logout_button = (Button) findViewById(R.id.logout_button);

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
        logout_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getIntent().removeExtra("password");
                getIntent().removeExtra("username");
                switchToMainActivity();
                finish();
            }
        });
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
                } catch (IOException | InterruptedException | IllegalStateException e) {
                    e.printStackTrace();
                    showToast("Couldn't get user list!");
                    return;
                }
                if (users == null) {
                    showToast("You are not an admin.");
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
                            ImageView sent_image = (ImageView) findViewById(R.id.profile_pic);
                            sent_image.setImageBitmap(default_img);
                            getIntent().removeExtra("send_image");
                        } catch (IOException e) {
                            e.printStackTrace();
                            showToast("Failed sending image!");
                        }
                    }
                });
                builder.show();
            }
        });

        /*
        Get image from server
         */
        new_pic_timer = new Timer("PICTURE_TIMER");
        new_pic_timer.schedule(new TimerTask() {
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
                        generateNewAlert(bmp);

                        // Wait for main thread to open dialog
                        while (!hasOpenedDialogs()) {}

                        // Wait for main thread to close dialog
                        while (hasOpenedDialogs()) {}

                        Thread.sleep(500);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    chooseResendAlert(new_bmp, server, target);
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }
                            }
                        });
                    }
                } catch (IOException | InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }, 5000, 5000);

    }
    private void chooseResendAlert(final Bitmap new_bmp, final ServerIntegrate server, final String target) throws InterruptedException {


        String title = "Resend picture";
        String message = "Would you like to resend the picture?";
        Intent currentIntent = HomeActivity.this.getIntent();

        final String username = currentIntent.getStringExtra("username");
        final String password = currentIntent.getStringExtra("password");

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle(title)
                .setMessage(message)
                .setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        // Resend bitmap
                        ByteArrayOutputStream stream = new ByteArrayOutputStream();
                        new_bmp.compress(Bitmap.CompressFormat.PNG, 100, stream);
                        try {
                            server.sendPicture(username, password, target, Base64.encodeToString(stream.toByteArray(), Base64.NO_WRAP | Base64.URL_SAFE));
                        } catch (IOException e) {
                            e.printStackTrace();
                        }

                    }
                })
                .setNegativeButton(android.R.string.cancel, null);
        builder.create().show();
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
                    showToast("Couldn't load image (too big)");
                    return;
                }

                final Bitmap bmp = BitmapFactory.decodeByteArray(imageBuf, 0, imageBuf.length);
                if (bmp.getWidth() < 200 || bmp.getHeight() < 200) {
                    showToast("Picture too small. Please add a picture greater than 200x200");
                    return;
                }
                changeProfilePic(bmp);
                HomeActivity.this.getIntent().putExtra("send_image", Base64.encodeToString(imageBuf, Base64.NO_WRAP | Base64.URL_SAFE));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void showToast(final String desc) {
        runOnUiThread(new Runnable() {
            public void run() {
                Toast.makeText(HomeActivity.this, desc, Toast.LENGTH_SHORT).show();
            }
        });
    }

    public void switchToMainActivity() {
        if (new_pic_timer != null) {
            new_pic_timer.cancel();
            new_pic_timer = null;
        }
        Intent i = new Intent(HomeActivity.this, MainActivity.class);
        startActivity(i);
    }

    public boolean hasOpenedDialogs() {
        // Log.e("12345", "Current alert: " + ((this.current_alert != null) ? this.current_alert.toString() : "null"));
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
                builder.setOnDismissListener(new DialogInterface.OnDismissListener() {
                    @Override
                    public void onDismiss(DialogInterface dialogInterface) {
                        HomeActivity.this.current_alert = null;
                    }
                });

                final AlertDialog alert_dialog = builder.create();

                HomeActivity.this.current_alert = alert_dialog;

                if (!isFinishing()) {
                    alert_dialog.show();
                    // layout_view.findViewById(R.id.ok_button_alert).setOnClickListener();
                    ImageView img = layout_view.findViewById(R.id.picture_alert);

                    img.setImageBitmap(bmp);
                }
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

    private native Bitmap modifyBitmapGrayscale(Bitmap bmp);
}