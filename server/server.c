#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 500
#define PORT 8080
#define SA struct sockaddr

char name[MAX];

void recvFile(int sockfd) {
	char buf[MAX];

	FILE *fp;

	if (recv(sockfd, name, MAX, 0) == -1) {
		perror("File name receiving error.\n");
		exit(0);
	}

	fp = fopen(name, "wb");

	if (fp == NULL) {
		printf("Error opening file. \n");
		return ;
	}

	while (read(sockfd, buf, MAX) > 0)
		fprintf(fp, "%s", buf);

	printf("File received successfully. \n");
}

int createSocket() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) {
		printf("Socket Creation Failed.\n");
		exit(0);
	} else {
		printf("Socket Successfully Created.\n");
	}

	return sockfd;
}

int main() {

	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;
	
	sockfd = createSocket(); 

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed..\n");
		exit(0);
	} else
		printf("Socket successfully bound.\n");

	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed..\n");
		exit(0);
	} else {
		printf("Server listening..\n");
	}

	len = sizeof(cli);

	connfd = accept(sockfd, (SA*)&cli, &len);

	if (connfd < 0) {
		printf("Server accept failed..\n");
		exit(0);
	} else
		printf("Server accepted the client..\n");

	recvFile(connfd);

	close(sockfd);

	return 0;
}
