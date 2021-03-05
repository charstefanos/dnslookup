#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFLEN 40

int
main(int argc, char *argv[])
{

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
    struct addrinfo* addrinfo_list;
    int error;

    if ((error = getaddrinfo(argv[i], "80", &hints, &addrinfo_list)) != 0) {
      printf("Unable to look up IP address of [%s]: %s",argv[i], gai_strerror(error)); 
      return 2;   
    }

    struct addrinfo* curr_addr;
    for (curr_addr = addrinfo_list; curr_addr != NULL; curr_addr = curr_addr->ai_next) {
      char buffer[BUFLEN] = {0};

      void * bin_addr = curr_addr->ai_family == AF_INET6 
        ? (void *)&((struct sockaddr_in6*)curr_addr->ai_addr)->sin6_addr
          : (void *)&((struct sockaddr_in*)curr_addr->ai_addr)->sin_addr;
      
      if (inet_ntop(curr_addr->ai_family, bin_addr, buffer, sizeof(buffer)) == NULL) {
        printf("Unable to convert IP address to string");
        return 3;

      }
      printf("%s %s %s\n", argv[i], curr_addr->ai_family == AF_INET6 ? "IPv6" : "IPv4", buffer);


    }


  }

  return 0;


}

