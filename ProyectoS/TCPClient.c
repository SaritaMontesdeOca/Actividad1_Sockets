/**
 * @file TCPClient.c
 *
 *@brief Funciones referentes al Cliente
 * Funcion que inicia el cliente en protocolo TCP
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Febrero 2012
 * @version 0.0.1
 * @defgroup ClienteTCP
 *
**/
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <stdlib.h>


#include "netlib/tcpServer.h"

int GetFileInfo(int socket);
int GetFileList(int socket);
int GetFile(int socket);
int cabecera(char *cab);
int flag1 = 1;
int total=1;
int npart=-2;

/**
  * @brief Inicia el cliente TCP
  *
  * Funcion que inicia al cliente TCP conectandolo al servidor deseado
  *
  * @ingroup ClienteTCP
**/

int startTCPClient(const char *ip, const u_short port) {

	int socket;
	char msg[1024]="";
	char newFile[500]="";
	int i=0;
	char opcion[2]="";
	int bytes;
	char buffer[1024]="";
	FILE *file;
	//char ip[18]="0.0.0.0";

	socket =newTCPClientSocket4(ip,port);
	if (socket == -1) {
		return -1;
	}
	
	printf("\nNos conectamos a %s:%u\n",ip,port);
	while(1){
		bzero(opcion,2);
		bzero(msg,1024);
		system("clear");
		printf("\t..:: CLIENTE TCP - CONECTADO A %s ::..\n",ip);
		printf("Selecciona una accion a realizar:\n");
		printf("1. Solicitar Lista de archivos\n2. Solicitar Archivo\n3. Solicitar Informacion de archivo\n");
		gets(opcion);
		fflush(stdout);
		if(strcmp(opcion,"1") == 0) {
			
			GetFileList(socket);
			 }
		else if(strcmp(opcion,"2") == 0) {
			//printf("\tOPCION 2\n");
			GetFile(socket); }
		else if(strcmp(opcion,"3") == 0) {
			GetFileInfo(socket); }
		else {
			printf("Opcion Invalida\n");			
			}
		
	
	}
	
	return 0;
}
/**
  * @brief Obteniendo lista de archivos
  *
  * Funcion que solicita al servidor conectado la lista de archivos con un filtro en especial
  *
  * @ingroup ClienteTCP
**/
int GetFileList(int socket){

	int bytes;
	char s2[1024]="";
	char *buffer;
	char filter[10]="";	
	char letra[1];

	buffer = (char *)malloc(9000);
	system("clear");
	printf("\t..:: LISTA DE ARCHIVOS ::..\n");
	printf("Introduzca el filtro de extensiones (0 para recibir todos los documentos)\n");
	gets(filter);
	fflush(stdout);
	strcpy(s2,"GetFileList\n\r");
	
	strcat(s2,"Filter = ");
	strcat(s2,filter);
	strcat(s2,"\n\r");
	strcat(s2,"SizeDatos = 0\n\r");
	strcat(s2,"\n\r");
	bytes = write(socket,s2,strlen(s2)); 
	system("clear");
	printf("\t..:: LISTA DE ARCHIVOS ::..\n");
	bytes = read(socket,buffer,9000);
	printf("%s\n",buffer);
	printf("Presiona una letra para continuar ... \n");	
	scanf(letra);	
	return 0;
}
/**
  * @brief Solicita un archivo al servidor
  *
  * Funcion que solicita un archivo al servidor
  *
  * @ingroup ClienteTCP
**/

int GetFile(int socket){
	
	int bytes;
	char fileName[500]="";
	char msg[1024]="";
	char s[100]="";
	char newFile[500]="";
	char status_t[500]="";
	char byteIni[10]="";
	char byteFin[10]="";
	int bytes_rec;
	char *request,*start,*end,*fullsize,*part,*acpart,*sizedat;
	char *text;
	char buffer[2048]="";
	char resp[2048]="";
	char fileby[2048]="";
	FILE *file;
	system("clear");
	printf("\t..:: DESCARGA DE ARCHIVO ::..\n");
	printf("Escribe el nombre del archivo:\n");
	bzero(fileName,500);
	bzero(status_t,500);
	bzero(newFile,500);
	bzero(byteIni,10);
	bzero(byteFin,10);
	gets(fileName);
	fflush(stdout);
	printf("Escribe el byte inicial:\n");
	gets(byteIni);
	fflush(stdout);
	printf("Escribe el byte final(0 para recibir hasta el EOF):\n");
	gets(byteFin);
	fflush(stdout);
	strcpy(msg,"GetFile\n\r");
	strcat(msg,"Name: ");
	strcat(msg,fileName);
	strcat(msg,"\n\r");
	strcat(msg,"StarBytes: ");
	strcat(msg,byteIni);
	strcat(msg,"\n\r");
	strcat(msg,"EndBytes: ");
	strcat(msg,byteFin);
	strcat(msg,"\n\r");
	strcat(msg,"SizeDatos: 0\n\r\n\r");
	char *lh=(char *)malloc(strlen(buffer));;
	bytes = write(socket,msg,strlen(msg)); 
	file = fopen(fileName,"w");
	fflush(stdout);
	system("clear");
	while(npart != total) {
		
		bzero(buffer,2048);
		bzero(resp,2048);
		bzero(fileby,2048);
		bytes_rec = read(socket,buffer,2048);
		strcpy(resp,buffer);
		cabecera(resp);
		bytes_rec = read(socket,fileby,2048);
		int writeb = fwrite(fileby,1,bytes_rec,file);

	}
	printf("\n\t..:: ARCHIVO RECIBIDO ::..\n");
	char letra[1];
	fflush(stdout);
	printf("Presiona una letra para continuar ... \n");	
	gets(letra);	
	fclose(file);
	return 0;
}

/**
  * @brief Obtener informacion del archivo
  *
  * Funcion que solicita al servidor la informacion de un archivo en especial (cadena MD5)
  *
  * @ingroup ClienteTCP
**/
int GetFileInfo(int socket){

	int bytes;
	char Filename[100]="";
	char s2[1024]="";
	char buffer[1024]="";	
	system("clear");
	printf("\t..:: INFORMACION DE ARCHIVO ::..\n");
	printf("Introduza el nombre del archivo del que desea la informacion:\n");
	bzero(Filename,100);
	bzero(s2,1024);	
	bzero(buffer,1024);
	gets(Filename);
	strcpy(s2,"GetFileInfo\n\r");
	strcat(s2,"Name: ");
	strcat(s2,Filename);
	strcat(s2,"\n\r");
	strcat(s2,"SizeDatos: 0\n\r\n\r");
	
	bytes = write(socket,s2,strlen(s2));
	printf("Esperando respuesta ..::\n");
	bytes = read(socket,buffer,1024);
	printf("server: %s\n",buffer);

	return 0;
}

/**
  * @brief Procesar las cabeceras recibidas con el archivo
  *
  * Funcion que procesa las cabeceras adjuntas al archivo, para poder identificar el total de transmisiones y la transmision actual
  *
  * @ingroup ClienteTCP
**/

int cabecera(char *cab) {
	
	//printf("%s\n",cab);
	char *partnum,*parts,*partnum_str,*parts_str,*ptrptr,*btrbtr,*btkbtk;
	strtok_r(cab,"\n\r",&ptrptr);
	for (int i=0;i<4;i++)
		strtok_r(NULL,"\n\r",&ptrptr);
	partnum_str = strtok_r(NULL,"\n\r",&ptrptr);
	parts_str = strtok_r(NULL,"\n\r",&ptrptr);
	strtok_r(partnum_str," ",&btrbtr);
	partnum = strtok_r(NULL,"\n\r",&btrbtr);
	strtok_r(parts_str," ",&btkbtk);
	parts = strtok_r(NULL,"\n\r",&btkbtk);
	npart = atoi(partnum);
	total = atoi(parts);
	printf("Recibido %i de %i\n",npart,total);	
	
}

