//
// Created by mal on 9/27/20.
//

#ifndef KAF_ANDROID_CHALLENGE_BMP_PARSE_H
#define KAF_ANDROID_CHALLENGE_BMP_PARSE_H
typedef struct __attribute__((packed)) {
    struct {
        unsigned char first;
        unsigned char second;
    } signature;
    unsigned int file_size;
    unsigned int reserved;
    unsigned int data_offset;
} bmp_header;

typedef struct __attribute__((packed)) {
    /*
     * Size of info_header struct (=40)
     */
    unsigned int size;
    /*
     * Width / height (in pixels)
     */
    unsigned int width;
    unsigned int height;
    /*
     * Number of planes in bitmap (=1)
     */
    unsigned short planes;
    /*
     * Bits per pixel:
     *  1 = 1 bit per pixel (monochrome)
     *  4 = 4 bit per pixel (16 bit colors)
     *  8 = 8 bit per pixel (256 bit colors)
     *  16 = 16 bit per pixel (65536 RGB)
     *  24 = 24 bit per pixel (16 million colors)
     *  32 = 32 bit per pixel
     */
    unsigned short bit_count;
    /*
     * Compression:
     *  0 = BI_RGB - no compression
     *  1 = BI_RLE8 - 8 bit RLE encoding
     *  2 = BI_RLE4 - 4 bit RLE encoding
     */
    unsigned int compression;
    /*
     * Compressed size of image
     */
    unsigned int image_size;
    /*
     * Horizontal resolution: Pixels / Meter
     */
    unsigned int x_pixels_per_meter;
    /*
     * Vertical resolution: Pixels / Meter
     */
    unsigned int y_pixels_per_meter;
    /*
     * Number of actually used colors
     */
    unsigned int colors_used;
    /*
     * Number of all important colors:
     *  0 = all
     */
    unsigned int important_colors;
} bmp_info_header;

/*
 * Element in color_table
 */
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char reserved;
} bmp_color_elem;

/*
 * Size of this struct is 2 ^ (bmp_info_header->bit_count + 2)
 */
typedef struct {
    bmp_color_elem* color_table;
    unsigned int table_size; // size in elements not in bytes
} bmp_color_table;

/*
 * BMP Big struct
 */
typedef struct {
    bmp_header header;
    bmp_info_header info_header;
    bmp_color_table *color_table;
    void* data;
} bmp_file;

/*
 * Parses BMP
 */

int read_bmp();

bool verify_bmp(bmp_header* header);

/*
 * Print BMP functions
 */
void print_bmp_header(bmp_header* header);

#endif //KAF_ANDROID_CHALLENGE_BMP_PARSE_H
