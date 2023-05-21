
#ifdef _WIN32
#elif defined(__linux__) || defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "common.h"



void os(char** name) {
#ifdef _WIN32
    *name = "Windows";
    return;
#elif defined(__linux__)
    FILE *file;
    char buffer[256];

    file = fopen("/etc/os-release", "r");
    if (file == NULL) {
        printf("Failed to open /etc/os-release\n");
        *name = "Unknown";
        return;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strncmp(buffer, "ID=", 3) == 0) {
            // i dont like this
            *name = malloc(256*sizeof(char));
            strcpy(*name, buffer+3);
            break;
        } 
    }
    fclose(file);
    return;

#elif defined(__APPLE__)
    *name = "MacOS";
    return;
#endif
    *name = "Unknown";
}

