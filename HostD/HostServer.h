/**
 * @file HostServer.h
 * 
 * @brief Funciones Referentes al Servidor
 * 
 * Funciones referentes al trabajo como servidor
 * 
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
 * @date Marzo 2012
 * @version 0.0.1
 * 
 * \defgroup server Funciones relativos al servidor
 * */
 
 #ifndef _HOST_SERVER_H
 #define _HOST_SERVER_H
 
 #include <inttypes.h>
 #include <sys/types.h>
 
 
 /**
  * @brief Inicia la Operaciones del Servidor Ping
  *
  * Función que inicia las tareas del servidor, obtiene un socket y espera el comando PING
  * de un cliente, contestandole con la palabra PONG y desconectando al mismo.
  *
  * @param ip Dirección IP de la interfaz donde se escuchara.
  * @param port Puerto sobre el cual se inicia el servidor.
  *
  * @return Estado con el que finaliza el servidor.
  * @retval 1 termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		int status;
  *		status = startServer("0.0.0.0",1024) // Inicia el servidor en todas las interfaces en el puerto 1024 
  * @endcode
  *
  * @ingroup server
**/
int startServer(char *ip, const u_short port);

#endif


