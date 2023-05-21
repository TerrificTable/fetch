#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/statvfs.h>
#endif

#include "common.h"


void disk(int* total, float* used, float* perc) {
#ifdef _WIN32
    ULARGE_INTEGER totalSpace, freeSpace, usedSpace;

    if (GetDiskFreeSpaceEx("C:", &usedSpace, &totalSpace, &freeSpace)) {
        double totalGB = totalSpace.QuadPart / (1024.0 * 1024 * 1024);
        double usedGB = totalGB - (freeSpace.QuadPart / (1024.0 * 1024 * 1024));

        *total = totalGB;
        *used = usedGB;
        *perc = (usedGB / totalGB) * 100;
    } else {
        *total = -1;
        *used = -1;
        *perc = -1;
    }
#elif defined(__linux__) || defined(__APPLE__)
    struct statvfs fsData;
    const char* mountPath = "/";  // Specify the mount path of the filesystem you want to check

    if (statvfs(mountPath, &fsData) == 0) {
        unsigned long long totalSpace = fsData.f_blocks * fsData.f_frsize;
        unsigned long long freeSpace = fsData.f_bfree * fsData.f_frsize;
        unsigned long long usedSpace = totalSpace - freeSpace;

        double totalGB = (double)totalSpace / (1024 * 1024 * 1024);
        double usedGB = (double)usedSpace / (1024 * 1024 * 1024);
        
        *total = totalGB;
        *used = usedGB;
        *perc = (usedGB / totalGB) * 100;
    } else {
        printf("Failed to retrieve disk space information.\n");
        *total = -1;
        *used = -1;
        *perc = -1;
    }
#endif
}

