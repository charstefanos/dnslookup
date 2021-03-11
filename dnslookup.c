#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>

#define BUFLEN 40

int main(int argc, char *argv[]) {

  if (argc <= 1){
    printf("No domain(s) specified.\n");
    return 1;
  }

  struct addrinfo hints;

  // Look up the IP address of the hostname specified on the command line
  memset(&hints, 0, sizeof(hints));
  hints.ai_family    = AF_UNSPEC;
  hints.ai_socktype  = SOCK_STREAM;

  for (int i = 1; i < argc; i++){
    struct addrinfo *res, *rp;
    int error;

    /* Converts DNS or IPs into dynamically allocated linked list 
    of struct addrinfo structures. If can't be converted throws an error */
    if ((error = getaddrinfo(argv[i], "80", &hints, &res)) != 0) {
      printf("Unable to look up IP address of [%s]: %s",argv[i], gai_strerror(error)); 
      return 2;   
    }

    /* Loops through all the items in the linked list  */
    for (rp = res; rp != NULL; rp = rp->ai_next) {
      char dst_buffer[BUFLEN] = {0};

      /* Identifies if its either IPv4 or IPv6 address */
      void * src_addr = rp->ai_family == AF_INET6 
        ? (void *)&((struct sockaddr_in6*)rp->ai_addr)->sin6_addr
          : (void *)&((struct sockaddr_in*)rp->ai_addr)->sin_addr;
      
      /* Converts IPv4 and IPv6 addresses from binary to text form */
      if (inet_ntop(rp->ai_family, src_addr, dst_buffer, sizeof(dst_buffer)) == NULL) {
        printf("Unable to convert IP address to string");
        return 3;

      }
      printf("%s %s %s\n", argv[i], rp->ai_family == AF_INET6 ? "IPv6" : "IPv4", dst_buffer);

    }

  }

  return 0;

}

