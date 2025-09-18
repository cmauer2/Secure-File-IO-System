#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>

typedef FILE* File;
typedef enum { NONE, OPEN_FAILED, CLOSE_FAILED, READ_FAILED, WRITE_FAILED, ILLEGAL_ELF } FSError;
typedef enum { BEGINNING_OF_FILE, CURRENT_POSITION, END_OF_FILE } SeekAnchor;

extern FSError fserror;

File open_file(char *name);
void close_file(File file);
unsigned long read_file_from(File file, void *data, unsigned long num_bytes, SeekAnchor start, long offset);
unsigned long write_file_at(File file, void *data, unsigned long num_bytes, SeekAnchor start, long offset);
void fs_print_error(void);

#endif
