/**
 * @file UDPClient.h
 *
 * @brief Funciones Referentes al Cliente UDP
 *
 * Funcion que manda mensaje en broadcast por parte del cliente
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Marzo 2012
 * @version 0.0.1
 *
 * \defgroup ClienteUDP
**/
#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

#include <inttypes.h>
#include <sys/types.h>

/**
  * @brief Inicia la Operaciones del Cliente UDP File transfer
  *
  *
  * @param port Puerto al que se debera de conectar.
  *
  * @return Estado con el que finaliza el cliente.
  * @retval 1 termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		int status;
  *		status = startClient("127.0.0.1",1024) // Inicia el servidor en todas las interfaces en el puerto 1024 
  * @endcode
  *
  * @ingroup ClienteUDP
**/
int startUDPClient(const u_short port);

#endif

