#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"

// GLOBALS
FSError fserror;

// private functions
static int seek_file(File file, SeekAnchor start, long offset) {
    if (!file || (start != BEGINNING_OF_FILE && start != CURRENT_POSITION && start != END_OF_FILE)) {
        return 0;
    } else {
        if (!fseek(file, offset, start == BEGINNING_OF_FILE ? SEEK_SET : (start == END_OF_FILE ? SEEK_END : SEEK_CUR))) {
            return 1;
        } else {
            return 0;
        }
    }
}

// public functions

File open_file(char *name) {
    File fp;
    fserror = NONE;

    fp = fopen(name, "r+");
    if (!fp) {
        fp = fopen(name, "w+");
        if (!fp) {
            fserror = OPEN_FAILED;
            return NULL;
        }
    }
    return fp;
}

void close_file(File file) {
    if (file && !fclose(file)) {
        fserror = NONE;
    } else {
        fserror = CLOSE_FAILED;
    }
}

unsigned long read_file_from(File file, void *data, unsigned long num_bytes, SeekAnchor start, long offset) {
    unsigned long bytes_read = 0L;

    fserror = NONE;
    if (!file || !seek_file(file, start, offset)) {
        fserror = READ_FAILED;
    } else {
        bytes_read = fread(data, 1, num_bytes, file);
        if (ferror(file)) {
            fserror = READ_FAILED;
        }
    }
    return bytes_read;
}

unsigned long write_file_at(File file, void *data, unsigned long num_bytes, SeekAnchor start, long offset) {
    unsigned long bytes_written = 0L;
    char buffer[4] = {0};

    fserror = NONE;

    if (!file || !seek_file(file, start, offset)) {
        fserror = WRITE_FAILED;
        return 0;
    }

    // Check if the write would form "\x7F""ELF" in the first four bytes
    if (offset < 4) {
        fseek(file, 0, SEEK_SET);
        fread(buffer, 1, 4, file);
        fseek(file, offset, SEEK_SET);

        for (unsigned long i = 0; i < num_bytes; i++) {
            long pos = offset + i;
            if (pos < 4) {
                buffer[pos] = ((char*)data)[i];
            }
        }

        if (memcmp(buffer, "\x7F""ELF", 4) == 0) {
            fserror = ILLEGAL_ELF;
            return 0;
        }
    }

    bytes_written = fwrite(data, 1, num_bytes, file);
    if (bytes_written < num_bytes) {
        fserror = WRITE_FAILED;
    }

    return bytes_written;
}

void fs_print_error(void) {
    printf("FS ERROR: ");
    switch (fserror) {
        case NONE: puts("NONE"); break;
        case OPEN_FAILED: puts("OPEN_FAILED"); break;
        case CLOSE_FAILED: puts("CLOSE_FAILED"); break;
        case READ_FAILED: puts("READ_FAILED"); break;
        case WRITE_FAILED: puts("WRITE_FAILED"); break;
        case ILLEGAL_ELF: puts("ILLEGAL_ELF: SHAME ON YOU!"); break;
        default: puts("** UNKNOWN ERROR **");
    }
}
