/**
 * @file UDPClient.c
 * @brief Funciones Referentes al Cliente UDP
 * Funciones en las que el cliente descubre a los servidores por medio de un broadcast y crea la lista de servidores con la cual se realizara la conexion TCP a uno de ellos
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
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

#include "UDPClient.h"
#include "UDPClient.h"
#include "TCPClient.h"

int server = 0;
char flag = 2;
char lista[254][23]; //Matriz en la que se almacenan las IP's de los servidores
int i=0;
int j;
char ipServ[18]="0.0.0.0";
int k;
/**
  * @brief Inicia el cliente UDP Host Discover
  *
  * Función que envia un broadcast y recibe las respuestas de los servidores conectados
  *
  * @ingroup ClienteUDP
**/

int startUDPClient(const u_short port) {
	int udpSocket;
	struct sockaddr_in udpServer, udpClient;
	int status;
	socklen_t addrlen = sizeof(udpClient),len;
	char *buffer;
	char opcion[2]="";
	u_short clientPort;
	int option;	
	//char conc[23]="";	
	char ip[18]="0.0.0.0";	
	char Sel[2]="";		

	pid_t forkID;
	

	udpSocket = socket(AF_INET, SOCK_DGRAM,0);
	if (udpSocket == -1) {
		fprintf(stderr,"Can't create UDP Socket");
		return -1;
	}
	
	udpServer.sin_family = AF_INET;
	inet_pton(AF_INET,"192.168.1.255",&udpServer.sin_addr.s_addr);
	udpServer.sin_port = htons(port);
	len = sizeof(option);
	getsockopt(udpSocket, SOL_SOCKET, SO_SNDBUF, (void *)&option, &len);

	option = 1;
	status = setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, (void *)&option, len);
	if (status == -1) {
		printf("Error en poner la opcion\n");	
		return -1;
	}
	buffer = (char *)malloc(9000);
	status = sendto(udpSocket,"Hello - Alfredo",strlen("Hello - Alfredo"),0,(struct sockaddr*)&udpServer,sizeof(udpServer));
	
	while (1){
		server ++;
		if (flag != 2){
			printf("Deseas realizar una conexion?\n");
			printf("1. Si\n2. No\n");
			gets(opcion);
			if (strcmp(opcion,"1")==0){
				//system("cls");
				printf("Lista de Servidores\n\n");
				for (j=0;j<i;j++){
					printf("\n%i. [%s]\n",j+1,lista[j]);			
				}
				printf("Introduce el numero de servidor al que te deseas conectar:\n");
				gets(Sel);
				k = atoi(Sel);
				strcpy(ipServ,lista[k-1]);
				startTCPClient(ipServ,port);
		}
		else {
			system("clear"); 	
		}
		}
		status = recvfrom(udpSocket,buffer,9000,0,(struct sockaddr*)&udpClient, &addrlen);
		inet_ntop(AF_INET,&(udpClient.sin_addr),ip,INET_ADDRSTRLEN);
		strcpy(lista[i],ip);
		for (j=0;j<=i;j++){
			printf("\n%i. [%s] : %s\n",j+1,lista[j],buffer);			
		}
	
		i++;
		flag = 0;
		forkID = fork();
		if (forkID == -1){
			perror("fork"); }
		else if (forkID == 0) {
			fflush(stdout);
			close(udpSocket);
			break;
		} else {
		
		}
	

	}
	return 0;


}


	
