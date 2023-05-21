#include <math.h>
#include <stdio.h>
#include <time.h>

#include "common.h"



char* l1; char* l2; char* l3; char* l4; char* l5;
char* osName;

int mem_total;
float mem_used, mem_perc;

int disk_total;
float disk_used, disk_perc;



void ascii(char** l1, char** l2, char** l3, char** l4, char** l5);
void memory_usage(int* total, float* used, float* percentage);
void disk(int* total, float* used, float* perc);
void os(char** name);
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

    os(&osName);

    printf(" %s  cpu  %.1f%%\n", l1, cpu_usage());
    printf(" %s  mem  %.2f/%d gb (%.1f%%)\n", l2, mem_used, mem_total, mem_perc);
    printf(" %s  dsk  %.2f/%d gb (%.1f%%)\n", l3, disk_used, disk_total, disk_perc);
    printf(" %s  ip   todo\n", l4);
    printf(" %s  os   %s\n", l5, osName);

    return 0;
}

