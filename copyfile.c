#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <assert.h>
#include <dirent.h>



int copy( char *sfile , char *dfile)
{
    int fdo,fdn;
    char bufor[2048];

    fdo=open(sfile,O_RDONLY);
	if(fdo==-1)
    	{
        return 1;
    	}
    fdn = creat(dfile,0666);
	 if(fdn==-1)
    {
        return 1;
    }
    int licznik;
    while ((licznik=read(fdo,bufor, sizeof(bufor)))>0)
        write(fdn,bufor,licznik);

    close(fdn);
    close(fdo);


    return 0;
}

int map ( char *sfile , char *dfile)
{
    int fs, fd;
    struct stat st;
    void *source, *destination;

    fs = open(sfile, O_RDONLY);
    if (fs == -1) {
        perror("open source");
        return 1;
    }
    fd = open(dfile, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open destination");
        return 1;
    }
    if (fstat(fs, &st)) {
        perror("fstat");
        return 1;
    }
    if (ftruncate(fd, st.st_size)) {
        perror("truncate fd");
        return 1;
    }
    source = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fs, 0);

    if (source == MAP_FAILED) {
        perror("mmap source");
        return 1;
    }
    destination = mmap(0, st.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (destination == MAP_FAILED) {
        perror("mmap destintation");
        return 1;
    }
    memmove(destination, source, st.st_size);
    return 0;

}




int removefile( char * dirpath)
{
    struct dirent *entry;
    DIR *dir;
    dir = opendir(dirpath);
    if (dir == NULL)
        return -1;
    while ((entry = readdir(dir)) != NULL) {
        int len = sizeof(dirpath) + sizeof(entry->d_name) + 1;
        char path[len];

        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
        {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
           unlink(path);

        }else if (entry->d_type == DT_DIR ) {
         removefile(path);
            rmdir(path);


    }}
    closedir(dir);
    return 0;
}

