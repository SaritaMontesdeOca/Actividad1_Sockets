
// Cliente Filetransfer

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_PORT 8080
#define BUFFSIZE 8096

void str_cli (int);

int
main (int argc, char **argv)
{
	int	socketfd;
	struct sockaddr_in	serv_addr;

	socketfd = socket (AF_INET, SOCK_STREAM, 0);

	bzero (&serv_addr, sizeof (serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons (SERV_PORT);
	inet_pton (AF_INET, "localhost", &serv_addr.sin_addr);

	connect (socketfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));

	str_cli (socketfd);
	
	close (socketfd);
	
	sleep(1);
	exit (0);
}

void
str_cli (int sockfd)
{
	long ret;	
	char send_line[BUFFSIZE], recv_line[BUFFSIZE];

	fgets (send_line, BUFFSIZE, stdin);
	write (sockfd, send_line, strlen (send_line));
	while ((ret = read (sockfd, recv_line, BUFFSIZE)) > 0) {
		if (ret > 0 && ret < BUFFSIZE) {
			recv_line[ret] = 0;
		} else {
			recv_line[0] = 0;
		}
		fputs (recv_line, stdout);
	}
}
