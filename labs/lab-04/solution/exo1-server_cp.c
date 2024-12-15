#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAXLENGTH 256
#define SERVER_PORT 9000

void * server (void *args) {
    int *socketfd;
    char buff[MAXLENGTH];
    struct sockeaddr_in adrsock;

    if ((*socketfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket server issue\n");
        free(socketfd);
        exit(1);
    }
    
    memset(&adrsock, 0x0, sizeof(adrsock));
    adrsock.sin_family = AF_INET;
    adrsock.sin_port = htons(SERVER_PORT);
    adrsock.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(*socketfd, (struct sockeaddr_in *) &adrsock, sizeof(adrsock) )) < 0) {
        perror("Bind server issue\n");
        close(*socketfd);
        free(socketfd);
        exit(1);
    }

    if ((listen(*socketfd, 0)) < 0) {
        perror("Listen server issue\n");
        close(*socketfd);
        free(socketfd);
        exit(1);
    }

    return 0;
}


void * client (void *args) {
    int *connfd;
    char recvline[MAXLENGTH];
    struct sockaddr_in adrclient;


}



int main (void) {

    srand(time(NULL));
    int socketfd, n;
    struct sockaddr_in adrsock;
    char buff[MAXLENGTH];
    char recvline[MAXLENGTH];

    if ((socketfd = socket (PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket issue\n");
        exit(1);
    }

    memset(&adrsock, 0x0, sizeof(adrsock));
    adrsock.sin_family = AF_INET;
    adrsock.sin_port = htons(SERVER_PORT);
    adrsock.sin_addr.s_addr = htonl(INADDR_ANY);

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

    int random_int = rand() % 65535 + 0;

    for ( ; ; ) {

        struct sockaddr_in adrclient;
        memset (&adrclient, 0x0, sizeof(adrclient));
        socklen_t size_cli = sizeof(adrclient);

        printf("Listening on any address on port %d...\n",SERVER_PORT);
        
        int *connfd = malloc (sizeof(int));
        if ((*connfd = accept(socketfd, (struct sockaddr *) &adrclient, &size_cli)) < 0) {
            perror("Accept issue\n");
            close(socketfd);
            exit(1);
        }

        switch (fork()) {

            case -1 : break;
            case 0 :
                close (socketfd);
                int ret = communication (connfd);
                exit(1);

            case 1 :
                close (connfd);
                while(waitpid(-1, NULL, WNOHANG) > 0);
        }


        memset(buff, 0x0, MAXLENGTH);

        inet_ntop(AF_INET, &(adrclient.sin_addr), buff, sizeof(buff));
        printf("Connection from %s:%d\n", buff, ntohs(adrclient.sin_port));
        int i = 0;
        while (1) {
            memset(recvline, 0x0, MAXLENGTH);
            n = recv(connfd, recvline, MAXLENGTH, 0);
            if (n < 1) break;
            
            //Game rules here
            int number_input = atoi(recvline);

            if (i == 20) {
                send(connfd, "PERDU\n", 6, 0);
                break;
            }
            else if (number_input == random_int) {
                send(connfd, "GAGNE\n", 6, 0);
                break;
            } else if (number_input < random_int) {
                send(connfd, "PLUS\n", 5, 0);
            } else {
                send(connfd, "MOINS\n", 6, 0);
            }
            i++;
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