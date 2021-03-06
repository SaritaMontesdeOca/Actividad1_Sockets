/**

  @file final_cliente.c
  @brief Este programa presenta una interface de linea de comandos al usuario en la cual puedes producir los comandos 
   especificados en el protocolo de tal manera que se pueden recivir archivos e informacion del archivo. 

  Podemos especificar exactamente 3 comandos, el "getfilelist" el cual nos regresa la lista de archivos que queremos 
  solicitar al servidor, el "getfile" en el cual solicitamos un archivo especificado, y "getfileinfo" donde solicitamos 
  informacion de un archivo. 

  @author Edgar Rodolfo Lopez R.
  @date 5/05/2012

*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PROMPT "> "
#define PORT 5000
#define NUMARGS 10
#define MAXHEAD 500
#define MAXLINE 8096
#define MAXMESSAGE 1024

int DOgetfile (void);
int DOgetfileinfo (void);
int DOgetfilelist (void);
int readline (char *, int);
int getmessage (int, char *, int);
static void makeargs (char *);

//Asoc string con una funcion
struct commands {
  char *name;
  int (*func) (void);
};

static struct commands commands[] = {
  "getfile",      DOgetfile,
  "getfileinfo",  DOgetfileinfo,
  "getfilelist",  DOgetfilelist,
  "",            (int (*) ()) 0
};

char *cmdargv[NUMARGS];
int cmdargc;
int socketfd;

int
main (void)
{
  int s;
  struct commands *cmd;
  static char buffer[MAXLINE], message[MAXMESSAGE];

  s = 0;
  while (s >= 0)
    {
      printf (PROMPT);
      s = readline (buffer, MAXLINE);
      if (s < 0)
	break;

      makeargs (buffer);

      if (cmdargc == 0)
	continue;

      for (cmd = commands; *cmd->name != '\0'; cmd++)
	if (strcmp (cmdargv[0], cmd->name) == 0)
	  break;

      if (*cmd->name != '\0')
	s = (*cmd->func) ();
      else
	{
	  s = 0;
	  printf ("Command \"%s\" not recognized.\n", cmdargv[0]);
	}
    }

  close (socketfd);

  sleep (1);
  return (0);
}
/**

@brief Almacena en un buffer lo que el usuario introdujo a partir de la entrada estandar



@param Recive buff: Donde se almacenan los datos que introdujo el usuario 
len: Es el espacio reservado del buffer en memoria



@returns Devuelve un entero que dice si se pudo leer o no la entrada del usuario



*/
int
readline (char *buff, int len)
{
  char *p;

  if (fgets (buff, len, stdin) == NULL)
    {
      printf ("\nBye!\n");
      return (-1);
    }

  p = buff + strlen (buff) - 1;
  while (p >= buff)
    if (*p == '\r' || *p == '\n' || isspace (*p))
      *p-- = '\0';
    else
      break;

  return (0);
}
/**

@brief Descompone el comando que itrodujo el usiario en diferentes componentes para ser procesados por las funciones

@param Recibe el buffer donde esta almacenado dicho comando

@returns Static Void

*/
static void
makeargs (char *buff)
{
  int i;
  char *p;

  for (i = 0; i < NUMARGS; i++)
    cmdargv[i] = (char *) 0;

  p = buff + strlen (buff) - 1;
  while (p >= buff)
    if (*p == '\r' || *p == '\n' || isspace (*p))
      *p-- = '\0';
    else
      break;

  p = buff;
  cmdargc = 0;
  while (cmdargc < NUMARGS)
    {
      while (*p && isspace (*p))
	p++;

      if(*p == '\0')
	break;

      cmdargv[cmdargc++] = p;

      while (*p && !isspace (*p))
	{
	  if (cmdargc == 1)
	    *p = tolower (*p);
	  p++;
	}

      if (*p == '\0')
	break;

      *p = '\0';
      p++;
    }
}

/**

@brief Crea el mensaje correspondiente al comando "GetFile" para ser enviado al servidor, asi mismo recive la respuesta del servidor junto con los datos que este envia, es decir, el archivo 

@pram (void)

@returns Entero que indica si hubo o no exito en la ejecucion de la funcion

*/

int
DOgetfile (void)
{
  int s, fd;
  struct sockaddr_in servaddr;
  char *ip, *file, *localfile, *start = "0", *end = "0";
  static char sendline[MAXLINE], line[MAXLINE],
    recvline[MAXLINE], head[MAXHEAD];


  ip = cmdargv[2];

  socketfd = socket (AF_INET, SOCK_STREAM, 0);

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (PORT);
  inet_pton (AF_INET, ip, &servaddr.sin_addr);

  connect (socketfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

  file = cmdargv[1];

  if (cmdargc < 2)
    {
      printf ("Remote File: ");
      s = readline (line, sizeof (line));
      if (s < 0)
	return (-1);
      file = line;
    }

  fd = open (file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (fd < 0)
    {
      printf ("Could not open local file %s. Error %s\n",
	      localfile, strerror (errno));
      return (0);
    }

  strcpy (sendline, "GetFile\n\r");
  strcat (sendline, "Name: ");
  strcat (sendline, file);
  strcat (sendline, "\n\r");
  strcat (sendline, "StartBytes: ");
  strcat (sendline, start);
  strcat (sendline, "\n\r");
  strcat (sendline, "Endbytes: ");
  strcat (sendline, end);
  strcat (sendline, "\n\r");
  strcat (sendline, "SizeDatos: 0\n\r");

  write (socketfd, sendline, strlen (sendline));
  read (socketfd, head, MAXHEAD);

  while ((s = read (socketfd, recvline, MAXLINE)) > 0)
    write (fd, recvline, s);

  close (fd);
  close (socketfd);

  return (s);
}

/**

@brief Crea el mensaje correspondiente al comando "GetFileInfo" para ser enviado al servidor, asi mismo recive la respuesta del servidor la cual consiste en la informacion del archivo

@pram (void)

@returns Entero que indica si hubo o no exito en la ejecucion de la funcion

*/

int
DOgetfileinfo (void)
{
  int s;
  char *ip, *file;
  static char line[MAXLINE], sendline[MAXLINE], head[MAXHEAD];
  struct sockaddr_in servaddr;

  ip = cmdargv[2];

  socketfd = socket (AF_INET, SOCK_STREAM, 0);

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (PORT);
  inet_pton (AF_INET, ip, &servaddr.sin_addr);

  connect (socketfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

  file = cmdargv[1];

  if (cmdargc < 2)
    {
      printf ("Remote File: ");
      s = readline (line, sizeof (line));
      if (s < 0)
	return (-1);
      file = line;
    }

  strcpy (sendline, "GetFileInfo\n\r");
  strcat (sendline, "Name: ");
  strcat (sendline, file);
  strcat (sendline, "\n\r");
  strcat (sendline, "SizeDatos: 0\n\r");

  write (socketfd, sendline, strlen (sendline));
  s = read (socketfd, head, MAXHEAD);
  printf ("%s", head);

  close (socketfd);

  return (s);
}

/**

@brief Crea el mensaje correspondiente al comando "GetFileList" para ser enviado al servidor, asi mismo recive la respuesta del servidor junto con los datos que este envia, es decir, la lista de archivos

@pram (void)

@returns Entero que indica si hubo o no exito en la ejecucion de la funcion

*/

int
DOgetfilelist (void)
{
  int s;
  char *ip, *filter;
  static char line[MAXLINE], sendline[MAXLINE], head[MAXHEAD],
    recvline[MAXLINE];
  struct sockaddr_in servaddr;

  ip = cmdargv[2];

  socketfd = socket (AF_INET, SOCK_STREAM, 0);

  bzero (&servaddr, sizeof (servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons (PORT);
  inet_pton (AF_INET, ip, &servaddr.sin_addr);

  connect (socketfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

  filter = cmdargv[1];

  strcpy (sendline, "GetFileList\n\r");
  strcat (sendline, "Filter: ");
  strcat (sendline, filter);
  strcat (sendline, "SizeDatos: 0\n\r");

  write (socketfd, sendline, strlen (sendline));
  read (socketfd, head, MAXHEAD);

  // printf ("%s", head);

  s = read (socketfd, recvline, MAXLINE);
  printf ("%s", recvline);

  close (socketfd);

  return (s);
}
