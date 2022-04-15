#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int tcpMode()
{
    struct sockaddr_in serv_address;

    int opt = 1;
    int new_socket;
    struct sockaddr_in newaddress;
    int addrlen1 = sizeof(serv_address);
    int addrlen2 = sizeof(newaddress);
    char buffer[2048];
    int sockfd = 0;
    pid_t childpid;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Couldn't create socket\n");
        exit(EXIT_FAILURE);
    }
    printf("server socket is created \n");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        fprintf(stderr, "failed to set SO_REUSEADDR\n");
        return 1;
    }

    // memset(&serv_address,'\0',addrlen1);
    serv_address.sin_family = AF_INET;
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_address.sin_port = htons(PORT);

    int ret = bind(sockfd, (struct sockaddr *)&serv_address, addrlen1);
    if (ret < 0)
    {
        fprintf(stderr, "bind failed1\n");
        exit(EXIT_FAILURE);
    }
    printf("bind to port \n");

    if (listen(sockfd, 10) == 0)
    {
        printf("listening....\n");
    }
    else
    {
        printf("bind failed2\n");
    }
    while (1)
    {
        if ((new_socket = accept(sockfd, (struct sockaddr *)&newaddress, (socklen_t *)&addrlen2)) < 0)
        {
            fprintf(stderr, "accept failed\n");
            exit(EXIT_FAILURE);
        }
        printf("connection accepted from %s:%d\n", inet_ntoa(newaddress.sin_addr), ntohs(newaddress.sin_port));
        if ((childpid = fork()) == 0)
        {
            close(sockfd);
            while (1)
            {
                recv(new_socket, buffer, 2048, 0);
                if (strcmp(buffer, "exit") == 0)
                {
                    printf("disconnected from %s:%d\n", inet_ntoa(newaddress.sin_addr), ntohs(newaddress.sin_port));
                    break;
                }
                else
                {
                    printf("client: %s\n", buffer);
                    send(new_socket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
                }
            }
        }
    }
    close(new_socket);

    return 0;
}

int udpMode()
{

    int sockfd;
    char buffer[2048];
    // char *hello = "Hello from server";
    struct sockaddr_in serv_addr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    // Filling server information
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket with the server address
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int recv_len, slen = sizeof(serv_addr);

    while (1)
    {
        printf("Waiting for data...");
        fflush(stdout);

        // try to receive  data
        if (recv_len = (recvfrom(sockfd, buffer, 2048, 0, (struct sockaddr *)&serv_addr, &slen)) == -1)
        {
            fprintf(stderr, "recvfrom()");
            exit(1);
        }

        printf("Received packet from %s:%d\n", inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
        printf("Data: %s\n", buffer);

        // now reply the client with the same data
        if (sendto(sockfd, buffer, recv_len, 0, (struct sockaddr *)&serv_addr, slen) == -1)
        {
            fprintf(stderr, "sendto()");
            exit(1);
        }
    }

    close(sockfd);
    /*
      int len, n;

      len = sizeof(client); // len is value/resuslt

      n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL,
                   (struct sockaddr *)&client, &len);
      buffer[n] = '\0';
      printf("%s\n", buffer);
      sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
             (const struct sockaddr *)&client, len);
      printf("Hello message sent.\n");
  */
    return 0;
}

int main(int argc, char *argv[])
{
    if (!strcmp(argv[1], "TCP"))
    {
        tcpMode(argc, argv);
    }
    else if (!strcmp(argv[1], "UDP"))
    {
        udpMode(argc, argv);
    }
    return 0;
}
