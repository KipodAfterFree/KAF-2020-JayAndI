//
// Created by mal on 9/27/20.
//

#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <jni.h>
#include <android/bitmap.h>
#include <bmp_parse.h>
#include "../include/bmp_parse.h"

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

extern "C"
JNIEXPORT jobject JNICALL
Java_com_example_kaf_12020_1android_HomeActivity_modifyBitmapGrayscale(JNIEnv *env, jobject thiz,
                                                                       jobject bmp) {
    jclass bmp_class = env->GetObjectClass(bmp);
    jfieldID native_field = env->GetFieldID(bmp_class, "mNativePtr", "J");
    jlong native_ptr = env->GetLongField(bmp, native_field);

    jfieldID width_field = env->GetFieldID(bmp_class, "mWidth", "I");
    jint width = env->GetIntField(bmp, width_field);

    jfieldID height_field = env->GetFieldID(bmp_class, "mHeight", "I");
    jint height = env->GetIntField(bmp, height_field);

    jmethodID set_pixel_method = env->GetStaticMethodID(bmp_class, "nativeSetPixel", "(JIII)V");
    /*
     * TODO: Improve speed, get all array at once, set all array at once
     */
    jmethodID get_pixel_method = env->GetStaticMethodID(bmp_class, "nativeGetPixel", "(JII)I");

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < height; ++j) {
            jint color_val = env->CallStaticIntMethod(bmp_class, get_pixel_method, native_ptr, i, j);
            env->CallStaticVoidMethod(bmp_class, set_pixel_method, native_ptr, i, j,
                                      convert_to_grayscale(color_val));
        }
    return bmp;
}