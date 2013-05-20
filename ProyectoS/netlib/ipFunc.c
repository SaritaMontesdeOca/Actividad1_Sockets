/**
 * @file ipFunc.c
 *
 * @brief Funciones Generales para manejo de direcciones IP
 *
 * Funciones generales para el manejo de direcciones IP
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Febrero 2012
 * @version 0.0.1
 *
 * \defgroup sockets Funciones Referentes a Sockets
**/
#include <stdio.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "ipFunc.h"

int buildIP4addr(const char *ip, struct sockaddr_in *addr) {
	
	int status;
	
	if (ip==NULL) {
		fprintf(stderr,"ERROR: La direccion IP es nula\n");
		return -1;
	}
	
	if(addr == NULL) {
		fprintf(stderr,"ERROR: El espacio de memoria de la estructura de direcciones es nula\n");
		return -1;
	}
	
	bzero(addr, sizeof(addr));
	
	addr->sin_family=AF_INET;
	status=inet_pton(AF_INET,ip,&(addr->sin_addr.s_addr));
	
	if(status==-1) {
		fprintf(stderr,"ERROR: Direccion IP %s no valida\n",ip);
		return -1;
	}
	return 0;
}
