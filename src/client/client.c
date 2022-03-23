#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#define PORT 8080

int main(int argc, char *argv[])
{   
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    char buffer[2048] = {0};
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Couldn't create socket\n");
        exit(EXIT_FAILURE);
    }

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        fprintf(stderr, "Invalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "Connection Failed \n");
        exit(EXIT_FAILURE);
    }
    printf("Connected\n");

    char* message = "Client: hello ";
    send(sockfd, message, strlen(message), 0);
    printf("Hello message sent\n");
    
    if (recv(sockfd, buffer, 2048, 0) < 1) 
    {
        fprintf(stderr, "recv failed\n");
        return 1;
    }
    else
    {
        printf("%s\n",buffer);
    }
    return 1;
}