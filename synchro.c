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
#include "synchro.h"

void startsynchronization(configuration conf)
{
    syslog(LOG_INFO, "%s Tworzenie listy plikow katalogu zrodlowego",gettime());
    files *source_file_list = preparelistfrompath(conf.source_path,conf); ///PRZYGOTOWANIE LISTY PLIKOW W KATALOGU ZRODLOWYM
    show(source_file_list);
        syslog(LOG_INFO, "%s Tworzenie listy plikow katalogu docelowego",gettime());
    files *destination_file_list = preparelistfrompath(conf.destination_path,conf); // PRZYGOTOWANIE LISTY PLIKOW W KATALOGU DOCELOWYM
        show(destination_file_list);
    files *ptr1 = source_file_list;
    files *ptr2 = destination_file_list;
    bool flag = false;
    // USUWANIE PLIKOW KTORYCH NIE MA W KATALOGU ZRODLOWYM
    while (ptr2 != NULL)
    {
        ptr1 = source_file_list;
        flag = false;
        if( ( ptr2->filetype == DIRECTORY) && (!conf.recursive_flag) ) {
            ptr2 = ptr2->next;
            continue;
        }
        else
        {
            while (ptr1 != NULL)
            {
                if (strcmp(ptr2->filename,ptr1->filename) == 0)
                {
                    flag = true;
                    break;
                }
                ptr1 = ptr1->next;
            }
            if(!flag)
            {
                //PRZYGOTOWANIE FULL PATH
                int len = 1024;
                char result[len];
                snprintf(result,len,"%s%s",ptr2->path,ptr2->filename);
                if (ptr2->mmap) printf("  trzeba mapowac \n");
                else printf("  nie trzeba mapowac\n");
                if (getfiletype(result) == DIRECTORY)
                {
                    syslog(LOG_INFO, "%s Usuwanie katalogu ",gettime());
                    removefile(result);
                    rmdir(result);
                }
                else unlink(result);
                syslog(LOG_INFO, "%s Usuwanie pliku",gettime());
                removefilefromlist(destination_file_list, ptr2); //USUNIECIE Z LISty
                flag = false;
            }
        }
        ptr2 = ptr2->next;
    }
    //USUNIETE ZOSTALY PLIKI KTORE NIE POWTARZAJA SIE W KATALOGU ZRODLOWYM
    //TERAZ POROWNYWANY JEST SOURCE LIST Z DEST LIST 
    ptr1 = source_file_list;
    ptr2 = destination_file_list;
    flag = false;
                int lens = 1024;
            char results[lens];
    while (ptr1 != NULL )
    {
        flag = false;
            //to chyba nie ladnie ale mam dosc;
            snprintf(results,lens,"%s%s",conf.source_path,ptr1->filename);

        ptr2 = destination_file_list;
        int lend = 1024;
        char resultd[lend];
        while (ptr2 != NULL)
        {
            snprintf(resultd,lend,"%s%s",conf.destination_path,ptr2->filename);
            if (strcmp(ptr1->filename, ptr2->filename) == 0) // SZUKANIE PLIKU O TAKIEJ SAMEJ NAZWIE
            {
                flag = true; // ZNALAZL PLIK I SPRAWDZAL DATY MODYFIKACJ
                    if (comparemodificationdate(results, resultd))
                    {
                        syslog(LOG_INFO, "%s Pliki nie wymagaja kopiowania ",gettime());
                        //NIE TRZEBA KOPIOWAC
                    }
                    else
                    {   // TRZEBA KOPIOWAC I 2 OPCJE 1. ZWYKLY PLIK 2. KATALOG
                        if (getfiletype(results) == REGULAR_FILE)
                        {
                            syslog(LOG_INFO, "%s Kopiowanie plikow",gettime());
                            if (ptr2->mmap) map(results,resultd);
                            else copy(results,resultd);
                            setmodificationdate(results,resultd);
                        }
                        else if (getfiletype(results) == DIRECTORY)
                        {
                            syslog(LOG_INFO, "%s Rekurencyjnie kopiowanie katalogu ",gettime());
                            configuration *confR;
                            confR = malloc(sizeof(configuration));
                            confR->mmap_threshold = conf.mmap_threshold;
                            confR->recursive_flag = conf.recursive_flag;
                            confR->sleep_time = conf.sleep_time;
                            confR->source_path = malloc(sizeof(results)+1);
                            strcpy(confR->source_path,results);
                            confR->destination_path = malloc(sizeof(resultd)+1);
                            strcpy(confR->destination_path,resultd);
                            startsynchronization(*confR);
                            setmodificationdate(results,resultd);
                        }
                    }
                    break;
            }
            ptr2 = ptr2->next;
        }
        if (!flag) // NIE ZNALAZL PLIKU - STWORZYC NOWY/KATALOG PLIK I WRZUCIC TAM ZAWARTOSC
        {
            if (getfiletype(results) == REGULAR_FILE)
                {
                    syslog(LOG_INFO, "%s Tworzenie nieistniejacych plikow ",gettime());
                    int lenx = 1024;
                    char katalog[lenx];
                    strcpy(katalog,conf.destination_path);
                    strcat(katalog,ptr1->filename);
                    if (ptr1->mmap) map(results,katalog);
                    else copy(results,katalog);
                    setmodificationdate(results,katalog);
                }
                else if (getfiletype(results) == DIRECTORY)
                {
                    syslog(LOG_INFO, "%s Tworzenie nieistniejacych katalogow",gettime());
                    int lenx = 1024;
                    char katalog[lenx];
                    strcpy(katalog,conf.destination_path);
                    strcat(katalog,ptr1->filename);
                    //mkdir(katalog,S_IRWXU | S_IRWXG | S_IRWXO);
                    mkdir(katalog,0700);
                    // TO MOZNA W FUNCKJE ZAMKNAC
                    configuration *confR;                   
                    confR = malloc(sizeof(configuration));
                    confR->mmap_threshold = conf.mmap_threshold;
                    confR->recursive_flag = conf.recursive_flag;
                    confR->sleep_time = conf.sleep_time;
                    confR->source_path = malloc(sizeof(results)+1);
                    strcpy(confR->source_path,results);
                    confR->destination_path = malloc(sizeof(katalog)+1);
                    strcpy(confR->destination_path,katalog);
                    startsynchronization(*confR);
                    setmodificationdate(results,katalog);
                    flag = false;
                 }
            flag = false;
        }
        ptr1= ptr1->next;
    }
    return;
}

files* preparelistfrompath(char *path, configuration confx)
{
    files *list_of_files = NULL;
    DIR *dir;
    dir = opendir(path);
    struct dirent *ent;
    char buf[PATH_MAX+1];
    char siema[PATH_MAX+1];
    FILETYPE filetype;
    bool if_mmap;
    bool if_recursive;
    while ((ent = readdir (dir)) != NULL) {
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
        {
            continue;
        }
        realpath(ent->d_name,buf);
        int len = strlen(path) + strlen(buf) + 1;
        char result[len];
        snprintf(result,len,"%s%s",path,buf);
        filetype = getfiletype(result);


        if(ifmmap(result, confx.mmap_threshold)) if_mmap = true;
        else if_mmap = false;
        if_recursive = confx.recursive_flag;
        if (filetype == REGULAR_FILE) addfile(&list_of_files, buf, path, if_mmap, filetype);
        else if ( (if_recursive) && (filetype == DIRECTORY) ) addfile(&list_of_files, buf, path,if_mmap, filetype); // NALEZY ZMIENIC
    }
    closedir(dir);


return list_of_files;
}
