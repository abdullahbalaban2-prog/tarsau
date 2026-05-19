#include "tarsau.h"

// Check if file contains only ASCII characters
bool is_valid_ascii(const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        return false; // Error opening file
    }
    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, f)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] > 127) {
                fclose(f);
                return false;
            }
        }
    }
    
    fclose(f);
    return true;
}

// Validates the list of files, sizes, limits and ASCII format
bool validate_files(int file_count, char *file_paths[], FileInfo *file_info, off_t *total_size) {
    if (file_count > MAX_FILES) {
        fprintf(stderr, "Error: Maximum %d files allowed.\n", MAX_FILES);
        return false;
    }
    
    *total_size = 0;
    
    for (int i = 0; i < file_count; i++) {
        if (!is_valid_ascii(file_paths[i])) {
            fprintf(stderr, "%s giriş dosyasının formatı uyumsuzdur!\n", file_paths[i]);
            return false;
        }
        
        if (get_file_info(file_paths[i], &file_info[i]) != 0) {
            fprintf(stderr, "Error: Cannot get info for %s.\n", file_paths[i]);
            return false;
        }
        
        *total_size += file_info[i].size;
        if (*total_size > MAX_TOTAL_SIZE) {
            fprintf(stderr, "Error: Total size exceeds 200 MB limit.\n");
            return false;
        }
    }
    
    return true;
}
