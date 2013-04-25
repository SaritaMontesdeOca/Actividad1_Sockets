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

#define PORT 5000 //Puerto
#define MAXLINE 1024

void BrCast (FILE *, int, const struct sockaddr *, socklen_t);  //Prototipo de funcion

int main (void) //Main
{
	int sockfd;
	struct sockaddr_in servaddr;

	bzero (&servaddr, sizeof (servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons (PORT);
	inet_pton (AF_INET, "192.168.1.255", &servaddr.sin_addr);

	sockfd = socket (AF_INET, SOCK_DGRAM, 0);

	BrCast (stdin, sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

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

