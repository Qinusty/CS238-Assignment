#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <zconf.h>
#include <errno.h>
#include <string.h>
#include "Inglenook.h"

#define error(param) { fprintf(stderr, param); }
#define ABORT(param) { fprintf(stderr, param); exit(1); }
#define TRUE  1
#define FALSE 0
#define PORT 10090

#define DEBUG

// initial setup, great for testing with telnet
int DEFAULT_SIZES[] = {3, 5, 3 ,3};

siding_config* default_sidings() {
    siding_config* config = init_siding(4, DEFAULT_SIZES);
    config->siding_contains[1] = 2;
    config->siding_contains[2] = 3;
    config->siding_contains[3] = 3;
    return config;
}

int main() {
    siding_config* config = default_sidings();
    printf("Inglnook Siding jos67\n");

    struct sockaddr_in ourself;
    int socketid = -1;
    int t, y;
    socketid = socket(AF_INET, SOCK_STREAM, 0);
    if (socketid < 0)
        ABORT("socket function returned < 0");
    int so_reuseaddr = 1;
    setsockopt(socketid, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof so_reuseaddr);
    ourself.sin_family = AF_INET;
    ourself.sin_addr.s_addr = INADDR_ANY;
    ourself.sin_port = htons(PORT);

    if (bind(socketid, (struct sockaddr *) & ourself, sizeof(ourself))<0) {
        (void)close(socketid);
        ABORT("Failed to bind socket");
    }
    listen(socketid, 10);

    for (;;) {
        unsigned int len_otherend;
        struct sockaddr_in otherend;
        /* Wait for connection. */
        fprintf(stdout, "About to accept..\n");
        do {
            errno = 0; // to make sure we don't react to an old error
            len_otherend = sizeof(otherend);
            t = accept(socketid, (struct sockaddr *) &otherend,
                       &len_otherend);
        } while (t < 0 && errno == EINTR);

        if (t < 0) { // if couldn't accept
            error("Couldn't accept...")
            sleep(5);
            continue;
        }
        y = dup(t);
        printf("Sending message!\n");
        // Send the welcome message.
        char str[] = "Inglenook Sidings jos67\n";
        write(y, str, sizeof(str));
        
        // COMMAND LOOP
        for (;;) {
            char buffer[BUFFER_SIZE] = "";
            int bytes_read = read(t, buffer, BUFFER_SIZE);
            char response[BUFFER_SIZE] = "";
            if (bytes_read == 0) {
                ABORT("Client sent EOF");
            } else if (bytes_read == -1) {
                ABORT("Client connection closed unexpectedly.");
            }
            fflush(stdout);
            if (strnlen(buffer, BUFFER_SIZE)) {
                printf("RECEIVED: %s\n", buffer);
                handleCommand(buffer, response, &config);
                printf("SENDING: %s\n", response);
                write(y, response, sizeof(response));
                // Padd line for readability
                printf("\n"); 
            }
        }
    }
}





