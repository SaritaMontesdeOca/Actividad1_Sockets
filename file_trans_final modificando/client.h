// Sahara Montes de Oca y Fatima De Niz
// Copia de Alvaro Parres

#ifndef SERVER_H
#define SERVER_H

int start_client(const u_short port, const char ip[], const char remoteFilename[], const char localFilename[]);
int get_list(const u_short port, const char ip[]);

#endif
