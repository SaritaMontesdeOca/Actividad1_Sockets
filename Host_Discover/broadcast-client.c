//Sahara MOntes de Oca
//Fatima de Niz
//Host discover prueba 4 - Client

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

void BrCast (FILE *, int, const struct sockaddr *, socklen_t);  //Prototipo de funcion

int main (void) //Main
{
	int cliport;
	int sockfd; //soceket del cliente
	struct sockaddr_in servaddr; //SOy esta ip y quiero conectarme a este server
 	static struct sockaddr_in cliaddr; //structura del client
	bzero (&servaddr, sizeof (servaddr));//sets the first n bytes of the byte area starting at s to zero (bytes containing '\0').
        servaddr.sin_family = AF_INET; //ipv4
	servaddr.sin_port = htons (PORT); //puerto
	
	char recvline[MAXLINE + 1],ip[INET_ADDRSTRLEN];	
	
	//string a structura		
	inet_pton (AF_INET, "192.168.1.255", &servaddr.sin_addr); //antes
	
	sockfd = socket (AF_INET, SOCK_DGRAM, 0);	// antes
	
	//BrCast (stdin, sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)); //antes
	
	bind (sockfd, (struct sockaddr *) &cliaddr, sizeof (cliaddr));// enlace de un wey que quieres que te conteste
	sendto (sockfd,"Hello", strlen("Hello"), 0, (struct sockaddr*) &servaddr, sizeof (servaddr));// ANTES
	
	
	inet_ntop(AF_INET, &(servaddr.sin_addr), ip, INET_ADDRSTRLEN); //transforma struct a string
	cliport = ntohs (servaddr.sin_port);
	printf ("encontrado desde %s : %i %s", ip, cliport, recvline);		

	
	
	close (sockfd);
	sleep (1);
	exit (0);
}



void BrCast (FILE *fp, int sockfd, const struct sockaddr *pservaddr, socklen_t servlen)  //Funcion Broadcast
{
	int n;
	const int on = 1;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t len;
	struct sockaddr	*preply_addr;
	preply_addr = (struct sockaddr *) malloc (servlen);
	setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof (on));
	fgets (sendline, MAXLINE, fp);
	sendto (sockfd, sendline, strlen (sendline), 0, pservaddr, servlen);
	
	
	len = servlen;
	n = recvfrom (sockfd, recvline, MAXLINE, 0, preply_addr, &len);
	if (n > 0) { 
		recvline[n] = '\0';
		printf ("%s\n", recvline);
	}
}
/*
		inet_ntop(AF_INET, &(cliaddr.sin_addr), ip, INET_ADDRSTRLEN); //transforma struct a string
		cliport = ntohs (cliaddr.sin_port);
		printf ("encontrado desde %s:%d %s", ip, cliport, recvline);
		
*/
