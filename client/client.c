#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 500
#define PORT 8080
#define SA struct sockaddr

char name[MAX], fName[MAX];

char* getName(int sockfd) {
	printf("Input the file name : ");
	scanf("%s", name);

	if (send(sockfd, name, MAX, 0) == -1) {
		perror("File name sending error.\n");
		exit(0);
	}

	return name;	
}

void sendFile(int sockfd) {
	char buff[MAX];

	FILE *fp;
	while(1) {
		fp = fopen(getName(sockfd), "rb");
		
		if (fp == NULL) {
			printf("File doesn't exist.\n");
		}
		else	break;
	}

	while (fgets(buff, MAX, fp) != NULL) {
		write(sockfd, buff, sizeof(buff));
	}

	fclose(fp);
	printf("File Sent Successfully.\n");
}

int createSocket() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Socket Creation Failed.\n");
		exit(0);
	} else 
		printf("Socket Successfully Created.\n");

	return sockfd;
}

int main() {
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	sockfd = createSocket(); 

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection failed.\n");
		exit(0);
	} else
		printf("Connected to the server.\n");

	sendFile(sockfd);

	close(sockfd);

	return 0;
}
