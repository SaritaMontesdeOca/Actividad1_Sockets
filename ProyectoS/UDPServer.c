/**
 * @file UDPServer.c
 * @brief Funciones Referentes al Servidor UDP
 * Funcion que recibe el broadcast de los clientes. Y llama a otra funcion 
 * para enviar la lista de archivos por un unicast
 * @version 0.0.1
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

#include "Querys.h"
//int sendList(int clientSocket,char *ip, int num, struct sockaddr_in udpClient, socklen_t addrlen);
int clientes = 0;
/**
  * @brief Inicia la operacion del servidor UDP
  *
  * Función que inicia la escucha en broadcast del servidor
  * 
  *@ingroup ServidorUDP
**/

int startUDPServer(const u_short port) {
	int udpSocket;
	struct sockaddr_in udpServer, udpClient;
	int status;
	socklen_t addrlen = sizeof(udpClient);
	char buffer[255];
	char msg[255];
	pid_t forkID;
	u_short clientPort;
	char ip[18]="0.0.0.0";	
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
	system("clear");
	printf(".:: Starting FTP  Server - UDP ::.\n\n");
	while (1){	
		printf("Esperando Conexion ... \n");
		status = recvfrom(udpSocket,buffer,255,0,(struct sockaddr*)&udpClient, &addrlen);
		strcpy(msg,buffer);
		bzero(buffer,255);		
		inet_ntop(AF_INET,&(udpClient.sin_addr),ip, INET_ADDRSTRLEN);
		clientPort = ntohs(udpClient.sin_port);
		clientes++;
		fflush(stdout);
		printf("%i - [%s:%i] %s\n",clientes,ip,clientPort,msg);
		forkID = fork();
		
		if (forkID == -1) {
			fprintf(stderr,"Cant Fork\n");
		} else if (forkID == 0) {
			sendList(udpSocket,ip,clientes,udpClient,addrlen);
			clientes--;
			fflush(stdout);
			close(udpSocket);
			break;
		} else {
			//close(udpSocket);
		} 

		//sleep(10);
	}
	return 0;
}


