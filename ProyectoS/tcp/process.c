/**
 * @file process.c
 * @brief Funciones Referentes al Servidor
 * El servidor compara la solicitud del cliente y la procesa con el codigo
 * para realizar la operacion necesaria
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
 * @date Abril 2012
 * @version 0.0.1
 * 
**/

 #include <inttypes.h>
 #include <sys/types.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <string.h>
 #include <fcntl.h>
 #include <sys/uio.h>
 #include <stdlib.h>
 #include <dirent.h> 
 #include <math.h>
 #include <stdarg.h>
 #include <openssl/md5.h>

 #define BUFFERSIZE 1024
 #define FALSO 0 
 #define VERDADERO !FALSO 

int FileList(int clientSocket, char *msg);
int FileTransfer(int clientSocket, char *msg);
int FileInfo(int clientSocket, char *msg);
static char mydir[40] = "/media/share/Codigos/UDP_Ft";
char *filtro;

/**
  * @brief Analiza la solicitud del cliente
  *
  * Funcion que analiza y decide que accion tomar al recibir la solicitud del cliente
  *
  * @ingroup ServidorTCP
**/
int DialogosTCP(int clientSocket, int client) {
	
	char msg[BUFFERSIZE]="";
	int bytes;
	char *ptr;
	int k=0;
	char *request;	
	char *lh = (char *)malloc(strlen(msg));
	char resp[BUFFERSIZE];

	while(1){
		bzero(msg,BUFFERSIZE);
		bzero(resp,BUFFERSIZE);
		bytes = read(clientSocket,msg,BUFFERSIZE);
		strcpy(resp,msg);
		lh = msg;
		request = strtok(lh,"\n\r");
	
		if (strcmp(request,"GetFileList") == 0) {
			FileList(clientSocket,resp);
		}
		else if(strcmp(request,"GetFile")==0) {

			FileTransfer(clientSocket,resp);
		}
		else if(strcmp(request,"GetFileInfo")==0) {
			FileInfo(clientSocket,resp);
		}
	
	}

	return 0;
}
/**
  * @brief Envia lista de archivos al cliente
  *
  * Funcion que envia la lista de archivos con los filtros de extension pedidos por el cliente
  *
  * @ingroup ServidorTCP
**/

int FileList(int clientSocket, char *msg) {
	
	DIR *dirh;
	struct dirent *dirp;
	struct dirent **list;
	int files;
	int s=0;
	char size[20]="";
	char *lista;
	char *s2;
	char s1[20]="";
	char *request,*lh;
	int bytes;
	int i=1;
	int selecc_arch(const struct dirent *entry);
	
	lh = msg;
	request = strtok(lh," \n\r");
	for(int k=0;k<3;k++){
		request = strtok(NULL," \n\r");

	}
	filtro = request;
	lista = (char *)malloc(9000);
	s2 = (char *)malloc(9000);
	strcpy(s2,"FileList\n\r");
	files = scandir(mydir,&list,selecc_arch,alphasort);
	
	
	for (i=0;i<files;i++) {
		strcat(lista,list[i]->d_name);
		strcat(lista,"\n\r");	
		
	}
	
	s=strlen(lista);
	sprintf(size,"SizeDatos: %i \n\r",s);
	sprintf(s1,"FileCount: %i \n\r",files);
	strcat(s2,s1);
	strcat(s2,size);
	strcat(s2,"\n\r");
	strcat(s2,lista);
	bytes = write(clientSocket,s2,strlen(s2));
	return 0;
	
}
/**
  * @brief Descarta archivos que no cumplan con el filtro solicitado 
  *
  * Funcion que filtra los archivos con las extensiones solicitadas por el cliente
  *
  * @ingroup ServidorTCP
**/

int selecc_arch(const struct dirent *entry)
{
	if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
		return (FALSO);
	else if(filtro =="0")
		return (VERDADERO);
	else {
		if(strstr(entry->d_name,filtro) != NULL)
			return(VERDADERO);
		else
			return(FALSO);
	}
}
/**
  * @brief Envia el archivo al cliente
  *
  * Funcion que envia el archivo solicitado al cliente
  *
  * @ingroup ServidorTCP
**/

int FileTransfer(int clientSocket, char *msg) {
	
	char dir[40]="";
	int bytes;
	char *clientFile;
	char Filename[100]="";
	FILE *file;
	char buffer[BUFFERSIZE]="";
	int bytes_read;
	int bytes_written;
	long Size;
	int Partes;
	int acpart=0;
	char *lh;
	char *ptr;
	char *request;
	char *start;
	char *end;
	char *s2;
	ptr = msg;
	lh = ptr;
	s2 = (char *)calloc(1,9000);
	request = strtok(lh," \n\r");
	for(int k=0;k<8;k++){
		if (k==1){
			clientFile = strtok(NULL," \n\r");
		} else if(k == 3){
			 start = strtok(NULL," \n\r");
		} else if(k == 5){
			end = strtok(NULL," \n\r");
		}
		else{
			request = strtok(NULL," \n\r");
		}
		lh++;
	}
	strcpy(dir,mydir);
	strcat(dir,"/");
	strcat(dir,clientFile);
	
	file = fopen(dir,"r");
	if (file == NULL) {
		perror("fopen");
		}
	else {
		fseek(file,0,SEEK_END);
		Size = ftell(file);
		rewind(file);
		float x;
		char startend[40]="";
		bzero(startend,40);
		x = (Size+0.0)/BUFFERSIZE;
		Partes = ceil(x);
		int cont = 0;
		while (1){
			cont++;
			if(cont > Partes){
				break;
			}
			bzero(buffer,BUFFERSIZE);
			bzero(s2,9000);
			acpart++;
			strcpy(s2,"File\n\rName: ");
			strcat(s2,clientFile);
			strcat(s2,"\n\r");
			sprintf(startend,"StartBytes: %s\n\rEndBytes: %s\n\r",start,end);
			strcat(s2,startend);
			sprintf(startend,"FullSize: %i\n\r",Size);
			strcat(s2,startend);
			sprintf(startend,"Part: %i\n\r",acpart);
			strcat(s2,startend);
			strcat(s2,"TotalParts: ");
			sprintf(startend,"%i\n\r",Partes);
			strcat(s2,startend);
			bytes_read = fread(buffer,1,sizeof(buffer),file);
			sprintf(startend,"SizeDatos: %i\n\r\n\r",bytes_read);
			strcat(s2,startend);
			int bytes_c = write(clientSocket,s2,strlen(s2));
			printf("\n%s\n",s2); 
			sleep(3);
			if (bytes_read == 0) {
                       		break;
                	}
	                if (bytes_read < 0) {
				//ERRORES DE LECTURA
        	                printf("\t\n** ERROR AL LEER EL ARCHIVO **\n");
				break;
        	        }			
			
			void *p=buffer;
			while(bytes_read > 0) {
				bytes_written = write(clientSocket,p,bytes_read);
				printf("\n%s\n",p);
				if (bytes_written <= 0) {
				}
				bytes_read -= bytes_written;
				p += bytes_written;
			}
		}
	
		}

	return 0;
	
}
/**
  * @brief Envia la informacion del archivo seleccionado
  *
  * Funcion que le envia al cliente el tamanio del archivo y la cadena MD5 del cliente
  *
  * @ingroup ServidorTCP
**/

int FileInfo(int clientSocket, char *msg) {
	
	int bytes;
	char buffer[BUFFERSIZE]="";
	char *lh;
	char *ptr;
	char *request;
	FILE *file;
	char *s2;
	s2 = (char *)calloc(1,9000);
	unsigned char md5_result[16];
	int i,bytes_read;
	ptr = msg;
	lh = ptr;

	request = strtok(lh," \n\r");
	for (int k=0;k<2;k++){
		request = strtok(NULL," \n\r");	
	}

	file = fopen(request,"r");
	if(file == NULL){
		printf("El archivo solicitado no existe\n");
		return 0;
	}
	
	MD5_CTX ctx; // Estructura especial para el MD5




	MD5_Init(&ctx);
	
	while ((bytes_read = fread(buffer,1,sizeof(buffer),file)) > 0) {
		MD5_Update(&ctx, buffer, bytes_read); } // Esta linea la puede llamar N veces

	

	MD5_Final(md5_result, &ctx);
	
	char tmp[2]="";
	char *md5str;
	printf("MD5: ");
	for(i = 0; i < 16; i++) {
		sprintf(tmp,"%02hhx",md5_result[i]);
		strcat(md5str,tmp);
	} 		
	
	strcpy(s2,"FileInfo\n\rName: ");
	strcat(s2,request);
	strcat(s2,"\n\r");
	strcat(s2,"FullSize: 16\n\r");
	strcat(s2,"MD5: ");
	strcat(s2,md5str);
	strcat(s2,"\n\r\n\r");
	printf("%s\n",s2);
	int bytes_c = write(clientSocket,s2,strlen(s2));
	return 0;
	
}




