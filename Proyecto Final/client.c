// Sahara Montes de Oca y Fatima De Niz
// Copia de Alvaro Parres
//Ya sirve
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "netlib/tcp.h"
#include "utils/debug.h"


#define	MAXLINE 4096

//Prototipos Locales
void start_protocol(const int clientSocket,const char *remoteFilename,const char *localFilename);
int sendLine(const int clientSocket, const char* writeBuffer);
void doGet(int socket, const char *localFilename, unsigned long size);
u_long currentTimeMillis();

void dg_cli(FILE *, int, const struct sockaddr *, socklen_t);
char *sock_ntop_host(const struct sockaddr *, socklen_t);

void
dg_cli(FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
	int	n;
	const int on = 1;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t len;
	struct sockaddr	*preply_addr;
	preply_addr = (struct sockaddr *) malloc(servlen);
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	strcpy(sendline, "hola\n\0");
        
	for (;;) {
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		for (; ;) {
			len = servlen;
			n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
			if (n < 0) {
				if (errno == EINTR)
					break;		/* waited long enough for replies */
				else
					puts("recvfrom error");
			} else {
				recvline[n] = 0;	/* null terminate */
				printf("from %s: %s", sock_ntop_host(preply_addr, len), recvline);
			}
		}
	}
	free(preply_addr);
}

char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
    static char str[128];		/* Unix domain is largest */
	struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

	if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
		return(NULL);
	return(str);
}

 
//Funcion start
int start_client(const u_short port, const char ip[], const char remoteFilename[], const char localFilename[]) {
	int socketf;
	socketf = newTCPClientSocket4(ip, port);
	int	sockfd;
	pid_t pid;
	struct sockaddr_in servaddr;

	//if (argc != 2) {
	//	puts("usage: cli <IPaddress>");
	//	exit(1);
	//}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6000);
	inet_pton(AF_INET, "192.168.1.255", &servaddr.sin_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	

	if ((pid = fork()) == 0) { 
		dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
		exit(0);
	}

	
	if(socketf == -1) {
		return 1;
	}
	
	start_protocol(socketf,remoteFilename,localFilename);
	closeTCPSocket(socketf);
	debug(2,"Close connection (%s:%u)",ip,port);
	sleep(2);
	return 0;
			
}

//Funcion GET list
int get_list(const u_short port, const char ip[]) {

	int socketf;
	char *list;
	char *readBuffer = (char *) malloc(255); 
	char *writeBuffer = (char *) malloc(255);
	int readBytes;
	char *ptr;
	
	socketf = newTCPClientSocket4(ip, port);
	
	if(socketf == -1) {
		return 1;
	}
	
	sprintf(writeBuffer,"LIST\r\n\r\n");
	sendLine(socketf,writeBuffer);	
	
	//LEEMOS LA RESPUESTA AL GET
	list = (char *) malloc(254);
	while((readBytes = read(socketf,readBuffer,1))>0) {
		list = (char *) realloc(list,strlen(list)+readBytes+1);
		strncat(list,readBuffer,readBytes);
		ptr = list+(strlen(list)-4);
		if(strcmp(ptr,"\r\n\r\n")==0) {
			*ptr = '\0';
			break;
		}
	}	
	
	printf("%s",list);

	free(list);
	free(readBuffer);
	free(writeBuffer);
	
	debug(2,"Close connection (%s:%u)",ip,port);
	closeTCPSocket(socketf);
	
	for(;;);
	return 0;
}

//Funcion protocolo
void start_protocol(const int socketf,const char *remoteFilename,const char *localFilename) {

	char writeBuffer[1024];
	char readBuffer[10240];
	int readBytes;
	char *cmdString;
	char *freeCmdString;
	char *ptr;
	char *token;

	sprintf(writeBuffer,"GET %s\r\n\r\n",remoteFilename);
	sendLine(socketf,writeBuffer);
	
	//LEEMOS LA RESPUESTA AL GET
	cmdString = (char *) malloc(254);
	while((readBytes = read(socketf,readBuffer,1))>0) {
		cmdString = (char *) realloc(cmdString,strlen(cmdString)+readBytes+1);
		strncat(cmdString,readBuffer,readBytes);
		ptr = cmdString+(strlen(cmdString)-4);
		if(strcmp(ptr,"\r\n\r\n")==0) {
			*ptr = '\0';
			break;
		}
	}
	
	freeCmdString = cmdString;
	
	debug(4,"Recibimos: %s\n",cmdString);
	token = strsep(&cmdString,"\r");
	cmdString++; // Eliminamos el \n de igual manera.
	
	if(strcmp(token,"NOT_FOUND")==0) {
		debug(1,"ERROR: Archivo no encontrado(%s)",token);
		return;
	} else if(strcmp(token,"OK")==0) {
		token = strstr(cmdString,":");
		doGet(socketf,localFilename,strtol(token+2,NULL,10));
	} else {
		debug(1,"ERROR UNKOWN ANSWER (%s)",token);
	}
	
	sendLine(socketf,"QUIT\r\n\r\n");
	free(freeCmdString);
}

//Funcion do GET
void doGet(int socketf, const char *localFilename, unsigned long size) {

	u_int writeBytes = 0;
	char *readBuffer;
	u_int readBytes = 0;
	u_long totalReadBytes = 0;
	int fd;
	int localError;
	u_long start;
	u_long end;
		
	debug(4,"Doing Get for: %s (%li)",localFilename,size);
	fd = open(localFilename,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
	if(fd == -1) {
		//Error al crear el archivo local.
		localError = errno;
		debug(1,"Can't create Local File (%s)",strerror(localError));
		return;
	}
	
	readBuffer = (char *) malloc(102400);
	start = currentTimeMillis();	
	while(totalReadBytes < size) {
		readBytes = read(socketf,readBuffer,102400);
		if(readBytes > 0) {
			writeBytes = 0;
			while(writeBytes < readBytes) {
				writeBytes += write(fd,readBuffer+writeBytes,readBytes-writeBytes);
			}
		}
		totalReadBytes += readBytes;
		end = currentTimeMillis();
		if(end>start)
			debug(4,"Get %lu/%lu ( %0.0f%% ) speed %lukbps",totalReadBytes,size,(((float)totalReadBytes/(float)size)*100),(totalReadBytes/(end-start)*1000/1024));		
	}	
	
	free(readBuffer);
	close(fd);

}

//Funcion sendline
int sendLine(const int clientSocket, const char* writeBuffer) {
	u_int writeBytes = 0;
	
	do { 
		writeBytes += write(clientSocket,writeBuffer+writeBytes,strlen(writeBuffer)-writeBytes);
	} while(writeBytes < strlen(writeBuffer));
	debug(4,"-> %s",writeBuffer);	

	return writeBytes;
}

u_long currentTimeMillis() {
        u_long t;
        struct timeval tv;
        gettimeofday(&tv, 0);
        t = (tv.tv_sec*1000)+tv.tv_usec;
        return t;
}
