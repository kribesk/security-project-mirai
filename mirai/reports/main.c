#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdint.h>

typedef uint32_t ipv4_t;
typedef uint16_t port_t;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void read_field(int sock, void * data, size_t size) {
    int n = read(sock, data, size);
    if (n < 0)
      error("ERROR while reading from socket");
    if (n != size)
      error("ERROR size mismatch");
}

void read_str(int sock, char * buffer) {
    uint8_t len;
    read_field(sock, &len, sizeof(uint8_t));
    int n = read(sock, buffer, len);
    if (n < 0)
      error("ERROR while reading from socket");
    if (n != len)
      error("ERROR size mismatch");
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
         error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(48101);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    if (argc > 0) {
//      printf("Using initial file: %s\n", argv[1]);
      FILE * fd = fopen(argv[1], "r");
      if (fd == NULL)
        error("File does not exist");
      else {
        char * line = NULL;
        size_t len = 0;
        size_t read;
        while ((read = getline(&line, &len, fd)) != -1) {
          printf("%s\n", line); 
          fflush(stdout);
        }
        if (line)
          free(line);
      }
      fclose(fd);
    }


//    printf("Listening on %s:%d...\n", inet_ntoa(serv_addr.sin_addr), htons(serv_addr.sin_port));
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    int pid;
    while (1) {

         if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0)
             error("ERROR on accept");

         if ((pid = fork()) < 0)
             error("ERROR in new process creation");

         if (pid == 0) {
            // close(sockfd);

            uint8_t zero;
            ipv4_t address;
            port_t port;
            char username[256];
            char password[256];

            read_field(newsockfd, &zero, sizeof(uint8_t));
            if (zero != 0)
              error("ERROR protocol");

            read_field(newsockfd, &address, sizeof(ipv4_t));
            read_field(newsockfd, &port, sizeof(port_t));
            read_str(newsockfd, username);
            read_str(newsockfd, password);

            struct in_addr printable_address;
            printable_address.s_addr = address;
            printf("%s:%d %s:%s\n", inet_ntoa(printable_address), port, username, password);
            fflush(stdout);
            close(newsockfd);

          } else {

             close(newsockfd);

          }
    }

    return 0;
}