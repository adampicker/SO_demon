#include <dirent.h> //katalogi
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h> //close
#include <fcntl.h> //open, read
#include <sys/mman.h> //mmap
#include <syslog.h>


#include "file.h"


FILETYPE getfiletype(const char* path){
    struct stat st;
    stat(path, &st);
    if(S_ISLNK(st.st_mode)) return SYMBOLIC_LINK;
    else if(S_ISDIR(st.st_mode)) return DIRECTORY;
    else if(S_ISCHR(st.st_mode)) return CHARACTER_DEVICE;
    else if(S_ISBLK(st.st_mode)) return BLOCK_DEVICE;
    else if(S_ISFIFO(st.st_mode))return FIFO;
    else if(S_ISSOCK(st.st_mode))return SOCKET;
    else if(S_ISREG(st.st_mode)) return REGULAR_FILE;
    else return UNKNOWN;
}

bool ifmmap(char *path, size_t mmap_threshold)
{
    struct stat st;
    stat(path, &st);

    if (st.st_size >= mmap_threshold) return true;
    else return false;
}




