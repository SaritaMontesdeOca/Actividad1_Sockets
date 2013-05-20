#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "UDPServer.h"
#include "UDPClient.h"
#include "TCPClient.h"
#include "TCPServer.h"

//Variables Globales
enum workMode_t {
	NOT_DEFINE, 
	SERVER, 
	CLIENT,

};
enum proto_t {
	NO_PROT,
	TCP,
	UDP
};
proto_t protocolo = NO_PROT;
workMode_t mode = NOT_DEFINE;
char listenIP[18] = "192.168.1.1";
u_short listenPort = 5001;

/**
  * @brief Procesa los Argumentos del Servidor
  *
  * Función que procesa todo los argumentos y hace el setup de las variables globales
  *
  * @ingroup internal
**/
int processArguments(int args, char *argv[]) {
	int i;
	
	for(i=1; i < args; i++) {
		if(strcmp(argv[i],"-s")==0) {
			//modo de trabajo
			mode = SERVER;
		} else if(strcmp(argv[i],"-c")==0) {
			mode = CLIENT;
		} else if(strcmp(argv[i],"-i")==0) {
			strncpy(listenIP,argv[i+1],18);
			i++;
		} else if(strcmp(argv[i],"-p")==0) {
			listenPort = atoi(argv[i+1]);
			i++;
		} else if(strcmp(argv[i],"-u")==0) {
			protocolo = UDP;
			i++;
		} else if(strcmp(argv[i],"-t")==0) {
			protocolo = TCP;
			i++;
		} 		
		else {
			return i;
		}
	}
	
	return 0;
}

/**
  * @brief Imprime la Ayuda y Errores de Parametros.
  *
  * Imprime la ayuda del sistema, asi como los errores de los parametros
  *
  * @param [cmd] Nombre del binario utilizado por el usuario
  * @param [error] Imprimir mensaje de Error
  * @param [msg] Mensaje de Error a Imprimir
  *
  * Ejemplo de Uso
  * @code
  *		printHelp(argv[0]); // Imprime solo la ayuda 
  *		printHelp(argb[0],true,"Faltan argumentos") // Imprime la ayuda desplegando el mensaje de error.
  * @endcode
  *
  * @ingroup ServerInt
**/
void printHelp(const char *cmd, const short error, const char* msg) {

	if(error) {
		fprintf(stderr,"Error: %s\n\n",msg);
	}
	
	printf(".:: Server Proyecto - Cliente Help ::.\n\n");
	printf("Use:\t%s (-s|-c) [options]\n",cmd);
	printf("Modes:\n");
	printf("\t -s Server Mode\n");
	printf("\t -c Client Mode\n");
	printf("Protocol:\n");
	printf("\t -t TCP Server\n");
	printf("\t -u UDP Server\n");
	printf("Server Options:\n");
	printf("\t -i IP_ADDRESS Listen Interface IP Address (Default: 0.0.0.0) \n");
	printf("GLobal Options:\n");
	printf("\t -p PORT Define Port Number (Default: 1024)\n");

	printf("\n");
	
	return;
}

int main(int argc, char  *argv[]) {
	
	
	int status;
	printf(".:: Starting FTP  Server - Cliente ::.\n\n");
	
	status = processArguments(argc, argv);
	
	if(status != 0) {
		fprintf(stderr,"Error al procesar el argumento %i\n",status);
		return -1;
	}
	
	switch(mode) {
		case SERVER:
			switch(protocolo) {
				case UDP:
					startUDPServer(listenPort);
					break;
				case TCP:
					printf("Introduce la IP del servidor: ");
					gets(listenIP);
					startTCPServer(listenIP,listenPort);
					break;
				case NO_PROT:
					printHelp(argv[0],true,"Es necesario indicar el protocolo de operacion\n");
					break;
			}
			
			break;
		case CLIENT:
			/*if(strcmp(listenIP,"0.0.0.0")==0) {
				printHelp(argv[0],true,"En modo cliente es necesario especificar la IP con -i");
			}*/
			/*switch(protocolo) {
				case UDP:
					startUDPClient(listenPort);
					break;
				case TCP:
					//printf("TCP SERVIDOR\n");
					startTCPClient(listenPort);
					break;
				case NO_PROT:
					printHelp(argv[0],true,"Es necesario indicar el protocolo de operacion\n");
					break;
			}*/
			startUDPClient(listenPort);
			break;
		case NOT_DEFINE:
			printHelp(argv[0],true,"Es necesario indicar el modo de operación\n");
			break;
	}
	
	return 0;
}
