/**
 * @file tcpServer.c
 *
 * @brief Funciones Generales para manejo de servidores HTTP
 *
 * Funciones para controlar un servidor HTTP
 *
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
 * @date Febrero 2012
 * @version 0.0.1
 *
 * \defgroup sockets Funciones Referentes a Sockets
**/

/* Requires Files*/
#include <stdio.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#include "ipFunc.h"
#include "tcpServer.h"

/* Private Functions */
/**
  * @brief Crea un nuevo servidor TCP Generico.
  *
  * Crea un nuevo Servidor TCP utilizando el direccionamiento indicado.
  *
  * @param addrType Tipo de Direccionamiento a utilizar.
  *
  * @return Identificador del Socket
  * @retval -1 	Ocurrio un Error y no se pudo crear el Socket
  *
  * Ejemplo de Uso:
  * @code
  *		getTCPSocket(AF_INET) // Crea un nuevo socket que utiliza IPv4 
  * @endcode
  *
  * @ingroup sockets
  * @private
  * @warning No utilizar
  *
**/
int getTCPSocket(int addrType) {
	int socketNum;
	
	socketNum = socket(addrType,SOCK_STREAM,0);
	if (socketNum == -1) {
		fprintf(stderr,"ERROR: No se puede crear un socket nuevo\n");
		return -1;
	}
	
	return socketNum;
}

int newTCPServerSocket4(const char *ip, const u_short port, int q_size) {
	int socket;
	int status;
	struct sockaddr_in addr;
	
	status = buildIP4addr(ip,&addr);
	if (status == -1) {
		fprintf(stderr,"ERROR: La direccion IP %s es invalida\n",ip);
		return -1;
	}
	
	addr.sin_port = htons(port);
	socket=(getTCPSocket(PF_INET));
	
	status = bind(socket,(struct sockaddr*)&addr, sizeof(addr));
	
	if (status==-1) {
		fprintf(stderr,"Can't bind on port %i\n",port);
		return -1;
	}
	
	status = listen(socket,q_size);
	if(status == -1) {
		fprintf(stderr,"ERROR: No se puede escuchar en %s:%i\n",ip,port);
		return -1;
	}
	
	return socket;
}

int waitConnection(int socket, char *clientIP, u_short *clientPort) {
	
	int client;
	struct sockaddr_in addrClient;
	socklen_t addrLen = sizeof(addrClient);
	char ip[18]="0.0.0.0";
	
	client = accept(socket,(struct sockaddr *)&addrClient, &addrLen);
	
	if (client == -1) {
		fprintf(stderr,"Can't retrive client socket for connection\n");
		return -1;
	}
	
	if(clientIP !=NULL) {
		inet_ntop(AF_INET,&(addrClient.sin_addr),ip,INET_ADDRSTRLEN);
		strcpy(clientIP,ip);
	}
	
	if (clientPort!=NULL) {
		*clientPort=ntohs(addrClient.sin_port);
	}
	return client;
}

int newTCPClientSocket4(const char *ip,const u_short port) {
	int socket;
	int status;
	struct sockaddr_in addr;
	
	status = buildIP4addr(ip,&addr);
	
	if (status==-1){
		fprintf(stderr,"ERROR: La direccion IP %s es invalida\n",ip);
		return -1;
	}
	
	addr.sin_port=htons(port);
	
	socket = getTCPSocket(PF_INET);
	
	status = connect(socket, (struct sockaddr*)&addr, sizeof(addr));
	
	if(status==-1) {
		fprintf(stderr,"No se puede conectar a %s:%i\n",ip,port);
		return -1;
	}
	
	return socket;
}


