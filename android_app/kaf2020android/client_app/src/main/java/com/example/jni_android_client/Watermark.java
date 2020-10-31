package com.example.jni_android_client;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;

public class Watermark {
    public static Bitmap setWatermark(Bitmap src, char[] watermark, int x, int y, int color, int alpha, int size) {
        int w = src.getWidth();
        int h = src.getHeight();
        Bitmap result = Bitmap.createBitmap(w, h, src.getConfig());

        Canvas canvas = new Canvas(result);
        canvas.drawBitmap(src, 0, 0, null);

        Paint paint = new Paint();
        paint.setColor(color);
        paint.setAlpha(alpha);
        paint.setTextSize(size);
        paint.setAntiAlias(true);

        canvas.drawText("Resent by: " + String.valueOf(watermark), x, h - y, paint);

        return result;
    }

}
