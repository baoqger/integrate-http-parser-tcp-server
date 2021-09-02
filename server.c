#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "net.h"
#include "parser.h"

#define PORT "3490" // the port users will be connecting to

/**
 * Handle HTTP request and send response
 */

void handle_http_request(int fd)
{
    const int request_buffer_size = 65536; // 64k
    char request[request_buffer_size];
    // size_t nparsed;

    http_parser *parser = malloc(sizeof(http_parser));
    http_parser_init(parser, HTTP_REQUEST);

    parser->data = &fd; // ? 

    // Read request
    int bytes_recvd = recv(fd, request, request_buffer_size - 1,  0);

    if (bytes_recvd < 0) {
        perror("recv");
        return;
    }

    http_parser_execute(parser, &settings_null, request, bytes_recvd);

    free(parser);
    parser = NULL;
}


int main(void)
{
    int newfd; // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0) {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }
    
    printf("webserver: waiting for connections on port %s...\n", PORT);

    // This is the main loop that accepts incoming connections and
    // responds to the request. The main parent process
    // then goes back to waiting for new connections

    while(1) {
        socklen_t sin_size = sizeof their_addr;

        // Parent process will block on the accept() until someone
        // makes a new connection:
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }

        // Print out a message that we got the connection
        inet_ntop(their_addr.ss_family, 
            get_in_addr((struct sockaddr *)&their_addr), 
            s, sizeof s);
        
        printf("server: got connection from %s\n", s);

        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.

        handle_http_request(newfd);

        close(newfd);
    }

    return 0;
}