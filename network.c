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
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 1
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif
#endif

#include "common.h"



void network(char** ip, char** out_hostname) {
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

    *out_hostname = malloc((strlen(hostname)+1) * sizeof(char));
    strcpy_s(*out_hostname, strlen(hostname)+1, hostname);


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
            if (strncmp(ipstr, "192.168.2", 9) == 0) {
                *ip = malloc(INET_ADDRSTRLEN * sizeof(char));
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
        return;
    }


    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
      *out_hostname = malloc((strlen(hostname)+1) * sizeof(char));
      strcpy(*out_hostname, hostname);
    } else {
        printf("Failed to get the hostname.\n");
    }
        

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                fprintf(stderr, "Failed to get IPv4 address: %s\n", strerror(s));
                return;
            }
          
            // i also dont like this
            if (strncmp(host, "192.168.2", 9) == 0) {
                *ip = malloc(NI_MAXHOST);
                strcpy(*ip, host);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
#endif
}

