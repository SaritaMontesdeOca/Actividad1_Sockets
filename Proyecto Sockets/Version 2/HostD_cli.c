/**

  @file HostD_cli.c
  @brief Este programa envia por broadcast un mensaje indicando a los servidores de esta red que 
   la maquina donde se ejecuta el programa esta disponible  
 

  @author Edgar Rodolfo Lopez R.
  @date 5/05/2012

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5001
#define MAXLINE 1024

void BrCast (int, const struct sockaddr *, socklen_t);

int
main (void)
{
	int sockfd;
	struct sockaddr_in servaddr;

	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons (PORT);
	inet_pton (AF_INET, "192.168.1.255", &servaddr.sin_addr);

	sockfd = socket (AF_INET, SOCK_DGRAM, 0);

	for(;;){
	BrCast (stdin, sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

	sleep(30);
	}
	close (sockfd);

	sleep (1);
	exit (0);
}

/**

@brief Esta funcion es la que se encarga de generar el mensaje a enviar y a su vez enviarlo 

@param Recive el Socket, la estructura con informacion de conexion y la longitud de dicha estructura

@returns (void)

*/

void
BrCast (int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)
{
	int n;
	const int on = 1;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t len;
	struct sockaddr	*preply_addr;
 
	preply_addr = (struct sockaddr *) malloc (servlen);

	setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof (on));

	//fgets (sendline, MAXLINE, fp);
	sendto (sockfd, "Hello Calvo\n", strlen ("Hello Calvo\n"), 0, pservaddr, servlen);
	len = servlen;
	n = recvfrom (sockfd, recvline, MAXLINE, 0, preply_addr, &len);
	if (n > 0) { 
		recvline[n] = '\0';
		printf ("%s\n", recvline);
	}
}

