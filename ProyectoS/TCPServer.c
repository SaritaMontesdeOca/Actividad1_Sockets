/**
 * @file TCPServer.c
 *
 *@brief Funciones referentes al Servidor TCP
 * Funciones que inician el trabajo del servidor en protocolo TCP
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Abril 2012
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
#include "netlib/tcpServer.h"
#include "tcp/process.h"


int client = 0;

/**
  * @brief Inicializa al servidor TCP
  *
  * Función que inicia al servidor TCP en espera de una conexion 
  *
  * @ingroup ServidorTCP
**/

int startTCPServer(const char *ip,const u_short port) {
	
	int serverSocket;
	int clientSocket;	
	
	
	char clientIP[20];
	u_short clientPort;

	pid_t forkID;

	serverSocket = newTCPServerSocket4(ip,port,5);
	system("clear");
	printf(".:: Starting FTP  Server - TCP ::.\n\n");
	while(1){
		printf("Esperando Conexion ...\n");
		fflush(stdout);
		clientSocket=waitConnection(serverSocket,clientIP,&clientPort);
		client++;
		printf("Tenemos una conexion de %s: %i\t\n",clientIP,clientPort);
		fflush(stdout);
		forkID = fork();
		
		if (forkID == -1){
			perror("fork");
		} else if(forkID == 0) {
			close(serverSocket);
			DialogosTCP(clientSocket,client);
			client--;
			fflush(stdout);
			close(clientSocket);
				
		} else {
			
		}
				
	}
	return 0;
}
