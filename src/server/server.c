#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 8080

int main()
{
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    int new_socket;
    char buffer[2048] = {0};

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (!sockfd)
    {
        fprintf(stderr, "Couldn't create socket\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        fprintf(stderr, "Could attach the socket to port 8080, port already in use ?\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    if (bind(sockfd, (struct sockaddr *)&address, addrlen) < 0)
    {
        fprintf(stderr, "bind failed\n");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 3);

    if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0 )
    {
        fprintf(stderr, "bind failed\n");
        exit(EXIT_FAILURE);
    }

    read(new_socket, buffer, 2048);
    printf("%s\n",buffer );

    return 0;
}