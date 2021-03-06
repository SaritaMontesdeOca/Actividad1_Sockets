// Sahara Montes de Oca y Fatima De Niz
// Copia de Alvaro Parres

#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "defaults.h"
#include "utils/debug.h"
#include "server.h"


#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>


//Variables globales
u_short port = CONFIG_DEFAULT_PORT;
int debugLevel = CONFIG_DEFAULT_VERBOSE;
char baseDir[255] = CONFIG_DEFAULT_BASEDIR;

//Prototipos de funciones
int startClient(const char *ip, const u_short port);

int processArguments(int argc, char* argv[]);

void printHelp(const char *cmd, const short error, const char *msg="");

int main(int argc, char *argv[]) {	
	
	printf("--File Transfer Server--\n");
	if(!processArguments(argc, argv)) return -1;
	startClient("0.0.0.0", 5001);
	start_server(port,baseDir);
}

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

//Funcion de argumentos
int processArguments(int argc, char* argv[]) {
	int i;
	u_int temp;
	for(i=1; i<argc; i++) {
		if(strcmp(argv[i],"-h")==0) {
			printHelp(argv[0],false);
		return true;
		}	
		
		if(strcmp(argv[i],"-p")==0) {
			temp = strtoimax(argv[++i], NULL, 10);
			if(temp == 0 || temp < MINPORT || temp > MAXPORT) {
				printHelp(argv[0],true,"Port out of range\n");
				return false;
			}
			port = temp;
		} else if(strcmp(argv[i],"-d")==0) {
			strncpy(baseDir,argv[++i],254);
		} else if(strstr(argv[i],"-v")!=NULL) {
			debugLevel = strlen(argv[i])-1;
		} else {
			printHelp(argv[0],true,"Unkown option\n");
			return false;
		}	
	}

	debug(2,"%s","Configuraciones:");
	debug(2,"Puerto:\t\t%u",port);
	debug(2,"Base Directory:\t%s",baseDir);
	return true;
}

//Funcion print

void printHelp(const char *cmd, const short error, const char *msg) {
	if(error) {
		fprintf(stderr,"Error: %s\n\n",msg);
	}
	printf("Use:\t%s [options]\n", cmd);
	printf("\n");
	printf("Options:\n");
	printf("\t-p PORT_NUMBER\t Set port number to work on (Default %u)\n",CONFIG_DEFAULT_PORT);
	printf("\t-d DIRECTORY\t Set port number to work on (Default Same)\n");	
	printf("\t-v[vvvv]\t Increments the verbosity of the server (Default %u)\n",CONFIG_DEFAULT_VERBOSE);
	printf("\t-h\t Prints this help message\n");
	printf("\n");
}
