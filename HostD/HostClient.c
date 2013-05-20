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

#include "HostClient.h"

int startClient(const char *ip, const u_short port) {
	int udpSocket;
	struct sockaddr_in udpServer, udpClient;
	int status;
	socklen_t addrlen = sizeof(udpClient),len;
	char *buffer;
	u_short clientPort;
	int option;



	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == -1) {
		fprintf(stderr,"Can't create UDP Socket");
		return -1;
	}


	udpServer.sin_family = AF_INET;
	inet_pton(AF_INET,"192.168.1.255",&udpServer.sin_addr.s_addr);
	udpServer.sin_port = htons(port);
	
	len = sizeof(option);
	getsockopt(udpSocket, SOL_SOCKET, SO_SNDBUF, (void *)&option, &len);
	//printf("El buffer de envio es de %i\n",option);
	
	option = 1;
	status = setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (void *)&option, len);
	if (status == -1) {
		printf("Error en poner la opcion\n");
		return -1;
	}
	buffer = (char *) malloc(9000);
	status = sendto(udpSocket,"hello",strlen("hello"),0,(struct sockaddr*)&udpServer,sizeof(udpServer));
	status = recvfrom(udpSocket,buffer,9000,0,(struct sockaddr*)&udpClient, &addrlen);
	printf("Mensaje del Servidor: %s\n",buffer);
	
	close(udpSocket);
	return 0;
		
}

