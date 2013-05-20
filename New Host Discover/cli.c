#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#define	MAXLINE 4096

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

	while (fgets(sendline, MAXLINE, fp) != NULL) {
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

char *
sock_ntop_host(const struct sockaddr *sa, socklen_t salen)
{
	static char str[128];		/* Unix domain is largest */
	struct sockaddr_in *sin = (struct sockaddr_in *) sa;

	if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
		return(NULL);
	return(str);
}


int
main(int argc, char **argv)
{
	int	sockfd;
	struct sockaddr_in servaddr;

	if (argc != 2) {
		puts("usage: cli <IPaddress>");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5000);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	dg_cli(stdin, sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	exit(0);
}

