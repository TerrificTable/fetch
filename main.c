#include <stdio.h>
#include <time.h>

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#endif

#define unused __attribute__((unused))



float cpu_usage();



int main(unused int argc, unused char *argv[]) {
    printf("Cpu Usage: %.1f%%\n", cpu_usage());

    return 0;
}


#if defined(__linux__) || defined(__APPLE__)
unsigned long long getCPUIdleTime() {
    FILE* fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        return 0;
    }

    char cpuLabel[5];
    unsigned long long user, nice, system, idle;
    fscanf(fp, "%s %llu %llu %llu %llu", cpuLabel, &user, &nice, &system, &idle);

    fclose(fp);

    return idle;
}
#endif


float cpu_usage() {
    float cpu_usage = 0;

#if defined(__linux__) || defined(__APPLE__)
    unsigned long long idleTimeStart = getCPUIdleTime();
    sleep(1);
    unsigned long long idleTimeEnd = getCPUIdleTime();
    
    unsigned long long idleTicks = idleTimeEnd - idleTimeStart;
    unsigned long long totalTicks = sysconf(_SC_CLK_TCK);

    cpu_usage = ((double)idleTicks) / totalTicks;
#elif _WIN32
    FILETIME idleTime, kernelTime, userTime;
    ULARGE_INTEGER idleTimeStart, idleTimeEnd;
    ULARGE_INTEGER kernelTimeStart, kernelTimeEnd;
    ULARGE_INTEGER userTimeStart, userTimeEnd;
    
    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    idleTimeStart.LowPart = idleTime.dwLowDateTime;
    idleTimeStart.HighPart = idleTime.dwHighDateTime;
    kernelTimeStart.LowPart = kernelTime.dwLowDateTime;
    kernelTimeStart.HighPart = kernelTime.dwHighDateTime;
    userTimeStart.LowPart = userTime.dwLowDateTime;
    userTimeStart.HighPart = userTime.dwHighDateTime;
    
    Sleep(1000);

    GetSystemTimes(&idleTime, &kernelTime, &userTime);
    idleTimeEnd.LowPart = idleTime.dwLowDateTime;
    idleTimeEnd.HighPart = idleTime.dwHighDateTime;
    kernelTimeEnd.LowPart = kernelTime.dwLowDateTime;
    kernelTimeEnd.HighPart = kernelTime.dwHighDateTime;
    userTimeEnd.LowPart = userTime.dwLowDateTime;
    userTimeEnd.HighPart = userTime.dwHighDateTime;
    
    ULONGLONG idleTicks = idleTimeEnd.QuadPart - idleTimeStart.QuadPart;
    ULONGLONG kernelTicks = kernelTimeEnd.QuadPart - kernelTimeStart.QuadPart;
    ULONGLONG userTicks = userTimeEnd.QuadPart - userTimeStart.QuadPart;
    ULONGLONG totalTicks = kernelTicks + userTicks;
    
    cpu_usage = (1.0 - ((float)idleTicks) / totalTicks) * 100.0;
#endif

    return cpu_usage;
}


