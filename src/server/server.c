#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int tcpMode(int argc, char *argv[]) {
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    int addrlen = sizeof(address);
    int opt = 1;
    int new_socket;
    char buffer[2048] = {0};
    int sockfd = 0;


    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Couldn't create socket\n");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        fprintf(stderr, "Couldn't attach the socket to port 8080, port already in use ?\n");
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (struct sockaddr *)&address, addrlen) < 0)
    {
        fprintf(stderr, "bind failed\n");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 3);

    if ((new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0 )
    {
        fprintf(stderr, "accept failed\n");
        exit(EXIT_FAILURE);
    }

    char* message = "Server: hello";
    while (recv(new_socket, buffer, 2048, 0) > 0)
    {
        printf("%s\n",buffer );
        send(new_socket, message, strlen(message), 0);
        printf("Message sent to client\n");
    }
    return 0;}

int udpMode(int argc, char *argv[]) {

	int sockfd;
	char buffer[2048];
	char *hello = "Hello from server";
	struct sockaddr_in serveur, client;

	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&serveur, 0, sizeof(serveur));
	memset(&client, 0, sizeof(client));
	// Filling server information
	serveur.sin_family = AF_INET; // IPv4
	serveur.sin_addr.s_addr = INADDR_ANY;
	serveur.sin_port = htons(PORT);

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&serveur, sizeof(serveur)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len, n;

	len = sizeof(client); // len is value/resuslt

	n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL,
				(struct sockaddr *)&client, &len);
	buffer[n] = '\0';
	printf("%s\n", buffer);
	sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
			(const struct sockaddr *)&client, len);
	printf("Hello message sent.\n");

	return 0;
}

int main(int argc, char *argv[]) {
	if (!strcmp(argv[1], "TCP")) {
		tcpMode(argc, argv);
	} else if (!strcmp(argv[1], "UDP")) {
		udpMode(argc, argv);
	}

	return 0;
}
