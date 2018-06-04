#include "time.h"




char* gettime()
{

time_t t = time(NULL);
struct tm tm = *localtime(&t);
int len = 1024;
char data[len];
snprintf(data,len,"%d-%d-%d %d:%d:%d: \n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
char *czas = malloc(sizeof(data));
strcpy(czas,data);
//printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
return czas;
}