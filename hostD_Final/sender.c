#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h>     
#include <string.h>     
#include <unistd.h>     

int main(int argc, char *argv[])
{
    int bcSock;  
    int size=0;
    int nlista=0;                     
    struct sockaddr_in broadcastAddr; 
    char *broadcastIP;                
    unsigned short broadcastPort;     
    char *sendString;                 
    int broadcastPermission;          
    unsigned int sendStringLen; 
    int status;
    int i;
    
    
    //////////
    pid_t ForkDiscover;
    char *ip[17], *listahost[20];
    
    //int var_lcl = 0;
	
    if (argc < 4)                     
    {
        fprintf(stderr,"Usage:  %s <IP Address> <Port> <Send String>\n", argv[0]);
        exit(1);
    }

    broadcastIP = argv[1];            /* First arg:  broadcast IP address */ 
    broadcastPort = atoi(argv[2]);    /* Second arg:  broadcast port */
    sendString = argv[3];             /* Third arg:  string to broadcast */

	bcSock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(bcSock == -1) {
		fprintf(stderr,"Can't create Socket");
		return 1;
	}
 
//////////////////////// COmenzamos con el fork

 ForkDiscover = fork();

    if(ForkDiscover >= 0) // fork was successful
    {
        if(ForkDiscover == 0) // child process
        {
        // Debera enviar en Brodcast un mensaje cada 10 segundos hacia el puerto UDP/5000 con el mensaje "WHO [su_nombre] 		     [puerto_respuesta]\r\n\r\n"
        
        //bcSock = socket (PF_INET,SOCK_DGRAM,IPPROTO_UDP);	// socket de broadacast
	//bind (bcSock, (struct sockaddr *) &udpClient, sizeof (udpClient));// enlace de un wey que quieres que te conteste
	//sendto (bcSock,"Who", strlen("Who"), 0, (struct sockaddr*) &servaddr, sizeof (servaddr));// ANTES
        } ///Termina el proceso hijo
        else //Parent process
        {
        
// Debera estar escuchando respuestas de los posibles RECEPTORES en el UDP/[puerto respuesta] y a cada mensaje que reciva debera validar antes de agregarlos a la lista de HOSTS CONOCIDOS
    //          1) Que no sea el mismo.
    //          2) Que no exista ya en al lista de HOSTS CONOCIDOS.   
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
        }
    }
    else // fork failed
    {
        printf("\n Fork failed, quitting!!!!!!\n");
        return 1;
    }
    return 0;
//}	
/////////////////////////////////////////////////	

    broadcastPermission = 1;
    status = setsockopt(bcSock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission));
    if(status == -1) {
		fprintf(stderr,"Can't set Brodcast Option");
		return 1;
    }
    
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   
    broadcastAddr.sin_family = AF_INET;                 
    inet_pton(AF_INET,broadcastIP,&broadcastAddr.sin_addr.s_addr); 
    broadcastAddr.sin_port = htons(broadcastPort);         
    sendStringLen = strlen(sendString);     
	for(i=0; i<100; i++) {
		status = sendto(bcSock,sendString,sendStringLen,0,(struct sockaddr*)&broadcastAddr, sizeof(broadcastAddr));    
		printf("Send %i bytes to brodcast addr\n",status);
        sleep(3);   
    }
  }
}
