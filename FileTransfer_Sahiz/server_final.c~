/**

  @file Final_Server.c
  @brief Este programa atiende las peticiones de los clientes las cuales estan especificadas en el protocolo  

  Responde a 3 diferentes tipos de mensajes, "GetGileList", "GetFile" y "GetFileInfo" 

  @author Edgar Rodolfo Lopez R.
  @date 5/05/2012

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "md5.h"

#define MSGSIZE 8096
#define BUFFERSIZE 100000
#define PARTSIZE 1024.0
#define PORT 5000
#define LISTENQ 23


void file (int, char *, char *, char *);
void fileinfo (int, char *);
void filelist (int);
int parse (int);

int
main (void)
{
  int listenfd, socketfd, pid;
  size_t length;
  static struct sockaddr_in serv_addr;
  static struct sockaddr_in cli_addr;

  // Se crea el socket de escuaha
  listenfd = socket (AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
  serv_addr.sin_port = htons (PORT);

  // Se asociamos la direccion con el socket
  bind (listenfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));

  listen (listenfd, LISTENQ);

  // Creamos los hijos(fork) para manejar las solicitudes 
  for (; ;) {
    length = sizeof (cli_addr);
    socketfd = accept (listenfd, (struct sockaddr *) &cli_addr, &length);
    if ((pid = fork ()) == 0) {
      close (listenfd);
      parse (socketfd);
      exit (0);
    } else {
      close (socketfd);	
    }
  }

  // close (socketfd);

  sleep (1);
  return 0;
}

/**

@brief Esta funcion descompone el mensaje ya recivido para asi determinar que funcion se va a ejecutar en 
respuesta a dicho mensaje

@param Recive el Socket atravez del cual se comunica con el cliente 

@returns Devuelve un entero indicando si se pudo procesar el mensaje

*/

int
parse (int socketfd)
{
  // char message[] = "GetFile\n\rName: x.tmp\n\rStartByte: 0\n\rEndByte: 11\n\r";
  const char *delimiter = "\n\r";
  char *command, *name_str, *startbytes_str, *endbytes_str,
    *filter_str, *name, *filter, *fullsize_str, *md5_str, *md5, *token,
    *startbytes, *endbytes;
  unsigned long fullsize, ret;
  char message[200], *ps;

  ret = read (socketfd, message, BUFFERSIZE);

  // Si leemos un numero de bytes menor al tamano del buffer, pongo el caracter de terminacion
  if (ret > 0 && ret < BUFFERSIZE) {
    message[ret] = '\0';
  } else {
    message[0] = '\0';
  }

  command = strtok (message, delimiter);
  if (command == NULL)
    return 0;

  if (strcmp (command, "GetFile") == 0) {
    name_str = strtok (NULL, delimiter);
    startbytes_str = strtok (NULL, delimiter);
    endbytes_str = strtok (NULL, delimiter);

    strtok (name_str, " ");
    name = strtok (NULL, " ");
    if (name == NULL)
      return 0;
    // printf ("%s %s\n", command, name);

    strtok (startbytes_str, " ");
    startbytes = strtok (NULL, " ");
    if (startbytes == NULL)
      return 0;
    // startbytes = strtol (token, NULL, 0);
    // printf ("%s\n", startbytes);

    strtok (endbytes_str, " ");
    endbytes = strtok (NULL, " ");
    if (endbytes == NULL)
      return 0;
    // endbytes = strtol (token, NULL, 0);
    // printf ("%s\n", endbytes);

    file (socketfd, name, startbytes, endbytes);


  } else if (strcmp (command, "GetFileList") == 0) {
    filter_str = strtok (NULL, delimiter);

    strtok (filter_str, " ");
    filter = strtok (NULL, " ");
    if (filter == NULL)
      return 0;

    filelist (socketfd);
  } else if (strcmp (command, "GetFileInfo") == 0) {
    name_str = strtok (NULL, delimiter);
    strtok (name_str, " ");
    name = strtok (NULL, " ");
    if (name == NULL)
      return 0;
    printf ("%s %s\n", command, name);

    fileinfo (socketfd, name);
  }

  /*if (strcmp (command, "FileList") == 0) {
    filecount_str = strtok (NULL, delimiter);
    sizedatos_str = strtok (NULL, delimiter);
    datos = strtok (NULL, delimiter);

    strtok (filecount_str, " ");
    filecount = strtol (strtok (NULL, " "), NULL, 0);

    strtok (sizedatos_str, " ");
    sizedatos = strtol (strtok (NULL, " "), NULL, 0);
    } else if (strcmp (command, "File") == 0) {*/
  return 1;
}

/**

@brief Es la funcion que responde al comando "GetFile" preparando un mensaje segun lo especificado en el 
protocolo y a su ves mandando el archivo correspondiente 

@param Recive el Socket, el nombre del archivo y los Bytes inicial y final 

@returns (Void)

*/

void
file (int socketfd, char *name, char *start, char *end)
{
  int fp;
  unsigned int total_parts, part_number;
  int c;
  long size, start_l, end_l, r, i;
  char ltostr[200], msj[MSGSIZE] = "File\n\r", buffer[BUFFERSIZE];
  struct stat st;

  strcat (msj, "Name: ");
  strcat (msj, name);
  strcat (msj, "\n\r");
  strcat (msj, "StartBytes: ");
  strcat (msj, start);
  strcat (msj, "\n\r");
  strcat (msj, "EndBytes: ");
  strcat (msj, end);
  strcat (msj, "\n\r");
  strcat (msj, "FullSize: ");

  start_l = strtol (start, NULL, 0);
  end_l = strtol (end, NULL, 0);

  fp = open (name, O_RDONLY);
  if (fp < 0) {
	printf ("No se pudo abrir el archivo.\n");
	return;
  }



  stat (name, &st);
  size = st.st_size;

  snprintf (ltostr, 200, "%d", size);
  strcat (msj, ltostr);

  size = end_l - start_l;

  total_parts = (long) ceil(size / PARTSIZE);
  strcat (msj, "\n\r");
  strcat (msj, "TotalParts: ");
  snprintf (ltostr, 200, "%d", total_parts);
  strcat (msj, ltostr);
  strcat (msj, "\n\r");
  strcat (msj, "SizeDatos: ");
  snprintf (ltostr, 200, "%d", size);
  strcat (msj, ltostr);
  strcat (msj, "\n\r\n");

  part_number = 0;

  printf ("%s", msj);
  fflush (stdout);

  r = read (fp, buffer, BUFFERSIZE);
  //for (i = 0; i <= r; i++) {
    // putc (buffer[i], stdout);
     //fflush (stdout);
  //}
  // buffer[r] = '\0';
  // buffer[r+1] = '\0';
  // printf ("%s", buffer);
  // fflush (stdout);
  write (socketfd, msj, strlen (msj));
  write (socketfd, buffer, strlen (buffer));

  // close (socketfd);
}

/**

@brief Es la funcion que responde al comando "GetFileInfo" preparando un mensaje segun lo especificado en el 
protocolo y a su ves mandando la informacion del archivo 

@param Recive el Socket y el nombre del archivo 

@returns (Void)

*/
void
fileinfo (int socketfd, char *name)
{
  char msj[MSGSIZE], size_str[200];
  long size;
  struct stat st;

  strcpy (msj, "FileInfo\n\r");
  strcat (msj, "Name: ");
  strcat (msj, name);
  strcat (msj, "\n\r");
  strcat (msj, "FullSize: ");

  stat (name, &st);
  size = st.st_size;
  snprintf (size_str, 200, "%d", size);

  strcat (msj, size_str);
  strcat (msj, "\n\r");
  strcat (msj, "MD5: ");
  strcat (msj, MDFile (name));
  strcat (msj, "\n\r");

  write (socketfd, msj, strlen (msj));

  // close (socketfd);
}

/**

@brief Es la funcion que responde al comando "GetFileList" preparando un mensaje segun lo especificado en el 
protocolo y a su ves mandando la lista de archivos

@param Recive el Socket 

@returns (Void)

*/
void
filelist (int socketfd)
{
  char msj[MSGSIZE], size_str[200];
  char *list = "A.h\n\B.txt\n\C.png\n";
  
  strcpy (msj, "FileList\n\r");
  strcat (msj, "FileCount: 3\n\r");
  strcat (msj, "SizeDatos: ");

  snprintf (size_str, 200, "%d", strlen (list));
  strcat (msj, size_str);
  strcat (msj, "\n\r");

  write (socketfd, msj, strlen (msj));
  write (socketfd, list, strlen (list));

  // close (socketfd);
}
