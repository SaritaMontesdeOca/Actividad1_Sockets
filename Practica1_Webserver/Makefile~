#Makefile
#Definición de Variables
CC = g++
CFLAG = -Wall
LIBS = 

#Definición de los archivos .o Requeridos
OBJS = web_server.o 

#Definición del nombre del programa
NAME = web_server

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


