#include <assert.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <debug52.h>

#include "onegin.h"

int StatIsFileExists (const char* filename) {
    struct stat buffer = {};

    return (stat (filename, &buffer) == 0);
}

size_t StatFileSize (int fd) {
    assert (fd != -1);

    struct stat file_stat = {};
    fstat (fd, &file_stat);

    return (size_t)file_stat.st_size;
}

ssize_t ReadFileToBuf (int fd, char** buffer_ptr, size_t buffer_size) {
    assert (*buffer_ptr != 0);
    assert (buffer_size != 0);

    char* buffer = *buffer_ptr;

    ssize_t buffer_size_r = read (fd, buffer, buffer_size);

    return buffer_size_r;
}

size_t ReadFileAndRealloc (int fd, char** buffer_ptr, size_t orig_buffer_size) {
    assert (*buffer_ptr != 0);
    assert (orig_buffer_size != 0);

    char* buffer = *buffer_ptr;

    ssize_t buffer_size_s = ReadFileToBuf (fd, buffer_ptr, orig_buffer_size);

    assert (buffer_size_s > 0);

    size_t buffer_size = (size_t)buffer_size_s;

    *buffer_ptr = (char*)realloc (buffer, buffer_size + 1);
    assert (*buffer_ptr != 0);
    (*buffer_ptr)[buffer_size] = '\0';

    return buffer_size;
}