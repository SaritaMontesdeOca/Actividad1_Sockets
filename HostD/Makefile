#Makefile

#Definición de Variables
CC = g++
CFLAG = -Wall
LIBS = 

#Definición de los archivos .o Requeridos
OBJS = main.o HostClient.o HostServer.o

#Definición del nombre del programa
NAME = hostD

#Creación de Regla por Default
all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAG} ${OBJS} -o $@

%.o: %.c
	${CC} ${CFLAG} -c $< -o $@

clean:
	rm *.o
	rm netlib/*.o
	rm ${NAME}

doc:
	doxygen Doxyfile

#NOTA recordemos que el archivo se estrucutra
#objetivo: LISTA_DE_DEPENDENCIAS
#->TABULADOR->forma de generar el objetivo basado en las dependencias.

