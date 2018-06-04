#ifndef FILE_H
#define FILE_H
#include <stdbool.h>
#include <sys/stat.h>


typedef enum {
    SYMBOLIC_LINK
    , DIRECTORY
    , CHARACTER_DEVICE
    , BLOCK_DEVICE
    , FIFO
    , SOCKET
    , REGULAR_FILE
    , UNKNOWN
    } FILETYPE;

FILETYPE getfiletype(const char* path);

bool ifmmap(char *path, size_t mmap_threshhold);

#endif

