/*
 * SHhow output of stnard call to get IP address of running host
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/ip.h>

// true on error
static bool allIPAddr() {
    bool failed = false;
    struct ifaddrs *ifa, *ifap;

    if (getifaddrs(&ifa) != 0) {
        fprintf(stderr, "getifaddrs: %s\n", strerror(errno));
        return true;
    }

    for (ifap = ifa; ifap != NULL; ifap = ifap->ifa_next) {
        int family, addrlen, ret;
        char buf[256];

        /* Skip interfaces that have no configured addresses */
        if (ifap->ifa_addr == NULL)
            continue;

        /* Skip the loopback interface */
        if (ifap->ifa_flags & IFF_LOOPBACK)
            continue;

        /* Skip interfaces that are not UP */
        if (!(ifap->ifa_flags & IFF_UP))
            continue;

        /* Only handle IPv4 and IPv6 addresses */
        family = ifap->ifa_addr->sa_family;
        if (family != AF_INET && family != AF_INET6)
            continue;

        /* Skip IPv6 link-local addresses */
        if (family == AF_INET6) {
            struct sockaddr_in6 *sin6;

            sin6 = (struct sockaddr_in6 *)ifap->ifa_addr;
            if (IN6_IS_ADDR_LINKLOCAL(&sin6->sin6_addr) ||
                IN6_IS_ADDR_MC_LINKLOCAL(&sin6->sin6_addr))
                continue;
        }

        addrlen = (family == AF_INET) ? sizeof(struct sockaddr_in)
                                      : sizeof(struct sockaddr_in6);

        ret = getnameinfo(ifap->ifa_addr, addrlen, buf, sizeof(buf), NULL, 0,
                          NI_NUMERICHOST);

        /* Just skip addresses that cannot be translated */
        if (ret != 0) {
            if (ret != EAI_NONAME) {
                fprintf(stderr, "getnameinfo: %s", gai_strerror(ret));
                failed = true;
                break;
            }
        } else
            printf("%s\n", buf);
    }
    freeifaddrs(ifa);
    return failed;
}

int main(int argc, char **argv) {
    (void)argc, (void)argv;

    if (!allIPAddr())
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

