#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.c"

int main(int argc, char *argv[]) {
	directorio_completo = inicializar("simdiskfilled.bin");
	//Escribir despues de este comentario las funciones necesarias
	czFILE* file = cz_open("texto.txt", 'r');
	cz_close(file);
	return 0;
}