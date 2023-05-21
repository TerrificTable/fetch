
#ifdef _WIN32
#elif defined(__linux__) || defined(__APPLE__)
#endif

#include "common.h"



char* os() {
#ifdef _WIN32
    return "Windows";
#elif defined(__linux__)
    FILE *file;
    char buffer[256];

    file = fopen("/etc/os-release", "r");
    if (file == NULL) {
        printf("Failed to open /etc/os-release\n");
        return 1;
    }

    char* name;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strncmp(buffer, "PRETTY_NAME=", 12) == 0) {
            // Extract the distribution name from the line
            name = buffer + 13;
            break;
        }
    }

    fclose(file);

    return name;
#elif defined(__APPLE__)
    return "MacOS";
#endif
    return "Unknown";
}

