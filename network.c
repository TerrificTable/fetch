#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <ws2ipdef.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "common.h"



void network(unused char** ip) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize winsock.\n");
        return;
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        fprintf(stderr, "Failed to get hostname.\n");
        WSACleanup();
        return;
    }


    struct addrinfo hints, *res, *p;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "Failed to get host information.\n");
        WSACleanup();
        return;
    }

    char ipstr[INET6_ADDRSTRLEN];

    for (p = res; p != NULL; p = p->ai_next) {
        void *addr;
        
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            inet_ntop(AF_INET, addr, ipstr, sizeof(ipstr));
         
            // I do not like this
            if (strncmp(ipstr, "192.168.2", strlen("192.168.2")) == 0) {
                *ip = malloc(INET_ADDRSTRLEN);
                strcpy_s(*ip, strlen(ipstr)+1, ipstr);
                break;
            }
        }
    }

    WSACleanup();
#elif defined(__linux__) || defined(__APPLE__)
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("Failed to get interface addresses");
        return 1;
    }

    // Traverse the linked list of network interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        // Filter for IPv4 addresses
        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                fprintf(stderr, "Failed to get IPv4 address: %s\n", gai_strerror(s));
                return 1;
            }
            printf("Interface: %s\t IPv4 Address: %s\n", ifa->ifa_name, host);
        }
    }

    freeifaddrs(ifaddr);
#endif
}

