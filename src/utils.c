#include "tarsau.h"

// Returns the basename of a path (e.g. /path/to/file.txt -> file.txt)
const char* get_basename(const char *path) {
    const char *base = strrchr(path, '/');
    return base ? base + 1 : path;
}

// Retrieves file metadata (permissions and size)
int get_file_info(const char *filepath, FileInfo *info) {
    struct stat st;
    if (stat(filepath, &st) != 0) {
        perror("stat");
        return -1;
    }
    
    strncpy(info->path, filepath, sizeof(info->path) - 1);
    info->path[sizeof(info->path) - 1] = '\0';
    
    const char* base = get_basename(filepath);
    strncpy(info->basename, base, sizeof(info->basename) - 1);
    info->basename[sizeof(info->basename) - 1] = '\0';
    
    info->permissions = st.st_mode & 0777;
    info->size = st.st_size;
    
    return 0;
}

int is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return 0;
    }
    return S_ISDIR(st.st_mode);
}

// Creates a directory structure recursively if it doesn't exist
int create_directory_if_not_exists(const char *dir) {
    if (is_directory(dir)) {
        return 0;
    }
    
    char tmp[1024];
    strncpy(tmp, dir, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    
    char *p = NULL;
    size_t len = strlen(tmp);
    
    if(tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    
    for(p = tmp + 1; *p; p++)
        if(*p == '/') {
            *p = 0;
            if(mkdir(tmp, 0755) != 0 && errno != EEXIST) {
                return -1;
            }
            *p = '/';
        }
    
    if(mkdir(tmp, 0755) != 0 && errno != EEXIST) {
        return -1;
    }
    
    return 0;
}
