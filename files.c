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
#include <stdbool.h>
#include "file.h"


void addfile(files **elemfirst, char *name,char *path, bool mmap, FILETYPE type)
{
    files *ptr = *elemfirst;//wsk -wskaznik na wskiaznik

    if (ptr == NULL)
    {
        ptr = malloc(sizeof(files));
        ptr->filename = malloc(strlen(name)+1);
        ptr->path = malloc(strlen(path)+1);
        strcpy(ptr->filename,name);
        strcpy(ptr->path,path);
        ptr->mmap = mmap;
        ptr->filetype = type;
        ptr->next = NULL;
        *elemfirst = ptr;
    }
    else
    {
        files *ptr1 = ptr;
        while (ptr1->next != NULL)
        {
            ptr1 = ptr1->next;
        }
        files *new_element = malloc(sizeof(files));
        new_element->filename = malloc(strlen(name)+1);
        new_element->path = malloc(strlen(path)+1);
        strcpy(new_element->filename,name);
        strcpy(new_element->path,path);
        new_element->mmap = mmap;
        new_element->filetype = type;
        new_element->next = NULL;
        ptr1->next = new_element;
    }
}

void show(files *wskdolisty) // tylko do testow
{
    files *wsk = wskdolisty;
    while (wsk != NULL)
    {
        printf("%s%s\n",wsk->path,wsk->filename);
        wsk = wsk->next;
    }
    free(wsk);
    return;
}

void removefilefromlist(files *wskdolisty, files *to_remove)
{
    files *ptr;
    files *ptr2;
    //WARIANT USUWANIA 1 ELEMENTU
    if (wskdolisty == to_remove)
    {
        wskdolisty = wskdolisty->next;
        // potem mozna dac ajkies free
        return;
    }
   else
   {
       ptr = wskdolisty;
       while (ptr->next != to_remove) ptr = ptr->next;
       if (to_remove->next == NULL) ptr->next = NULL;
       else
       {
           ptr2 = to_remove->next;
           ptr->next = ptr2;
       }

   }
}




/*void removeall(files *first){

    if(first == NULL) return;
    files *ptr = first;
    while(ptr->next != NULL){
        ptr = ptr->next; // idziemy do ostatniego
    }
    while (ptr != first)
    {
        file  *prev = first;
        first = first->next;
        free(prev->filename);
        free(prev->path);
        free(prev);
    }
    }
    free(first);
}*/
