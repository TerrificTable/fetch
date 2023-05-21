#include <math.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <stdio.h>
#endif

#include "common.h"

void memory_usage(unused int* total, unused float* used, unused float* perc) {
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);

    if (GlobalMemoryStatusEx(&status)) {
        DWORDLONG totalMemory = status.ullTotalPhys;
        DWORDLONG usedMemory = status.ullTotalPhys - status.ullAvailPhys;

        double totalMemoryGB = (double)totalMemory / (1024 * 1024 * 1024);
        double usedMemoryGB = (double)usedMemory / (1024 * 1024 * 1024);

        *total = (int)ceil(totalMemoryGB);
        *used = usedMemoryGB;
        *perc = (usedMemoryGB / totalMemoryGB) * 100;
    } else {
        *total = -1;
        *used = -1;
        *perc = -1;
    }
#elif defined(__linux__) || defined(__APPLE__)
    FILE* file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        printf("Failed to open %s\n", "/proc/meminfo");
        *total = -1;
        *used = -1;
        *perc = -1;
        return;
    }

    unsigned long long totalMemory = 0;
    unsigned long long freeMemory = 0;
    unsigned long long buffersMemory = 0;
    unsigned long long cachedMemory = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "MemTotal: %llu kB", &totalMemory) == 1) {
            totalMemory *= 1024; // Convert from kilobytes to bytes
        } else if (sscanf(line, "MemFree: %llu kB", &freeMemory) == 1) {
            freeMemory *= 1024;
        } else if (sscanf(line, "Buffers: %llu kB", &buffersMemory) == 1) {
            buffersMemory *= 1024;
        } else if (sscanf(line, "Cached: %llu kB", &cachedMemory) == 1) {
            cachedMemory *= 1024;
        }
    }

    fclose(file);

    unsigned long long usedMemory = totalMemory - freeMemory - buffersMemory - cachedMemory;

    double totalMemoryGB = (double)totalMemory / (1024 * 1024 * 1024);
    double usedMemoryGB = (double)usedMemory / (1024 * 1024 * 1024);

    *total = totalMemoryGB;
    *used = usedMemoryGB;
    *perc = (usedMemoryGB / totalMemoryGB) * 100;
#endif
}

