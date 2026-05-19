#include "tarsau.h"

int extract_archive(const char *archive_file, const char *target_dir) {
    FILE *in = fopen(archive_file, "rb");
    if (!in) {
        fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
        return -1;
    }
    
    char size_buf[11] = {0};
    if (fread(size_buf, 1, 10, in) != 10) {
        fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
        fclose(in);
        return -1;
    }
    
    int org_size = atoi(size_buf);
    if (org_size <= 0 || org_size > MAX_TOTAL_SIZE) {
        fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
        fclose(in);
        return -1;
    }
    
    char *org_section = malloc(org_size + 1);
    if (!org_section) {
        perror("malloc");
        fclose(in);
        return -1;
    }
    
    if (fread(org_section, 1, org_size, in) != (size_t)org_size) {
        fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
        free(org_section);
        fclose(in);
        return -1;
    }
    
    org_section[org_size] = '\0';
    
    if (create_directory_if_not_exists(target_dir) != 0) {
        fprintf(stderr, "Error creating target directory.\n");
        free(org_section);
        fclose(in);
        return -1;
    }
    
    FileInfo files[MAX_FILES];
    int file_count = 0;
    
    char *token = strtok(org_section, "|");
    while (token != NULL) {
        if (file_count >= MAX_FILES) {
            break;
        }
        
        // token format: filename,permissions,size (wait, if it starts with | it might be empty)
        if (strlen(token) == 0) {
            token = strtok(NULL, "|");
            continue;
        }
        
        char *comma1 = strchr(token, ',');
        if (!comma1) {
            fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
            free(org_section);
            fclose(in);
            return -1;
        }
        
        char *comma2 = strchr(comma1 + 1, ',');
        if (!comma2) {
            fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
            free(org_section);
            fclose(in);
            return -1;
        }
        
        *comma1 = '\0';
        *comma2 = '\0';
        
        strncpy(files[file_count].basename, get_basename(token), sizeof(files[file_count].basename) - 1);
        files[file_count].basename[sizeof(files[file_count].basename) - 1] = '\0';
        
        files[file_count].permissions = strtol(comma1 + 1, NULL, 8);
        files[file_count].size = strtoll(comma2 + 1, NULL, 10);
        
        file_count++;
        token = strtok(NULL, "|");
    }
    
    for (int i = 0; i < file_count; i++) {
        char out_path[1024];
        snprintf(out_path, sizeof(out_path), "%s/%s", target_dir, files[i].basename);
        
        FILE *out = fopen(out_path, "wb");
        if (!out) {
            perror("fopen");
            free(org_section);
            fclose(in);
            return -1;
        }
        
        off_t remaining = files[i].size;
        unsigned char buffer[BUFFER_SIZE];
        
        while (remaining > 0) {
            size_t to_read = (remaining > BUFFER_SIZE) ? BUFFER_SIZE : remaining;
            size_t bytes_read = fread(buffer, 1, to_read, in);
            if (bytes_read == 0) {
                break;
            }
            fwrite(buffer, 1, bytes_read, out);
            remaining -= bytes_read;
        }
        
        fclose(out);
        chmod(out_path, files[i].permissions);
    }
    
    free(org_section);
    fclose(in);
    
    if (strcmp(target_dir, ".") == 0) {
        printf("Mevcut dizinde dosyalar açıldı.\n");
    } else {
        printf("%s dizininde dosyalar açıldı.\n", target_dir);
    }
    return 0;
}
