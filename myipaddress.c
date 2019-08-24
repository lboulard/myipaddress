/*
 * SHhow output of stnard call to get IP address of running host
 */

#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// true on error
static bool dnsResolve(const char *hostname) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG;
    hints.ai_protocol = 0; /* Any protocol */

    // Look it up
    struct addrinfo *info = NULL, *ai;
    int s = getaddrinfo(hostname, NULL, &hints, &info);
    if (s) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return true;
    }

    bool failed = false;
    char *tmp = calloc(1, INET6_ADDRSTRLEN + 1);
    for (ai = info; ai != NULL; ai = ai->ai_next) {
        if (getnameinfo(ai->ai_addr, ai->ai_addrlen, tmp, INET6_ADDRSTRLEN + 1,
                        NULL, 0, NI_NUMERICHOST)) {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
            failed = true;
            break;
        }
        // We succeeded
        printf("%s\n", tmp);
    }
    free(tmp);
    if (info)
        freeaddrinfo(info);
    return failed;
}

// true on error
static bool myIpAddress() {
    char *hostname = calloc(1, 1023);

    if (!gethostname(hostname, 1023)) {
        return dnsResolve(hostname);
    } else {
        perror("gethostname");
    }
    return true;
}

int main(int argc, char **argv) {
    (void)argc, (void)argv;

    if (!myIpAddress())
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

