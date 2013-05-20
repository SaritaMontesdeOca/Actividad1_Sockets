/**
 * @file Querys.h
 *
 * @brief Funciones Referentes al servidor UDP
 *
 * Funcion que responde el broadcast del cliente por medio de un unicast con la lista de archivos
 *
 * @author Alfredo Huerta (rt676272@iteso.mx)
 * @date Marzo 2012
 * @version 0.0.1
 *
 * \defgroup ServidorUDP
**/

/**
  * @brief Envia la lista de archivos por medio de un unicast al cliente
  *
  *
  * @param clientSocket Socket del cliente.
  * @param ip Ip del cliente.
  * @param num Numero del cliente.
  * @param udpClient Estructura del socket Cliente
  * @param addrlen Estructura de la direccion
  * 
  * @return Estado del envio.
  * @retval true termino sin errores
  * @retval -1 termino con errores.
  *
  * Ejemplo de Uso:
  * @code
  *		
  *		endList(udpSocket,ip,clientes,udpClient,addrlen);  
  * @endcode
  *
  * @ingroup ServidorUDP
**/


int sendList(int clientSocket,char *ip, int num, struct sockaddr_in udpClient, socklen_t addrlen);
