# Safe System Calls (Programming Assignment 2)

This project implements **safe file I/O system calls** in C: Operating Systems, Programming Assignment #2.

## 📂 Project Structure
- **fileio.c** – Implementation of system calls (`open_file`, `close_file`, `read_file_from`, `write_file_at`)
- **fileio.h** – Header definitions for system call APIs【42†source】
- **readwrite.c** – Test program to exercise the system calls【40†source】
- **important.dat** – Example data file created/used during testing

## 📌 Assignment Overview
System calls provide a controlled interface between user programs and the OS.  
This assignment enhances file I/O system calls by ensuring **safety against ELF injection**:

- Prevent the byte sequence `\x7FELF` from appearing in the **first four bytes** of any file.
- Maintain correct error handling through the global variable `fserror` and `fs_print_error()`.

### System Calls Implemented
- `File open_file(char *name);`  
- `void close_file(File file);`  
- `unsigned long read_file_from(File file, void *data, unsigned long num_bytes, SeekAnchor start, long offset);`  
- `unsigned long write_file_at(File file, void *data, unsigned long num_bytes, SeekAnchor start, long offset);`  
- `void fs_print_error(void);`  

### Error Codes
- `NONE` – No error  
- `OPEN_FAILED`, `CLOSE_FAILED`, `READ_FAILED`, `WRITE_FAILED`  
- `ILLEGAL_ELF` – Detected attempt to insert `\x7FELF` at file start【41†source】  

## ⚙️ Compilation
Compile the library with a test program (e.g., `readwrite.c`):

```bash
gcc -o readwrite readwrite.c fileio.c
```

## ▶️ Running
Run the test program:

```bash
./readwrite
```

### Example Output
```
Creating new file called "important.dat"...
FS ERROR: NONE
Writing something benign to beginning of file...
FS ERROR: NONE
Writing something evil to beginning of file...
FS ERROR: ILLEGAL_ELF: SHAME ON YOU!
Closing file...
FS ERROR: NONE
```【39†source】

## 📝 Notes
- The ELF check applies across multiple writes (e.g., partial writes like `\x7F` + `ELF` are also blocked).  
- Efficiency matters: do not read entire files unnecessarily; only check the first four bytes when required.  
- No thread-safety required for this assignment.  

## 👥 Authors
- Original framework: Provided by Prof. Ali-Gombe 
- Implementation & tests: Carter Mauer  
