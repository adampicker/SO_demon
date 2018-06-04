#include "modificationdate.h"


void setmodificationdate(char *fullsourcepath, char *fulldestpath){
    struct stat st;
    struct utimbuf new_times;
    stat(fullsourcepath, &st);
    new_times.actime = st.st_atim.tv_sec;
    new_times.modtime = st.st_mtim.tv_sec;
    utime(fulldestpath, &new_times);
    chmod(fulldestpath, st.st_mode);
}

bool comparemodificationdate(char *fullsourcepath, char *fulldestpath){
    struct stat st1;
    struct stat st2;
    stat(fullsourcepath, &st1);
    stat(fulldestpath, &st2);
    return (st1.st_mtim.tv_sec == st2.st_mtim.tv_sec);
}