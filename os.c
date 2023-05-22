
#ifdef _WIN32
#elif defined(__linux__) || defined(__APPLE__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "common.h"


// "alpine": "\uf300",
// "arch": "\uf303",
// "centos": "\uf304",
// "debian": "\uf306",
// "elementary": "\uf309",
// "fedora": "\uf30a",
// "gentoo": "\uf30d",
// "linux": "\ue712",
// "macos": "\ue711",
// "manjaro": "\uf312",
// "mint": "\uf30f",
// "opensuse": "\uf314",
// "raspbian": "\uf315",
// "ubuntu": "\uf31c",




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
            
            if (strcmp(*name, "alpine")) {
                *icon = "";
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

