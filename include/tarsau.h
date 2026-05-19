#ifndef TARSAU_H
#define TARSAU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_FILES 32
#define MAX_TOTAL_SIZE (200 * 1024 * 1024) // 200 MB
#define BUFFER_SIZE 4096

// Struct to hold file metadata for archiving
typedef struct {
    char path[1024];
    char basename[256];
    mode_t permissions;
    off_t size;
} FileInfo;

// Core functions
int archive_files(int file_count, char *file_paths[], const char *output_file);
int extract_archive(const char *archive_file, const char *target_dir);

// Validation functions
bool is_valid_ascii(const char *filepath);
bool validate_files(int file_count, char *file_paths[], FileInfo *file_info, off_t *total_size);

// Utility functions
const char* get_basename(const char *path);
int get_file_info(const char *filepath, FileInfo *info);
int create_directory_if_not_exists(const char *dir);
int is_directory(const char *path);

#endif // TARSAU_H
