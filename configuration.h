#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdio.h>
#include <string.h>
#include "files.h"
#include "configuration.h"

typedef struct configuration configuration;

struct configuration
{
    char *source_path;
    char *destination_path;
    bool recursive_flag;
    int sleep_time;
    size_t mmap_threshold;
};
void setconfiguration(int argc, char *argv[], configuration **conf);
configuration* duplicateconfiguration(char *source_path, char *destination_path, configuration conf);
#endif