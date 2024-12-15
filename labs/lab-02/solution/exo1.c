#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

int send_bytes_over_network (uint8_t *bytes, int num_bytes) {
    return 0;
}


int main (int argc, char **argv) {

    int x = 0x01234567;
    uint8_t buffer[256];

    memcpy(buffer, &x, sizeof(int));
    for (int i = 0; i < sizeof(int); i++) {
        printf("%02hhX ", buffer[i]);
    }
    printf("\n");

    send_bytes_over_network(buffer, sizeof(int));

    return 0;
}
