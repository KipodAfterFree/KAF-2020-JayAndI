//
// Created by mal on 9/27/20.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "../include/bmp_parse.h"

#define BMP_FILE "../res/test.bmp"
#define MAX_SIZE 4 * 1024  // 4KB

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    read_bmp();
    return 0;
}

bool verify_bmp(bmp_header* header) {
    if (header->signature.first != 'B' ||
            header->signature.second != 'M') {
        printf("[-] Invalid signature found: %x\n", header->signature);
        return false;
    }
#ifdef RELEASE
    if (header.file_size >= MAX_SIZE) {
        printf("[-] File size too large: %x, max: %x\n", header.file_size, MAX_SIZE);
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
    printf("* File size: 0x%x (%1$d)\n", header->file_size);
    printf("* Data offset: 0x%x (%1$d)\n", header->data_offset);
    printf("\n");
}

int read_bmp() {
#ifdef DEBUG
    printf("[+] Starting to parse bmp\n");
#endif

    bmp_header header;
    FILE* bmp_file;
    struct stat bmp_stat;

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

    return 0;
}
