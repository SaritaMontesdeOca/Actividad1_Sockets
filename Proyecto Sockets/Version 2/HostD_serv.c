/**

  @file HostD_serv.c
  @brief Este programa se encarga de escuchar todos los paquetes que envian los clientes de la red 
   asi como presentar las IP que pertenecen a los paquetes  
 

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

int
main (void)
{
	int n, cliport, sockfd;
	socklen_t cliaddrlen;
	char recvline[MAXLINE + 1], ip[INET_ADDRSTRLEN];
	static struct sockaddr_in servaddr;
	static struct sockaddr_in cliaddr;

	sockfd = socket (AF_INET, SOCK_DGRAM, 0);

	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (PORT);

	bind (sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

	for (; ;) {
		cliaddrlen = sizeof (cliaddr);
		n = recvfrom (sockfd, recvline, MAXLINE, 0,
				(struct sockaddr *) &cliaddr, &cliaddrlen);
		if (n > 0) {
			recvline[n] = '\0';
			inet_ntop(AF_INET, &(cliaddr.sin_addr), ip, INET_ADDRSTRLEN);
			cliport = ntohs (cliaddr.sin_port);
			printf ("desde %s:%d %s", ip, cliport, recvline);
			sendto (sockfd,"Hi Calvo server\n", strlen("Hi Calvo server\n"), 0
					, (struct sockaddr*) &cliaddr, sizeof (cliaddr));
		}
	}
}
