#include "fs.h"
#include <errno.h>

error_t open_files(size_t num_files, const char* const paths[], const char* const modes[], struct stat stats[], FILE* files[]) {
    for (size_t i = 0; i < num_files; i++) {
        if (stat(paths[i], &stats[i]) != 0) {
            return errno;
        }
    }

    for (size_t i = 0; i < num_files; i++) {
        files[i] = fopen(paths[i], modes[i]);
    }

    return 0;
}
void close_files(size_t num_files, FILE* const files[]) {
    for (size_t i = 0; i < num_files; i++) {
        fclose(files[i]);
    }
}