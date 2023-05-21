#include <stdio.h>
#include <time.h>

#if defined(__linux__) || defined(__APPLE__)
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

float cpu_usage() {
    float cpu_usage = 0;

#if defined(__linux__) || defined(__APPLE__)
    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        printf("Failed to open /proc/stat\n");
        return 1;
    }

    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("Failed to read /proc/stat\n");
        fclose(file);
        return 1;
    }

    fclose(file);

    char cpuLabel[5];
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    sscanf(buffer, "%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", cpuLabel, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    unsigned long long total_idle = idle + iowait;
    unsigned long long total_non_idle = user + nice + system + irq + softirq + steal;
    unsigned long long total = total_idle + total_non_idle;

    cpu_usage = (double)total_non_idle / total * 100.0;
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


