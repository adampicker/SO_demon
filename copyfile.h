#ifndef COPYFILE_H
#define COPYFILE_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>\


#include <dirent.h>

int copy( char *sfile , char *dfile);
int map ( char *sfile , char *dfile);
int removefile( char *file);



#endif 
