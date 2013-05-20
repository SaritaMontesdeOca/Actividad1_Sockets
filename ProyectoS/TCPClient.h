/**
 * @file TCPClient.h
 *
 * @brief Funciones Referentes al cliente TCP
 *
 * Funciones referentes al trabajo como cliente TCP
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Marzo 2012
 * @version 0.0.1
 *
 * @defgroup ClienteTCP
**/

/**
  * @brief Inicia la Operaciones del Cliente TCP
  *
  *
  * @param port Puerto al que se debera de conectar.
  * @param ip direccion Ip del servidor
  *
  * @return Estado con el que finaliza el cliente.
  * @retval 1 termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		int status;
  *		status = startTCPClient(listenIP,listenPort) 
  * @endcode
  *
  * @ingroup ClienteTCP
**/
int startTCPClient(const char *ip,const u_short port);
/**
  * @brief Pide la informacion de algun archivo al servidor
  *
  *
  * @param socket socket en el que se conecta al servidor
  *
  * @return Estado con el que finaliza el cliente.
  * @retval 1 termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		int status;
  *		status = startTCPClient("127.0.0.1",1024) // Inicia el servidor en todas las interfaces en el puerto 1024
  * @endcode
  *
  * @ingroup ClienteTCP
**/
int GetFileInfo(int socket);
/**
  * @brief Pide la lista de archivos al servidor
  *
  *
  * @param port Puerto al que se debera de conectar.
  * @param ip direccion Ip del servidor
  *
  * @return Estado con el que finaliza el cliente.
  * @retval 1 termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		int status;
  *		status = startTCPClient("127.0.0.1",1024) // Inicia el servidor en todas las interfaces en el puerto 1024
  * @endcode
  *
  * @ingroup ClienteTCP
**/
int GetFileList(int socket);
/**
  * @brief Pide algun archivo al servidor
  *
  *
  * @param port Puerto al que se debera de conectar.
  * @param ip direccion Ip del servidor
  *
  * @return Estado con el que finaliza el cliente.
  * @retval 1 termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		int status;
  *		status = startTCPClient("127.0.0.1",1024) // Inicia el servidor en todas las interfaces en el puerto 1024
  * @endcode
  *
  * @ingroup ClienteTCP
**/
int GetFile(int socket);
