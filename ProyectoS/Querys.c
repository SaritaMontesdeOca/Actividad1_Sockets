/**
 * @file Querys.c
 * @brief Funciones Referentes al Servidor UDP
 * Funcion que contesta al cliente el broadcast por medio de un unicast con el
 * el mensaje de hola y la cantidad de archivos
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
 * @date Abril 2012
 * @version 0.0.1
 * 
**/

#include <stdio.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <dirent.h>

#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <stdlib.h>
#include "Querys.h"

/**
 * @brief Funciones Referentes al Servidor UDP
 * Funcion que contesta al cliente el broadcast por medio de un unicast con el
 * el mensaje de hola y la cantidad de archivos
 * @ingroup ServidorUDP
**/

int sendList(int clientSocket,char *ip, int num, struct sockaddr_in udpClient, socklen_t addrlen) {

	int status;
	char msg[255];
	DIR *dirh;
	struct dirent *dirp;
	struct dirent **list;
	static char mydir[40] = "/media/share/Codigos/UDP_HostD";
	int archivos=0;
	char fileC[10]="";
	char *lista;	

	sprintf(msg,"Hello - %s",ip);
	archivos = scandir(mydir,&list,NULL,NULL);
	bzero(fileC,10);
	sprintf(fileC,"%s - %i\0",msg,archivos);
	//printf("%s\n",lista);
	status = sendto(clientSocket,fileC,strlen(fileC),0,(struct sockaddr*)&udpClient, addrlen);
	closedir(dirh);
	return true;
}

