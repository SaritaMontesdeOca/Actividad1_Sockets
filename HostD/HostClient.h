/**
 * @file HostClient.h
 *
 * @brief Funciones Referentes al Cliente UDP
 *
 * Funciones referentes al trabajo como cliente
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Marzo 2012
 * @version 0.0.1
 *
 * \defgroup client Funciones relativas al Cliente.
**/

/**
  * @brief Inicia la Operaciones del Cliente UDP File transfer
  *
  *
  * @param ip Dirección IP del servidor a conectarse.
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
  * @ingroup client
**/
int startClient(const char *ip, const u_short port);

