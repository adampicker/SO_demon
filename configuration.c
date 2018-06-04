#include "configuration.h"


void setconfiguration(int argc, char *argv[], configuration **conf)
{
    int i = 3;

    int len;
    for (i; i<argc ;i++)
    {
        char *argument = argv[i];

        switch (argument[1])
        {
            case 'R':
            (*conf)->recursive_flag = true;
            break;
            case 's':
            len = strlen(argument);
            int seconds = 0;
            for (int j=2; j<len-1;j++)
            {
                seconds *=10;
                seconds += (argument[j] - '0');                
            }
            switch(argument[len-1])
            {
                case 's':

                break;
                case 'm':
                seconds = seconds*60;
                break;
                case 'h':
                seconds = seconds*3600;
                break;
                default:
                printf("Nie dozwolony znak sprawdz.\n");
                break;
            }
            (*conf)->sleep_time = seconds;
            break;

            case 't':
            len  = strlen(argument);
            int size = 0;
            int j;
            for (j=2; j<len-1;j++)
            {
                if(argument[j] >= 48 && argument[j] <= 57) 
                {
                size = size *10;
                size += (argument[j] - '0');
                }
                else break;                
            }
            switch (argument[j])
            {
                case 'b':
                size = size;
                break;
                case 'k':
                size = size*1024;
                break;
                case 'm':
                size = size*1024*1024;
                break;
                case 'g':
                size = size*1024*1024*1024;
                break;
                default:
                printf("Nie dozwolony znak.\n");
                break;
            }
            (*conf)->mmap_threshold = size;
            break;

            default:
            printf("Nie dozwolony znak sprawdz\n");
            break;
        }
    }
}

configuration* duplicateconfiguration(char *source_path, char *destination_path, configuration conf)
{
    configuration *confR;
    confR = malloc(sizeof(configuration));
    confR->mmap_threshold = conf.mmap_threshold;
    confR->recursive_flag = conf.recursive_flag;
    confR->sleep_time = conf.sleep_time;
    confR->source_path = malloc(sizeof(source_path)+1);
    strcpy(confR->source_path,source_path);
    confR->destination_path = malloc(sizeof(destination_path)+1);
    strcpy(confR->destination_path,destination_path);
    return confR;       
}

