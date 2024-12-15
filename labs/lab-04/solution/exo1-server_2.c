#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int server_1p ();

int main (int argc, char **argv) {
    
    if (argc <= 1 || strcmp(argv[1], "-1p") == 0) {
        server_1p();
    }

    return 0;
}

/******************** les fonctions de jeu **********************/

void game_1p (int sock) {
    srand(time(NULL) + sock);

    unsigned short int n = rand() % (1 << 16);
    printf("nb mystere pour partie socket %d = %d\n", sock, n);

    unsigned short int guess;
    int taille = 0;
    int tentatives = 20;
    int gagne = 0;
    char buff_in[100];

    while ((taille = recv(sock, buff_in, 100, 0)) > 0) {
        sscanf(buff_in, "%hu", &guess);
        printf("Joueur courant a envoyé: %d\n", guess);
        char reponse[20];
        
        if (n < guess || n > guess) {
            tentatives--;
        }

        if (tentatives == 0) {
            sprintf(reponse, "PERDU\n");
        } else if (n < guess) {
            sprintf(reponse, "MOINS %d\n", tentatives);
        } else if (n > guess) {
            sprintf(reponse, "PLUS %d\n", tentatives);
        } else {
            sprintf(reponse, "GAGNE\n");
            gagne = 1;
        }
        
        send(sock, reponse, strlen(reponse), 0);
        if (gagne || !tentatives) break;
    }

    printf("Fin de partie\n");

    close(sock);

}


int server_1p() {
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4242);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int r = bind (serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (r != 0) {
        fprintf(stderr, "Echec de bind\n");
        exit(-1);
    }

    r = listen(serv_sock, 0);
    if (r != 0) {
        fprintf(stderr, "Echec de liste\n");
        exit(-1);
    }

    int client_sock = 0;
    while (1) {
        client_sock = accept (serv_sock, NULL, NULL);
        if (client_sock < 0) {
            fprintf(stderr, "Echec de accept\n");
            exit(-1);
        } else {
            printf("Connewion acceptée, nouvelle partie lancée\n");
            game_1p(client_sock);
            close(client_sock);
        }
    }
    close(serv_sock);
    return 0;
}

