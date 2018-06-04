#ifndef MODIFICATIONDATE_H
#define MODIFICATIONDATE_H
#include <stdbool.h>
#include <sys/stat.h>
#include <utime.h>


void setmodificationdate(char *fullsourcepath, char *fulldestpath);

bool comparemodificationdate(char *fullsourcepath, char *fulldestpath);





#endif