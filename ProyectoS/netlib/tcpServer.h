/**
 * @file tcpServer.h
 *
 * @brief Funciones Generales para manejo de servidores TCP
 *
 * Funciones para controlar un servidor TCP
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Febrero 2012
 * @version 0.0.1
 *
 * \defgroup sockets Funciones Referentes a Sockets
**/

#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H 

/**
  * @brief Crea un nuevo servidor TCP en IPv4.
  *
  * Crea un nuevo Servidor TCP utilizando IPv4, inicializando el socket
  * y dejandolo en el estado de listeing.
  *
  * @param ip IP de la interfaz en la que se escuchara.
  * @param port Puerto en el cual se escuchara
  * @param q_size Tamaño de la cola de conexiones. (Default: 5)
  *
  * @return Identificador del Socket
  * @retval -1 	Ocurrio un Error y no se pudo crear el Socket
  *
  * Ejemplo de Uso:
  * @code
  *		newTCPServerSocket4("0.0.0.0", 1024) // Crea un nuevo servidor en el puerto 1024 
  * @endcode
  *
  * @ingroup sockets
**/
int newTCPServerSocket4(const char *ip, const u_short port, int q_size);

/**
  * @brief Espera la conexión de un nuevo cliente.
  *
  * Espera un nuevo cliente, y nos regresa el numero de Socket del Cliente
  *
  * @param socket Identificador del Server Socket
  * @param *clientIP En caso de nos ser NULL en dicha ubicación de memoria se regresa la dirección IP del cliente.
  * @param *clientPort En caso de nos ser NULL en dicha ubicación de memoria se regresa el puerto del cliente
  *
  * @return Identificador del Client Socket
  * @retval -1 	Ocurrio un Error y no se pudo crear el Socket
  *
  * Ejemplo de Uso:
  * @code
  *		waitConnection(socket,clientIP,&clientPort) // Espera una conexión y regresa la IP y 
  *		//el puerto del cliente en las ubicaciones de memoria de clientIP y clientPort 
  * @endcode
  *
  * @ingroup sockets
**/
int waitConnection(int socket, char *clientIP=NULL, u_short *clientPort=NULL);

/**
  * @brief Crea un Socket Cliente TCP IPv4
  *
  * Crea un nuevo Socket en IPv4 que se conecta a un servicio TCP.
  *
  * @param ip Dirección IP del Servidor.
  * @param port Numero de puerto al que se debe conectar.
  *
  * @return Identificador del Socket
  * @retval -1 	Ocurrio un Error y no se pudo crear el Socket
  *
  * Ejemplo de Uso:
  * @code
  *		newTCPClientSocket4("127.0.0.1",1024) // Se conecta a localhost puerto 1024
  * @endcode
  *
  * @ingroup sockets
**/
int newTCPClientSocket4(const char *ip, const u_short port);
#endif
