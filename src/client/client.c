#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int tcpMode(int argc, char *argv[])
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

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
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

	char *message = "Hello ";
	send(sockfd, message, strlen(message), 0);
	printf("Hello message sent\n");

	if (recv(sockfd, buffer, 2048, 0) < 1)
	{
		fprintf(stderr, "recv failed\n");
		return 1;
	}
	else
	{
		printf("%s\n", buffer);
	}
	return 1;
}

int udpMode(int argc, char *argv[])
{

	struct sockaddr_in serv_addr;

	char buffer[2048];
	int sockfd;

	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	// Filling server information
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	int n, len;
	char *message = "Client : hello ";

	sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM,
		   (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
	printf("Hello message sent.\n");

	n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL,
				 (struct sockaddr *)&serv_addr, &len);
	buffer[n] = '\0';
	printf("Server : %s\n", buffer);

	close(sockfd);
	return 0;
}

int main(int argc, char *argv[])
{

	if (!strcmp(argv[1], "TCP"))
	{
		printf("salut\n");
		tcpMode(argc, argv);
	}
	else if (!strcmp(argv[1], "UDP"))
	{
		udpMode(argc, argv);
	}

	return 0;
}
