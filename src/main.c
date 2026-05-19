#include "tarsau.h"

void print_usage() {
    printf("Usage:\n");
    printf("  Archive: ./tarsau -b file1 [file2 ...] [-o output.sau]\n");
    printf("  Extract: ./tarsau -a archive.sau [target_dir]\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    if (strcmp(argv[1], "-b") == 0) {
        if (argc < 3) {
            print_usage();
            return 1;
        }
        
        char *input_files[MAX_FILES];
        int file_count = 0;
        const char *output_file = "a.sau";
        
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                if (i + 1 < argc) {
                    output_file = argv[i + 1];
                    break;
                } else {
                    fprintf(stderr, "Error: Missing output file after -o\n");
                    return 1;
                }
            } else {
                if (file_count < MAX_FILES) {
                    input_files[file_count++] = argv[i];
                } else {
                    fprintf(stderr, "Error: Maximum %d files allowed.\n", MAX_FILES);
                    return 1;
                }
            }
        }
        
        if (file_count == 0) {
            print_usage();
            return 1;
        }
        
        if (archive_files(file_count, input_files, output_file) != 0) {
            return 1;
        }
        
    } else if (strcmp(argv[1], "-a") == 0) {
        if (argc < 3 || argc > 4) {
            print_usage();
            return 1;
        }
        
        const char *archive_file = argv[2];
        const char *target_dir = ".";
        
        // Ensure archive_file ends with .sau
        size_t len = strlen(archive_file);
        if (len < 4 || strcmp(archive_file + len - 4, ".sau") != 0) {
            fprintf(stderr, "Arşiv dosyası uygunsuz veya bozuk!\n");
            return 1;
        }
        
        if (argc == 4) {
            target_dir = argv[3];
        }
        
        if (extract_archive(archive_file, target_dir) != 0) {
            return 1;
        }
        
    } else {
        print_usage();
        return 1;
    }
    
    return 0;
}
