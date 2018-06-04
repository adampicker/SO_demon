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
#include "file.h"
#include "files.h"
#include "synchro.h"
#include "configuration.h"
#include "time.h"



int dofork() {

syslog(LOG_INFO, "%s Forkowanie procesu",gettime());
    //forkowanie rodzica
    pid_t pid, sid;
    pid = fork();
    if(pid < 0){
        syslog(LOG_CRIT, "%s BŁĄD: nie udało się rozdzielić procesu.",gettime());
        exit(EXIT_FAILURE);
    }
    if(pid > 0){
        exit(EXIT_SUCCESS);
    }
    //zmiana maski plików
    umask(0);
    //tworzenie SIDa dla dziecka
    sid = setsid();
    if(sid < 0){
        syslog(LOG_CRIT, "%s BŁĄD: nie można utworzyć SID dla procesu potomnego",gettime());
        exit(EXIT_FAILURE);
    }
    //zmiana katalogu
    if((chdir("/")) < 0) {
        syslog(LOG_CRIT, "%s BŁĄD: nie można zmienić katalogu bieżącego",gettime());
    }
    //zamykanie STD
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}



void handler()
{
    syslog(LOG_INFO, "%s Wyslanie  SIGUSR1",gettime());
    printf("odebralem sygnal");

}
int sig()
{
    signal(SIGUSR1, handler);
    return 0;

}


int main( int argc, char *argv[])
{
    


    configuration *conf = malloc(sizeof(configuration));
    conf->sleep_time = 300; // DOMYSLNY CZAS SPANA DEMONA TO 5 MINUT
    conf->mmap_threshold = 50 *1024; // DOMYSLNY PROG MAPOWANIA PLIKOW TO 50KB
    conf->recursive_flag = false; // DOMYSLNIE DEMON INGORUJE KATALOGI
	char source_path[100];
	char destination_path[100];
	openlog("logs", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // var/log/syslog1



switch (argc)
{
	case 1:
		printf("nie wystarczajaca liczba argumentow\n");
		syslog(LOG_INFO, "Uzytkownik podal nie wystarczajaca liczbe arg.");
		return EXIT_FAILURE;
		break;
	case 2:
		strcpy(source_path,argv[1]);
		strcpy(destination_path , argv[2]);
        syslog(LOG_INFO, "Uzytkownik podal  wystarczajaca liczbe arg.");
		break;
	case 3:
    case 4:
    case 5:
    case 6:
        strcpy(source_path,argv[1]);
		strcpy(destination_path , argv[2]);
        setconfiguration(argc, argv, &conf);
        syslog(LOG_INFO, "Konfiguracja ustawiona");
        break;
	default:
		printf("Niewlasciwa liczba argumentow. Sprawdz pomoc -h\n");
		syslog(LOG_INFO, "Uzytkownik podal zla wystarczajaca liczbe arg.");
		return EXIT_FAILURE;
		break;
}

if (opendir(source_path) == NULL) 
{
    printf("Katalog zrodlowy nie istnieje\n");
    syslog(LOG_INFO, "%s Uzytkownik podal nieistniejacy katalog zrodlowy",gettime());
    return EXIT_FAILURE;
}
else syslog(LOG_INFO,"%s Katalog zrodlowy istnieje",gettime());
if (opendir(destination_path) == NULL) 
{
    printf("Katalog docelowy nie istnieje\n");
    syslog(LOG_INFO, "%s Uzytkownik podal nieistniejacy katalog docelowy",gettime());
    return EXIT_FAILURE;
}
else syslog(LOG_INFO,"%s Katalog docelowy istnieje",gettime());

conf->source_path = malloc(sizeof(source_path)+1);
conf->destination_path = malloc(sizeof(destination_path)+1);
strcpy(conf->source_path,source_path);
strcpy(conf->destination_path,destination_path);

dofork();


while (true)
{
    sleep(conf->sleep_time);
    signal(SIGUSR1,handler);
    syslog(LOG_INFO, "Zaczynam synchronizacje...");
    if (comparemodificationdate(conf->source_path,conf->destination_path))  // NIE TRZEBA SYNCHRONIZOWAC
    {
        syslog(LOG_INFO, "Synchronizacja nie byla konieczna, usypiam na %d sekund ", conf->sleep_time);
    }
    else
    {
        startsynchronization(*conf);
        setmodificationdate(conf->source_path,conf->destination_path);
        syslog(LOG_INFO, "Zakończono synchronizację, usypiam na %d sekund", conf->sleep_time);
    }
}



}

