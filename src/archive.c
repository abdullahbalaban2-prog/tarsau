#include "tarsau.h"

int archive_files(int file_count, char *file_paths[], const char *output_file) {
    FileInfo file_info[MAX_FILES];
    off_t total_size = 0;
    
    if (!validate_files(file_count, file_paths, file_info, &total_size)) {
        return -1;
    }
    
    // Calculate organization section size
    // Format for each record: filename,permissions,size|
    char org_section[BUFFER_SIZE * 4] = "";
    int org_size = 0;
    
    for (int i = 0; i < file_count; i++) {
        char record[1024];
        int len = snprintf(record, sizeof(record), "%s,%04o,%lld|", 
                           file_info[i].basename, 
                           file_info[i].permissions, 
                           (long long)file_info[i].size);
        if (org_size + len >= (int)sizeof(org_section)) {
            fprintf(stderr, "Error: Organization section too large.\n");
            return -1;
        }
        strcat(org_section, record);
        org_size += len;
    }
    
    FILE *out = fopen(output_file, "wb");
    if (!out) {
        perror("fopen");
        return -1;
    }
    
    // Write the 10-byte organization section size
    char size_buf[11];
    snprintf(size_buf, sizeof(size_buf), "%010d", org_size);
    fwrite(size_buf, 1, 10, out);
    
    // Write organization section
    fwrite(org_section, 1, org_size, out);
    
    // Write file contents
    for (int i = 0; i < file_count; i++) {
        FILE *in = fopen(file_paths[i], "rb");
        if (!in) {
            perror("fopen");
            fclose(out);
            return -1;
        }
        
        unsigned char buffer[BUFFER_SIZE];
        size_t bytes_read;
        
        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, in)) > 0) {
            fwrite(buffer, 1, bytes_read, out);
        }
        
        fclose(in);
    }
    
    fclose(out);
    printf("Dosyalar birleştirildi.\n");
    return 0;
}
