/**
 * @file UDPServer.h
 * 
 * @brief Funciones Referentes al Servidor
 * 
 * Funciones referentes al trabajo como servidor
 * 
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
 * @date Abril 2012
 * @version 0.0.1
 * 
 * \defgroup ServidorUDP
 * */
 
 #ifndef _UDP_SERVER_H
 #define _UDP_SERVER_H
 
 #include <inttypes.h>
 #include <sys/types.h>
 

/**
  * @brief Inicia la Operaciones del Servidor UDP Proyecto
  *
  * Función que inicia la escucha en broadcast del servidor
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
  *		status = startServer(5001) // Inicia el servidor en todas las interfaces en el puerto 5001 
  * @endcode
  *
  * @ingroup ServidorUDP
**/
int startUDPServer(const u_short port);

#endif



