
#ifdef _WIN32
#elif defined(__linux__) || defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "common.h"





void os(char** name, char** icon) {
#ifdef _WIN32
    *icon = "";
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
            
            if (strcmp(*name, "arch")) {
                *icon = "";
            } else if (strcmp(*name, "alpine")) {
                *icon = "";
            } else if (strcmp(*name, "ubuntu")) {
                *icon = "";
            } else if (strcmp(*name, "centos")) {
                *icon = "";
            } else if (strcmp(*name, "debian")) {
                *icon = "";
            } else if (strcmp(*name, "fedora")) {
                *icon = "";
            } else if (strcmp(*name, "manjaro")) {
                *icon = "";
            } else if (strcmp(*name, "gentoo")) {
                *icon = "";
            } else if (strcmp(*name, "linuxmint")) {
                *icon = "";
            } else if (strcmp(*name, "opensuse")) {
                *icon = "";
            } else if (strcmp(*name, "raspbian")) {
                *icon = "";
            } else {
                *icon = "";
            }

            break;
        } 
    }
    fclose(file);
    return;

#elif defined(__APPLE__)
    *icon = "";
    *name = "MacOS";
    return;
#endif
    *icon = "";
    *name = "Unknown";
}

