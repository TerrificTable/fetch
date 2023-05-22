#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "common.h"



char* l1; char* l2; char* l3; char* l4; char* l5;
char* ip; char* hostname;
char* osName; char* osIcon;

int mem_total;
float mem_used, mem_perc;

int disk_total;
float disk_used, disk_perc;



void ascii(char** l1, char** l2, char** l3, char** l4, char** l5);
void memory_usage(int* total, float* used, float* percentage);
void disk(int* total, float* used, float* perc);
void network(char** ip, char** hostname);
void os(char** name, char** icon);
float cpu_usage();


int main(unused int argc, unused char *argv[]) {
    ascii(&l1, &l2, &l3, &l4, &l5);
    memory_usage(&mem_total, &mem_used, &mem_perc);
    if (mem_total == -1 || mem_used == -1 || mem_perc == -1) {
        printf("Invalid memory values\n");
        return 1;
    }

    disk(&disk_total, &disk_used, &disk_perc);
    if (disk_total == -1 || disk_used == -1 || disk_perc == -1) {
        printf("Invalid disk values\n");
        return 1;
    }

    os(&osName, &osIcon);
    network(&ip, &hostname);


#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#else
    setlocale(LC_CTYPE, "");
#endif


    // very readable
    printf(" \033[38;5;241m%s  \033[38;5;105mcpu  \033[38;5;153m%.1f%%\n", l1, cpu_usage());
    printf(" \033[38;5;241m%s  \033[38;5;105mmem  \033[38;5;153m%.2f\033[37m/\033[38;5;153m%d\033[38;5;241mG \033[37m(\033[38;5;153m%.1f%%\033[37m)\n", l2, mem_used, mem_total, mem_perc);
    printf(" \033[38;5;241m%s  \033[38;5;105mdsk  \033[38;5;153m%.1f\033[37m/\033[38;5;153m%d\033[38;5;241mG \033[37m(\033[38;5;153m%.1f%%\033[37m)\n", l3, disk_used, disk_total, disk_perc);
    printf(" \033[38;5;241m%s  \033[38;5;105mip   \033[38;5;153m%s \033[37m(\033[38;5;153m%s\033[37m)\n", l4, ip, hostname);
    printf(" \033[38;5;241m%s  \033[38;5;105mos   \033[38;5;161m%s \033[38;5;153m%s\033[0m\n", l5, osIcon, osName);

    return 0;
}

