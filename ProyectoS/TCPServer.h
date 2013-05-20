/**
 * @file TCPServer.h
 * 
 * @brief Funciones Referentes al Servidor TCP
 * 
 * Funciones referentes al servidor TCP
 * @date Abril 2012
 * @version 0.0.1
 * 
 * \defgroup ServidorTCP
 * */
 
 #ifndef _TCP_SERVER_H
 #define _TCP_SERVER_H
 
 #include <inttypes.h>
 #include <sys/types.h>
 

/**
  * @brief Inicia la operacion del servidor TCP
  *
  * Funcion que inicializa el servidor TCP
  * 
  *
  * @param port Puerto sobre el cual se inicia el servidor.
  *
  * @return Estado con el que finaliza el servidor.
  * @retval 1 termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		int status;
  *		status = startTCPServer(ip,port) // Inicia el servidor en todas las interfaces en el puerto 5001 
  * @endcode
  *
  * @ingroup ServidorTCP
**/
int startTCPServer(const char *ip, const u_short port);

#endif




