/**
 * @file ipFunc.h
 *
 * @brief Funciones Generales para manejo de direcciones IP
 *
 * Funciones generales para el manejo de direcciones IP
 *
 * @author Alfredo Huerta Larios (rt676272@iteso.mx)
 * @date Febrero 2012
 * @version 0.0.1
 *
 * \defgroup sockets Funciones Referentes a Sockets
**/

#ifndef _IP_FUNC_H
#define _IP_FUNC_H

/**
  * @brief Completa una estructura addr con una dirección IPv4 
  *
  * Inicializa una estructura sockaddr_in zero y completa los valores adecuados
  * con la dirección IPv4 que se recive. 
  *
  * @param ip dirección IP en escritura natural (Ej: 148.201.1.1)
  * @param addr Apuntador a la estrucutra que se completara.
  *
  * @return status.
  * @retval 0  Se completo correctamente la estrucutra.
  * @retval -1 	Ocurrio un Error y no se pudo completar la estrucutra
  *
  * Ejemplo de Uso:
  * @code
  *		struct sockaddr_in addr;	
  *		buildIP4addr("0.0.0.0", &addr): // Completa al estructura addr con la dirección IPv4 0.0.0.0
  * @endcode
  *
  * @ingroup sockets
**/
int buildIP4addr(const char *ip, struct sockaddr_in *addr);

#endif
