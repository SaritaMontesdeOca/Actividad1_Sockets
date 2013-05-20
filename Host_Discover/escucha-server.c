//Sahara MOntes de Oca
//Fatima de Niz
//Host discover prueba 4 - Server

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5001 //Puerto
#define MAXLINE 1024

int main (void) //Main client 
{
////////////
	char ip[17], *listahost[20];

///////////
	int n, cliport, sockfd;
	int i;
	int size;
	int nlista;
	socklen_t cliaddrlen;
	char recvline[MAXLINE + 1]; // ip[INET_ADDRSTRLEN];
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
		n = recvfrom (sockfd, recvline, MAXLINE, 0,(struct sockaddr *) &cliaddr, &cliaddrlen);		

		//Guardar los host conocidos
		for (i=0; i < size; i++)
		{
		if(strcmp(listahost[i],ip)==0){
		nlista=1;
		break;		
		}
		if (!nlista){
		listahost[size] = (char *) malloc(17);
		strcpy(listahost[size],ip);
		size++;
		}
			}		
		//////////

		
		if (n > 0) {
			recvline[n] = '\0';
			inet_ntop(AF_INET, &(cliaddr.sin_addr), ip, INET_ADDRSTRLEN);
			cliport = ntohs (cliaddr.sin_port);
			printf ("encontrado desde %s:%d %s", ip, cliport, recvline);
			sendto (sockfd,"Hello", strlen("Hello"), 0 , (struct sockaddr*) &cliaddr, sizeof (cliaddr));
		}
	}
}
