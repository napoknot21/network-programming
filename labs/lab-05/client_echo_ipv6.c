#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE_MSG 100

#define PORT 4242
// adresse pour locomotive
#define ADDR "fdc7:9dd5:2c66:be86:7a45:c4ff:fe08:2118"

int main(int argc, char* argv[]) {
  struct sockaddr_in6 address_sock;
  struct in6_addr address;
  int socket_fd, ret_val;

  char buf[SIZE_MSG+1], msg[SIZE_MSG+1];
  int size_recv;

  // Creation socket
  address_sock.sin6_family = AF_INET6;
  address_sock.sin6_port = htons(PORT);
  inet_pton(AF_INET6, ADDR, &address);
  address_sock.sin6_addr = address;

  socket_fd = socket(PF_INET6, SOCK_STREAM, 0);

  // Connection
  ret_val = connect(socket_fd,
		    (struct sockaddr *) &address_sock,
		    sizeof(address_sock));

  if (ret_val == -1) {
    perror("connect failed");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 10; i++) {
    // Send
    snprintf(msg, SIZE_MSG+1, "Hello%d", i+1);
    printf("SENDING: %s\n", msg);
    send(socket_fd, msg, strlen(msg), 0);
    // Recv
    size_recv = recv(socket_fd, buf, SIZE_MSG, 0);
    buf[size_recv] = '\0';
    printf("RECEIVED: %s\n", buf);
  }

  close(socket_fd);
}
