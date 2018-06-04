#ifndef SYNCHRO_H
#define SYNCHRO_H


#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/fs.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <syslog.h>

#include "files.h"
#include "configuration.h"
#include "file.h"
#include "modificationdate.h"
#include "copyfile.h"
#include "time.h"


void startsynchronization(configuration conf);
files* preparelistfrompath(char *path, configuration conf);


#endif
