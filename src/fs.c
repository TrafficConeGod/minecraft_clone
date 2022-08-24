#include "fs.h"
#include <errno.h>

error_t open_files(const char* mode, size_t num_files, const char* const paths[], struct stat stats[], FILE* files[]) {
    for (size_t i = 0; i < num_files; i++) {
        if (stat(paths[i], &stats[i]) != 0) {
            return errno;
        }
    }

    for (size_t i = 0; i < num_files; i++) {
        files[i] = fopen(paths[i], mode);
    }

    return 0;
}
void close_files(size_t num_files, FILE* const files[]) {
    for (size_t i = 0; i < num_files; i++) {
        fclose(files[i]);
    }
}