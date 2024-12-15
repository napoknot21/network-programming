#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAXLENGTH 256
#define SERVER_PORT 9000

int main (void) {

    int socketfd, connfd, n;
    struct sockaddr_in6 adrsock;
    char buff[MAXLENGTH];
    char recvline[MAXLENGTH];

    if ((socketfd = socket (PF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("Socket issue\n");
        exit(1);
    }

    memset(&adrsock, 0x0, sizeof(adrsock));
    adrsock.sin6_family = AF_INET6;
    adrsock.sin6_port = htons(SERVER_PORT);
    adrsock.sin6_addr = in6addr_any;

    if ((bind(socketfd, (struct sockaddr *) &adrsock, sizeof(adrsock) )) < 0) {
        perror("Bind issue\n");
        close(socketfd);
        exit(1);
    }

    if ((listen(socketfd, 0)) < 0) {
        perror("Listen issue\n");
        close(socketfd);
        exit(1);
    }


    for ( ; ; ) {

        struct sockaddr_in6 adrclient;
        memset (&adrclient, 0x0, sizeof(adrclient));
        socklen_t size_cli = sizeof(adrclient);

        printf("Listening on any address on port %d...\n",SERVER_PORT);

        if ((connfd = accept(socketfd, (struct sockaddr *) &adrclient, &size_cli)) < 0) {
            perror("Accept issue\n");
            close(socketfd);
            exit(1);
        }
        memset(buff, 0x0, MAXLENGTH);

        inet_ntop(AF_INET6, &(adrclient.sin6_addr), buff, sizeof(buff));
        printf("Connection from %s:%d\n", buff, ntohs(adrclient.sin6_port));

        while (1) {
            memset(recvline, 0x0, MAXLENGTH);
            n = recv(connfd, recvline, MAXLENGTH, 0);
            if (n < 1) break;
            printf ("%s", recvline);

        }

        if (n < 1) {
            perror("Bye !\n");
            close (connfd);
            close (socketfd);
        }
        break;
    }

    return 0;

}
