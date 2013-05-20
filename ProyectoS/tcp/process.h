/**
 * @file process.h
 *
 * @brief Funciones Referentes al servidor TCP
 *
 * Funcion que contesta las peticiones del usuario TCP
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Marzo 2012
 * @version 0.0.1
 *
 * \defgroup ServidorTCP
**/
#ifndef _PROCESS_H
#define _PROCESS_H

/**
  * @brief Empieza el intercambio de mensajes con el cliente.
  *
  *
  * @param clientSocket Socket del cliente.
  * @param client Numero de cliente conectado

  * 
  * @return Estado del envio.
  * @retval true termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		
  *		DialogosTCP(clientSocket,client);  
  * @endcode
  *
  * @ingroup ServidorTCP
**/
int DialogosTCP(int clientSocket, int client);
/**
  * @brief Envia la lista de archivos al cliente.
  *
  *
  * @param clientSocket Socket del cliente.
  * @param msg mensaje enviado por el cliente
  * 
  * @return Estado del envio.
  * @retval true termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		
  *		FileList(clientSocket,msg);  
  * @endcode
  *
  * @ingroup ServidorTCP
**/
int FileList(int clientSocket, char *msg);
/**
  * @brief Envia el archivo solicitado por el cliente=
  * Envia el archivo solicitado al cliente, antes de transmitir cada parte del archivo se envia la cabecera con los datos correspondientes
  *
  * @param clientSocket Socket del cliente.
  * @param msg mensaje enviado por el cliente
  * 
  * @return Estado del envio.
  * @retval true termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		
  *		FileTransfer(clientSocket,msg);  
  * @endcode
  *
  * @ingroup ServidorTCP
**/
int FileTransfer(int clientSocket, char *msg);
/**
  * @brief Envia la informacion de un archivo al cliente 
  * Envia la informacion de un archivo al cliente, como el tamanio del archivo y la cadena MD5
  *
  * @param clientSocket Socket del cliente.
  * @param msg mensaje enviado por el cliente

  * 
  * @return Estado del envio.
  * @retval true termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		
  *		FileInfo(clientSocket,msg);  
  * @endcode
  *
  * @ingroup ServidorTCP
**/
int FileInfo(int clientSocket, char *msg);


#endif
