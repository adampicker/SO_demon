#ifndef FILES_H
#define FILES_H
#include <stdbool.h>
#include <stdlib.h>


#include "file.h"

typedef struct files files;


struct files
{
    char *filename;
    char *path;
    FILETYPE filetype;
    bool mmap;
    char *full_path;
    files *next;
};

void addfile(files **elemfirst, char *name, char* path, bool mmap, FILETYPE type);
void show(files *wskdolisty);
void removefilefromlist(files *wskdolisty, files *to_remove);


 #endif
