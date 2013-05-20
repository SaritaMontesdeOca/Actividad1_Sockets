/**
 * @file HostServer.c
 * 
**/
 
#include <stdio.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <stdlib.h>

int startServer(char *ip, const u_short port) {
	int udpSocket, clientSocket;
	struct sockaddr_in udpServer, udpClient;
	int status;
	socklen_t addrlen = sizeof(udpClient);
	char buffer[255];
	int option;
	pid_t forkID;
	u_short clientPort;

	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == -1) {
		fprintf(stderr,"Can't create UDP Socket");
		return -1;
	}


	udpServer.sin_family = AF_INET;
	inet_pton(AF_INET,"192.168.1.255",&udpServer.sin_addr.s_addr);
	udpServer.sin_port = htons(port);
	
	status = bind(udpSocket, (struct sockaddr*)&udpServer, sizeof(udpServer));
	
	if(status != 0) { 
		fprintf(stderr,"Can't bind");
		return -1;
	}
	status = recvfrom(udpSocket,buffer,255,0,(struct sockaddr*)&udpClient, &addrlen );
	inet_ntop(AF_INET,&(udpClient.sin_addr),ip,INET_ADDRSTRLEN);
	clientPort = ntohs(udpClient.sin_port);
	printf("[%s:%i] %s\n",ip,clientPort,buffer);
	status = sendto(udpSocket,"Hello-Client",strlen("Hello-Client"),0,(struct sockaddr*)&udpClient, addrlen );
}

