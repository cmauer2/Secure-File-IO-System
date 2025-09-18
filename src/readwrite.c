#include <stdio.h>
#include <stdlib.h>
#include "fileio.h"

int main() {
    File f;
    
    system("rm -f important.dat");
    printf("Creating new file called 'important.dat'...\n");
    f = open_file("important.dat");
    fs_print_error();

    // Test benign write at start
    printf("Writing 'TEST' to beginning of file...\n");
    write_file_at(f, "TEST", 4, BEGINNING_OF_FILE, 0L);
    fs_print_error();

    // Test writing ELF signature at start
    printf("Writing 'ELF' to offset 1...\n");
    write_file_at(f, "ELF", 3, BEGINNING_OF_FILE, 1L);
    fs_print_error();

    // Test illegal ELF attempt
    printf("Writing '\\x7FELF' to beginning of file...\n");
    write_file_at(f, "\x7F""ELF", 4, BEGINNING_OF_FILE, 0L);
    fs_print_error();

    // Test split writing (should fail)
    printf("Writing '\\x7F' at start and 'ELF' at offset 1...\n");
    write_file_at(f, "\x7F", 1, BEGINNING_OF_FILE, 0L);
    write_file_at(f, "ELF", 3, BEGINNING_OF_FILE, 1L);
    fs_print_error();

    // Close file
    printf("Closing file...\n");
    close_file(f);
    fs_print_error();

    return 0;
}
