/**
 * @file main.c
 *
 * @brief Descripción Corta del Archivo
 *
 * DESCRIPCIÓN LARGA DEL ARCHIVO
 * CONTINUA LA DESCRIPCIÓN
 *
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
 * @date Marzo 2012
 * @version 0.0.1
 * 
 * \defgroup internal Internal Functions
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "HostServer.h"
#include "HostClient.h"

//Variables Globales
enum workMode_t {
	NOT_DEFINE, 
	SERVER, 
	CLIENT
};

workMode_t mode = NOT_DEFINE;
char listenIP[18] = "0.0.0.0";
u_short listenPort = 1024;

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
	
	printf(".:: Simple Host Discover Server - Cliente help ::.\n\n");
	printf("Use:\t%s (-s|-c) [options]\n",cmd);
	printf("Modes:\n");
	printf("\t -s Server Mode\n");
	printf("\t -c Client Mode\n");
	printf("Server Options:\n");
	printf("\t -i IP_ADDRESS Listen Interface IP Address (Default: 0.0.0.0) \n");
	printf("GLobal Options:\n");
	printf("\t -p PORT Define Port Number (Default: 1024)\n");

	printf("\n");
	
	return;
}

int main(int argc, char  *argv[]) {
	
	int status;
	printf(".:: Starting Simple UDP Host Discover Server - Cliente ::.\n\n");
	
	status = processArguments(argc, argv);
	
	if(status != 0) {
		fprintf(stderr,"Error al procesar el argumento %i\n",status);
		return -1;
	}
	
	switch(mode) {
		case SERVER:
			startServer(listenIP, listenPort);
			break;
		case CLIENT:
			if(strcmp(listenIP,"0.0.0.0")==0) {
				printHelp(argv[0],true,"En modo cliente es necesario especificar la IP con -i");
			}
			startClient(listenIP,listenPort);
			break;
		case NOT_DEFINE:
			printHelp(argv[0],true,"Es necesario indicar el modo de operación\n");
			break;
	}
	
	return 0;
}

