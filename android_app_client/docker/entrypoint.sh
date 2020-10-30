#!/bin/bash

echo "Connecting to Emulator"

adb connect emulator:5555

echo "Wait for Emulator"

sleep 30

echo "Installing application"

adb install /client/app.apk

echo "Starting application"

adb shell am start -n com.example.kaf_2020_android/com.example.kaf_2020_android.MainActivity

echo "Print logcat"

adb logcat | grep kaf_2020_android
