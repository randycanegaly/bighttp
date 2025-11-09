/*
** showip.c
**
** show IP addresses for a host given on the command line
*/

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN]; // an array of chars with enough space for the
                                // largest ipv6 string, way big enough for an
                                // ipv4 address

  if (argc != 2) {
    fprintf(stderr, "usage: showip hostname\n");
    return 1;
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("IP addresses for %s:\n\n", argv[1]);

  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;
    struct sockaddr_in *ipv4; // sockaddr_in is the convenience struct to handle
                              // loading a sockaddr struct
    // a pointer to a sockaddr_in can be cast to a sockaddr and vice versa
    struct sockaddr_in6 *ipv6; // ipv6 version, same note ^^^^^^

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (p->ai_family == AF_INET) {             // IPv4
      ipv4 = (struct sockaddr_in *)p->ai_addr; // ai_addr is a sockaddr
      addr = &(ipv4->sin_addr); // sin_addr is an in_addr struct - the ipv4 IP
                                // address
      ipver = "IPv4";
    } else { // IPv6
      ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }

  freeaddrinfo(res); // free the linked list

  return 0;
}
