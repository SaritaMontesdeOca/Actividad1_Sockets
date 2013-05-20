
// Server Filetransfer

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define LISTENQ 64
#define BUFFERSIZE 8096

void send_file (int);

int
main (int argc, char **argv)
{
	int listenfd, socketfd, pid;
	size_t length;
	static struct sockaddr_in serv_addr;
	static struct sockaddr_in cli_addr;

	listenfd = socket (AF_INET, SOCK_STREAM, 0);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (PORT);
	
	bind (listenfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
	
	listen (listenfd, LISTENQ);
	
	for (; ;) {
		length = sizeof (cli_addr);
		socketfd = accept (listenfd, (struct sockaddr *) &cli_addr, &length);
		if ((pid = fork ()) == 0) {
			close (listenfd);
			send_file (socketfd);
		} else {
			close (socketfd);
		}
	}
}

void
send_file (int sockfd)
{
	int file_fd;
	long ret, trt, size;
	static char buffer[BUFFERSIZE+1];
	struct stat st;


	ret = read (sockfd, buffer, BUFFERSIZE);
	if (ret > 0 && ret < BUFFERSIZE) {
		buffer[ret-1] = 0;
	} else {
		buffer[0] = 0;
	}

	file_fd = open (buffer, O_RDONLY);
	fstat (file_fd, &st);
	size = st.st_size;

	while ((ret = read (file_fd, buffer, BUFFERSIZE)) > 0) {
		do { 
			trt = write (sockfd, buffer, ret);
			ret -= trt;
		} while (ret > 0);
	} 


	close (file_fd);
	sleep(1);
	exit(1);
}
